
/*****************************************************************************
* Copyright [2017-2019] [MTSQuant]
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*****************************************************************************/
#include "OrderMgrBase.h"
#include "base/MtsLog.h"
#include "PositionMgr.h"
#include "mts_core/Trade.h"
#include "mts_core/Order.h"
#include "mts_core/Position.h"
#include "mts_core/Environment.h"
#include "mts_core/OrderType.h"
#include "mts_core/TradingDateMgr.h"
#include "MtsMainThread.h"
#include "base/PerfTest.h"
#include "OrderFillMgr.h"
#include "TradingAccountMgr.h"
#include "PositionPersistence.h"

#include <QtCore/QFile>
#include <QtCore/QDir>

namespace mts
{
	
	OrderMgrBase::OrderMgrBase()
		:_notify(nullptr)
	{
		assert(Environment::instance()->isCurrentMtsThread());
	}


	void OrderMgrBase::doProcessOrder(Order* order, Order* lastorder, Position* pos)
	{
		if (!pos) {
			pos = getPosition(order->instrumentId());
		}
		pos->processOrder(order, lastorder);
		if (auto* strategyPos = getPosition(order->instrumentId(), order->instanceId(), order->strategyId()))
		{
			strategyPos->processOrder(order, lastorder);
			doPersistPos(strategyPos, order->instanceId(), order->strategyId());
		}
		
	}

	OrderMgrBase::~OrderMgrBase()
	{
		qDeleteAll(_persistence);
	}


	/*void OrderMgrBase::setTrade(Trade* trader)
	{
	_trader = trader;
	}*/

	void OrderMgrBase::setStrategy(StrategyInterface* notify/*StrategyMgr*/)
	{
		_notify = notify;
	}

	Position* OrderMgrBase::getPosition(const InstrumentId& id)
	{
		return PositionMgr::instance()->getPosition(id, GetMode::GM_GET_OR_CREATE);
	}


	mts::Position* OrderMgrBase::getPosition(const InstrumentId& id, int instanceId, int strategyId)
	{
		return PositionMgr::instance()->getPosition(id, instanceId, strategyId, GetMode::GM_GET_OR_CREATE);
	}

	QList<Order*> OrderMgrBase::allOrders() const
	{
		return OrderSetSingleton::instance()->allOrders();
	}

	QList<Order*> OrderMgrBase::allActiveOrders(const InstrumentId& id) const {
		return OrderSetSingleton::instance()->allActiveOrders(id);
	}

	Order* OrderMgrBase::getOrder(const QString&  referenceId)
	{
		return OrderSetSingleton::instance()->getOrder(referenceId);
	}

	Order* OrderMgrBase::getOrder(const QString& exchId, ExchId exch)
	{
		return OrderSetSingleton::instance()->getOrder(exchId, exch);
	}
	struct SplitOrdInfo
	{
		SplitOrdInfo(double size, DirectionSide dside, CombOffsetFlag flag) //TODO check double //TODO ??
			:volume(size), side(dside), offset(flag) {
		}
		double volume; //TODO check double
		DirectionSide side;
		CombOffsetFlag offset;
	};

	QList<SplitOrdInfo> splitOrderInfo(OrderActionNew* newaction, Position* pos)
	{
		QList<SplitOrdInfo> ords;
		double volume = newaction->volume(); //TODO check double
		bool isSHFE = exchHasBothPos(pos->instrumentId().exchId);
		QList<QPair<int, DirectionSide>> splitOrders;
		double leftVolume = volume; //TODO check double
		if (volume > 0) {
			double sLeftVolume = pos->shortLeftVolume(); //TODO check double
			if (isSHFE) {
				double sLeftYdVolume = pos->shortLeftYesterdayVolume(); //TODO check double
				if (sLeftYdVolume < 0) {
					double cYdVolume = std::min(-sLeftYdVolume, leftVolume); //TODO check double
					sLeftVolume += cYdVolume;
					leftVolume -= cYdVolume;
					ords.push_back(SplitOrdInfo(cYdVolume, DirectionSide::D_COVER, CombOffsetFlag::OF_CLOSEYESTERDAY));
				}
			}
			if (leftVolume > 0 && sLeftVolume < 0) {
				int cVolume = std::min(-sLeftVolume, leftVolume);
				leftVolume -= cVolume;
				ords.push_back(SplitOrdInfo(cVolume, DirectionSide::D_COVER, isSHFE ? CombOffsetFlag::OF_CLOSETODAY : CombOffsetFlag::OF_CLOSE));
			}
			if (leftVolume > 0) {
				ords.push_back(SplitOrdInfo(leftVolume, DirectionSide::D_BUY, CombOffsetFlag::OF_OPEN));
			}
		}
		else {
			double lLeftVolume = pos->longLeftVolume(); //TODO check double
			if (isSHFE) {
				double lLeftYdVolume = pos->longLeftYesterdayVolume(); //TODO check double
				if (lLeftYdVolume > 0) {
					double sYdVolume = std::max(-lLeftYdVolume, leftVolume); //TODO check double
					lLeftVolume += sYdVolume;
					leftVolume -= sYdVolume;
					ords.push_back(SplitOrdInfo(sYdVolume, DirectionSide::D_SELL, CombOffsetFlag::OF_CLOSEYESTERDAY));
				}
			}
			if (leftVolume < 0 && lLeftVolume > 0) {
				double sVolume = std::max(-lLeftVolume, leftVolume); //TODO check double
				leftVolume -= sVolume;
				ords.push_back(SplitOrdInfo(sVolume, DirectionSide::D_SELL, isSHFE ? CombOffsetFlag::OF_CLOSETODAY : CombOffsetFlag::OF_CLOSE));
			}
			if (leftVolume < 0) {
				ords.push_back(SplitOrdInfo(leftVolume, DirectionSide::D_SHORT, CombOffsetFlag::OF_OPEN));
			}
		}
		return ords;
	}

	QString OrderMgrBase::sendOrder(OrderActionNew*  newaction, int orderType)
	{
		double volume = newaction->volume(); //TODO check double
		if (volume == 0) {
			MTS_ERROR("Missing volume %s\n", qPrintable(newaction->instrumentId().toString()));
			return "";
		}
		newaction->setPrice(qAbs(newaction->price()));
		switch (newaction->directionSide()) {
		case DirectionSide::D_BUY:
		case DirectionSide::D_COVER:
			newaction->setVolume(abs(volume));
			break;
		case DirectionSide::D_SELL:
		case DirectionSide::D_SHORT:
			newaction->setVolume(-abs(volume));
			break;
		default:
			break;
		}
		newaction->setInstanceId(Environment::instance()->instanceId());
		newaction->setReferenceId(Environment::instance()->genOrderReferenceId(newaction->strategyId(), orderType, newaction->directionSide(), newaction->priceType(), newaction->instrumentId()));
		Position* pos = getPosition(newaction->instrumentId());
		if (newaction->tradingDay() <= 0) {
			newaction->setTradingDay(TradingDateMgr::instance()->tradingDate(DateTime::now()));
		}
		MTS_FILE("SendOrder:%s\n", qUtf8Printable(newaction->toJsonString()));
		if (doSendOrder(newaction, pos)) {
			return newaction->referenceId();
		}
		else {
			return "";
		}
	}

	bool OrderMgrBase::doCheckRisk(OrderActionNew* newaction, Position* pos) {
		return true;
	}

	bool OrderMgrBase::doSendOrder(OrderActionNew* newaction, Position* pos)
	{
		if (newaction->referenceId().isEmpty()) {
			OrderReportNewReject rjt;
			rjt.copyFrom(newaction);
			rjt.setNote("Failed GenOrderId");
			if (_notify && Environment::instance()->isCurrentInstanceInstanceId(rjt.instanceId())) {
				_notify->onOrderNewReject(&rjt);
			}
			return false;
		}
		Order* ord = OrderSetSingleton::instance()->getOrder(newaction->referenceId(), true);
		ord->processOrderActionNew(newaction);
		if (newaction->instrumentId().typeId == mts::TYPE_FUTR || newaction->instrumentId().typeId == mts::TYPE_STK_SPOT) { //TODO
			if (!checkStrategyPos(newaction))
			{
				OrderReportNewReject rjt;
				rjt.copyFrom(newaction);
				rjt.setNote("NotEnough Pos");
				ord->processOrderReportNewReject(&rjt);
				if (_notify && Environment::instance()->isCurrentInstanceInstanceId(rjt.instanceId())) {
					_notify->onOrderNewReject(&rjt);
				}
				return false;

			}
			if (!doCheckRisk(newaction, pos)) {
				OrderReportNewReject rjt;
				rjt.copyFrom(newaction);
				rjt.setNote("RiskFailed");
				ord->processOrderReportNewReject(&rjt);
				OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_NEW_REJECT);
				if (_notify && Environment::instance()->isCurrentInstanceInstanceId(rjt.instanceId())) {
					_notify->onOrderNewReject(&rjt);
				}
				return false;
			}
		}
		
		if (!Environment::instance()->trade()->sendOrderNewAction(newaction)) {
			OrderReportNewReject rjt;
			ord->copyTo(&rjt);
			rjt.setNote("Failed to send");
			ord->processOrderReportNewReject(&rjt);
			OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_NEW_REJECT);
			if (_notify&& Environment::instance()->isCurrentInstanceInstanceId(rjt.instanceId())) {
				_notify->onOrderNewReject(&rjt);
			}
			return false;
		}
		doProcessOrder(ord, nullptr, pos);
		return true;
	}

	bool OrderMgrBase::checkStrategyPos(OrderActionNew* act)
	{
		static bool check = Environment::instance()->strategyPosCheck() != 0;
		Position* pos = getPosition(act->instrumentId(), act->instanceId(), act->strategyId());
		bool isOk = true;
		if (pos && check && act->strategyId() != DEFAULT_STRATEGY_ID)
		{
			switch (act->directionSide()) {
			case DirectionSide::D_SELL:
				isOk = (pos->longCurrentVolume() + pos->longActiveCloseVolume() + act->volume() >= 0);
				break;
			case DirectionSide::D_COVER:
				isOk = (pos->shortCurrentVolume() + pos->shortActiveCloseVolume() + act->volume() <= 0);
				break;
			default:
				break;
			}
		}
		return isOk;
	}

	bool OrderMgrBase::cancelOrder(OrderActionCancel* cxlaction)
	{
		MTS_FILE("CancelOrder:%s\n", qUtf8Printable(cxlaction->toJsonString()));
		if (Order* ord = getOrder(cxlaction->referenceId())) {
			cxlaction->setStrategyId(ord->strategyId());
			cxlaction->setInstanceId(ord->instanceId());
			cxlaction->setOrderExchId(ord->orderExchId());
			Order lastorder = *ord;
			ord->processOrderActionCancel(cxlaction);
			if (!Environment::instance()->trade()->sendOrderCancelAction(cxlaction))
			{
				OrderReportCancelReject rjt;
				ord->copyTo(&rjt);
				rjt.setNote("Failed to cancel");
				ord->processOrderReportCancelReject(&rjt);
				OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_CANCEL_REJECT);
				if (_notify&& Environment::instance()->isCurrentInstanceInstanceId(rjt.instanceId())) {
					_notify->onOrderCancelReject(&rjt);
				}
			}
			doProcessOrder(ord, &lastorder);
			return true;
		}
		return false;
	}

	void OrderMgrBase::initStrategyPos(int strategyId, const QString& dir)
	{
		bool positionPersistenceEnable = Environment::instance()->positionPersistenceEnable();
		MTS_LOG("position Persistence enable:%d\n", positionPersistenceEnable);
		if (!positionPersistenceEnable) {
			return;
		}
		if (dir.isEmpty()) {
			return;
		}
		if (!QDir().mkpath(dir)) {
			MTS_FILE("Invalid persist path:%s\n", qUtf8Printable(dir));
			return;
		}
		int instanceId = Environment::instance()->instanceId();
		QString key = PositionMgr::getStrategyKey(instanceId, strategyId);
		auto it = _persistence.find(key);
		if (it != _persistence.end()) {
			delete it.value();
			_persistence.erase(it);
		}
		auto* p = new PositionPersistence(key, dir);
		auto poss = p->getYesterdayPos();
		_persistence[key] = p;
		for (auto& pos : poss)
		{
			double l = pos->longCurrentVolume(); //TODO check double
			double s = pos->shortCurrentVolume(); //TODO check double
			pos->clear();
			pos->setLongOpenVolume(l);
			pos->setShortOpenVolume(s);
			doPositionUpdate(pos, instanceId, strategyId);
		}
		qDeleteAll(poss);
	}

	void OrderMgrBase::onOrderNewDone(OrderReportNewDone* newdone)
	{
		Order* ord = getOrder(newdone->referenceId());
		if (!ord) {
			return;
		}
		bool isCurrentInstanceReport = Environment::instance()->isCurrentInstanceInstanceId(newdone->instanceId());
		if (isCurrentInstanceReport) {
			MTS_FILE("OrderReportNewDone:%s\n", qUtf8Printable(newdone->toJsonString()));
		}
		else {
			MTS_FILE("OuterOrderReportNewDone:%s\n", qUtf8Printable(newdone->toJsonString()));
		}
		Order lastorder = *ord;
		ord->processOrderReportNewDone(newdone);
		OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_NEW_DONE);
		Position* pos = getPosition(ord->instrumentId());
		doProcessOrder(ord, &lastorder, pos);
		newdone->setPerfNote(ord->perfNote());
		if (_notify&& isCurrentInstanceReport) {
			_notify->onOrderNewDone(newdone);
			_notify->onOrderUpdate(ord);
			_notify->onPositionUpdate(pos);
		}
	}

	void OrderMgrBase::onOrderNewReject(OrderReportNewReject* newrjt)
	{
		Order* ord = getOrder(newrjt->referenceId());
		if (!ord) {
			return;
		}
		bool isCurrentInstanceReport = Environment::instance()->isCurrentInstanceInstanceId(newrjt->instanceId());
		if (isCurrentInstanceReport) {
			MTS_FILE("OrderReportNewReject:%s\n", qUtf8Printable(newrjt->toJsonString()));
		}
		else {
			MTS_FILE("OuterOrderReportNewReject:%s\n", qUtf8Printable(newrjt->toJsonString()));
		}
		Order lastorder = *ord;
		ord->processOrderReportNewReject(newrjt);
		OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_NEW_REJECT);
		Position* pos = getPosition(ord->instrumentId());
		doProcessOrder(ord, &lastorder, pos);
		if (_notify&& isCurrentInstanceReport) {
			_notify->onOrderNewReject(newrjt);
			_notify->onOrderUpdate(ord);
			_notify->onPositionUpdate(pos);
		}
	}

	void OrderMgrBase::onOrderFill(OrderReportFill* fillrpt)
	{
		Order* ord = getOrder(fillrpt->referenceId());
		if (!ord) {//有些无法恢复reference id
			ord = getOrder(fillrpt->orderExchId(), fillrpt->instrumentId().exchId);
			if (!ord) {
				return;
			}
			fillrpt->setReferenceId(ord->referenceId());
		}
		bool isCurrentInstanceReport = Environment::instance()->isCurrentInstanceInstanceId(fillrpt->instanceId());
		if (isCurrentInstanceReport) {
			MTS_FILE("OrderReportFill:%s\n", qUtf8Printable(fillrpt->toJsonString()));
		}
		else {
			MTS_FILE("OuterOrderReportFill:%s\n", qUtf8Printable(fillrpt->toJsonString()));
		}
		Order lastorder = *ord;
		OrderReportFill* lastFillrpt = OrderFillMgr::instance()->getFill(fillrpt->fillId(), fillrpt->directionSide(), fillrpt->instrumentId().exchId);
		ord->processOrderReportFill(fillrpt, lastFillrpt);
		OrderFillMgr::instance()->updateFill(fillrpt);
		OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_FILL);
		Position* pos = getPosition(ord->instrumentId());
		doProcessOrder(ord, &lastorder, pos);
		MTS_FILE("Position:%s\n", qUtf8Printable(pos->toJsonString()));
		if (_notify&& isCurrentInstanceReport) {
			_notify->onOrderFill(fillrpt);
			_notify->onOrderUpdate(ord);
			_notify->onPositionUpdate(pos);
		}
	}

	void OrderMgrBase::onOrderCancelDone(OrderReportCancelDone* cxldone)
	{
		Order* ord = getOrder(cxldone->referenceId());
		if (!ord) {
			return;
		}
		bool isCurrentInstanceReport = Environment::instance()->isCurrentInstanceInstanceId(cxldone->instanceId());
		if (isCurrentInstanceReport) {
			MTS_FILE("OrderReportCancelDone:%s\n", qUtf8Printable(cxldone->toJsonString()));
		}
		else {
			MTS_FILE("OuterOrderReportCancelDone:%s\n", qUtf8Printable(cxldone->toJsonString()));
		}
		Order lastorder = *ord;
		ord->processOrderReportCancelDone(cxldone);
		OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_CANCEL_DONE);
		Position* pos = getPosition(ord->instrumentId());
		doProcessOrder(ord, &lastorder, pos);
		if (_notify&& isCurrentInstanceReport) {
			_notify->onOrderCancelDone(cxldone);
			_notify->onOrderUpdate(ord);
			_notify->onPositionUpdate(pos);
		}
	}

	void OrderMgrBase::onOrderCancelReject(OrderReportCancelReject* cxlrjt)
	{
		Order* ord = getOrder(cxlrjt->referenceId());
		if (!ord) {
			return;
		}
		bool isCurrentInstanceReport = Environment::instance()->isCurrentInstanceInstanceId(cxlrjt->instanceId());
		if (isCurrentInstanceReport) {
			MTS_FILE("OrderReportCancelReject:%s\n", qUtf8Printable(cxlrjt->toJsonString()));
		}
		else {
			MTS_FILE("OuterOrderReportCancelReject:%s\n", qUtf8Printable(cxlrjt->toJsonString()));
		}
		Order lastorder = *ord;
		ord->processOrderReportCancelReject(cxlrjt);
		OrderSetSingleton::instance()->updateOrder(ord, OrderReportType::ORT_CANCEL_REJECT);
		Position* pos = getPosition(ord->instrumentId());
		doProcessOrder(ord, &lastorder, pos);
		if (_notify&& isCurrentInstanceReport) {
			_notify->onOrderCancelReject(cxlrjt);
			_notify->onOrderUpdate(ord);
			_notify->onPositionUpdate(pos);
		}
	}

	void OrderMgrBase::onOrderUpdate(Order* order)
	{
		if (Order* last = getOrder(order->referenceId())) {
			doProcessOrder(order, last);
		}else {
			Order* newOrd = OrderSetSingleton::instance()->getOrder(order->referenceId(), true);
			order->copyTo(newOrd);
			OrderSetSingleton::instance()->updateOrder(newOrd, OrderReportType::ORT_ORDER);
			doProcessOrder(newOrd, nullptr);
		}
	}

	void OrderMgrBase::onPositionUpdate(Position * position) {
		doPositionUpdate(position, Environment::instance()->instanceId(), DEFAULT_STRATEGY_ID);
		_notify->onPositionUpdate(position);
	}

	void OrderMgrBase::doPositionUpdate(Position* position, int instanceId, int strategyId)
	{
		if (Position* pos = getPosition(position->instrumentId(), instanceId, strategyId)) {
			pos->setLongOpenVolume(position->longOpenVolume());
			pos->setLongCurrentVolume(position->longCurrentVolume());
			pos->setShortOpenVolume(position->shortOpenVolume());
			pos->setShortCurrentVolume(position->shortCurrentVolume());
		}
	}

	void OrderMgrBase::doPersistPos(Position* pos, int instanceId, int strategyId)
	{
		static bool positionPersistenceEnable = Environment::instance()->positionPersistenceEnable();
		if (!positionPersistenceEnable) {
			return;
		}
		auto it = _persistence.find(PositionMgr::getStrategyKey(instanceId, strategyId));
		if (it != _persistence.end()) {
			it.value()->updateTodayPosition(pos);
		}
	}

	void OrderMgrBase::onTradingAccountUpdate(TradingAccount* ta)
	{
		TradingAccountMgr::instance()->updateTradingAccount(ta);
	}

	void OrderMgrBase::onBusinessDateChanged(int businessDate)
	{
		if (!TradingDateMgr::instance()->is24Hour()) {
			OrderSetSingleton::instance()->clear();
		}
		PositionMgr::instance()->businessDateChanged(businessDate);
		if (_notify) {
			_notify->onBusinessDateChanged(businessDate);
		}
	}

}
