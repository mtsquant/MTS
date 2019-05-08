
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
#include "StrategyHub.h"
#include <boost/bind.hpp>
#include "base/MtsLog.h"
#include "mts_core/Environment.h"
#include "mts_core/ConfigParams.h"
#include "MtsProxy.h"
#include "mts_core/OrderType.h"
#include "mts_core/InstrumentPropertyDb.h"

namespace mts
{


	StrategyHub::StrategyHub()
	{
	}


	StrategyHub::~StrategyHub() {
	}

	bool StrategyHub::registerStrategy(StrategyInterface * strategy) {
		QString strategyName = QString::fromStdString(strategy->name());
		auto strategyId = strategy->strategyId();
		if (!isStrategyIdValid(strategyId)) {
			MTS_ERROR("Invalid strategy id %d , it should be 1~9\n", strategyId);
			return false;
		}
		if (_idStrategies.contains(strategyId)) {
			MTS_ERROR("Duplicated strategy id %d\n", strategyId);
			return false;
		}
		_idStrategies[strategyId] = strategy;
		return true;
	}

	void StrategyHub::unregisterStrategy(StrategyInterface * strategy) {
		QString strategyName = QString::fromStdString(strategy->name());
		_idStrategies.remove(strategy->strategyId());

		Q_FOREACH(int timerId, _timerStrategies.keys()) {
			_timerStrategies.remove(timerId);
		}

		Q_FOREACH(auto id, _quoteStrategies.keys()) {
			_quoteStrategies[id].remove(strategy);
		}
	}


	bool StrategyHub::initialize(const QVariantMap & params) {
		QVariantMap wholeParams = AbstractConfigParams::expendAllSections(params);
		return MtsProxy::instance()->initialize(this, wholeParams);
	}


	int StrategyHub::newTimer(StrategyInterface * strategy, int beginTime, int interval) {
		int timerId = MtsProxy::instance()->newTimer(beginTime, interval);
		_timerStrategies[timerId] = strategy;
		return timerId;
	}

	int StrategyHub::newOnceTimer(StrategyInterface * strategy, int beginTime) {
		int timerId = newTimer(strategy, beginTime, 0);
		_onceTimerIds.insert(timerId);
		return timerId;
	}

	bool StrategyHub::removeTimer(StrategyInterface * strategy, int id) {
		if (MtsProxy::instance()->removeTimer(id)) {
			_timerStrategies.remove(id);
			return true;
		}
		return false;
	}

	QString StrategyHub::newOrder(StrategyInterface * strategy, OrderActionNew * orderActionNew, int orderType) {
		orderActionNew->setStrategyId(strategy->strategyId());

		QString ordId = MtsProxy::instance()->newOrder(orderActionNew, orderType);
		if (ordId.isEmpty()) {
			assert(orderActionNew->strategyId() == strategy->strategyId());
		}
		return ordId;
	}

	bool StrategyHub::cancelOrder(StrategyInterface * strategy, OrderActionCancel * orderActionCancel) {
		orderActionCancel->setStrategyId(strategy->strategyId());
		if (MtsProxy::instance()->cancelOrder(orderActionCancel)) {
			auto id = orderActionCancel->referenceId();
			assert(!id.isEmpty());
			return true;
		}
		return false;
	}

	bool StrategyHub::subscribeQuotes(StrategyInterface * strategy, const QList<InstrumentId>& instruments) {
		MtsProxy::instance()->subscribeQuotes(instruments); //allow subscribe many times
		for (int i = 0, size = instruments.size(); i < size; ++i) {
			const InstrumentId& id = instruments[i];
			_quoteStrategies[id].insert(strategy);
		}
		return true;
	}

	bool StrategyHub::unsubscribeQuotes(StrategyInterface * strategy, const QList<InstrumentId>& instruments) {
		MtsProxy::instance()->unsubscribeQuotes(instruments);
		for (int i = 0, size = instruments.size(); i < size; ++i) {
			const InstrumentId& id = instruments[i];
			_quoteStrategies[id].remove(strategy);
			if (_quoteStrategies[id].isEmpty()) {
				_quoteStrategies.remove(id);
			}
		}
		return true;
	}


	void StrategyHub::unsubscribeAllQuotes(StrategyInterface * strategy) {
		auto ids = _quoteStrategies.keys();
		unsubscribeQuotes(strategy, ids);
	}

	QList<TradingAccount*> StrategyHub::getAllTradingAccounts() const
	{
		return MtsProxy::instance()->getAllTradingAccounts();
	}

	Position * StrategyHub::getPosition(const InstrumentId & id) const {
		return MtsProxy::instance()->getPosition(id);
	}

	Position* StrategyHub::getPosition(const InstrumentId& id, int instanceId, int strategyId) const
	{
		return MtsProxy::instance()->getPosition(id,instanceId,strategyId);
	}

	QList<Order*> StrategyHub::getAllOrders(StrategyInterface * strategy) const {
		return MtsProxy::instance()->getAllOrders();
	}

	QList<Order*> StrategyHub::getActiveOrders(StrategyInterface * strategy, const InstrumentId & id) const {
		return MtsProxy::instance()->getActiveOrders(id);
	}

	QList<OrderReportFill*> StrategyHub::getAllOrderFills() const
	{
		return MtsProxy::instance()->getAllOrderFills();
	}

	Quote * StrategyHub::getQuote(const InstrumentId & id) const {
		return MtsProxy::instance()->getQuote(id);
	}

	QList<Position*> StrategyHub::getAllPositions(int strategyId /*= DEFAULT_STRATEGY_ID*/) const {
		return MtsProxy::instance()->getAllPositions(strategyId);
	}

	DateTime StrategyHub::getNow() const {
		return MtsProxy::instance()->getNow();
	}

	int mts::StrategyHub::currentTradingDay() const {
		return MtsProxy::instance()->currentTradingDay();
	}
	
	QList<InstrumentId> StrategyHub::instrumentIds()const {
		return MtsProxy::instance()->instrumentIds();
	}


	int StrategyHub::subscribedInstrumentCount() const
	{
		return _quoteStrategies.keys().size();
	}

	QJsonArray StrategyHub::allStrategiesToJson() const
	{
		QJsonArray jarray;
		for (auto it = _idStrategies.cbegin(); it != _idStrategies.constEnd(); it++)
		{
			QJsonObject obj;
			obj.insert("name", QString::fromStdString(it.value()->name()));
			obj.insert("instanceId", Environment::instance()->instanceId());
			obj.insert("strategyId", it.value()->strategyId());
			jarray.append(obj);
		}
		return jarray;
	}

	std::string StrategyHub::allStrategiesToJsonString() const
	{
		return QJsonDocument(allStrategiesToJson()).toJson().simplified().toStdString();
	}

	TradingAccount * StrategyHub::getTradingAccount(CurrencyId currency) const {
		return MtsProxy::instance()->getTradingAccount(currency);
	}


	int StrategyHub::exec() {
		return MtsProxy::instance()->exec();
	}

	void StrategyHub::exit(int v) {
		MtsProxy::instance()->exit(v);
	}

	void StrategyHub::onEnvirInitialized() {
		for (auto it = _idStrategies.constBegin(), itEnd = _idStrategies.constEnd(); it != itEnd; ++it) {
			StrategyInterface* strategy = it.value();
			MtsProxy::instance()->initStrategyPos(strategy->strategyId(), strategy->strategyPath().c_str());
			strategy->onEnvirInitialized();
		}
	}

	void StrategyHub::onInitialized(Account * account) {
		for (auto it = _idStrategies.constBegin(), itEnd = _idStrategies.constEnd(); it != itEnd; ++it) {
			StrategyInterface* strategy = it.value();
			strategy->onInitialized(account);
		}
	}



	void StrategyHub::onTimeout(int timerId) {
		auto it = _timerStrategies.constFind(timerId);
		if (it != _timerStrategies.constEnd())
		{
			it.value()->onTimeout(timerId);
			auto sit = _onceTimerIds.constFind(timerId);
			if (sit != _onceTimerIds.constEnd())
			{
				_onceTimerIds.erase(sit);
				_timerStrategies.erase(it);
			}
		}
	}

	void StrategyHub::onQuoteSnapshotUpdate(QuoteSnapshotPtr qt) {
		auto id = qt->instrumentId();
		auto strategySet = _quoteStrategies.value(id);
		for (auto it = strategySet.constBegin(), itEnd = strategySet.constEnd(); it != itEnd; ++it) {
			StrategyInterface* strategy = (*it);
			strategy->onQuoteSnapshotUpdate(qt);
		}
	}

	void StrategyHub::removeAllOnceTimerIds() {
		for (auto it = _onceTimerIds.constBegin(), itEnd = _onceTimerIds.constEnd(); it != itEnd; ++it) {
			int timerId = (*it);
			StrategyInterface * strategy = _timerStrategies[timerId];
			this->removeTimer(strategy, timerId);
		}
		_onceTimerIds.clear();
	}

	void StrategyHub::onBusinessDateChanged(int businessDate) {
		this->removeAllOnceTimerIds();
		for (auto it = _idStrategies.constBegin(), itEnd = _idStrategies.constEnd(); it != itEnd; ++it) {
			StrategyInterface* strategy = it.value();
			strategy->onBusinessDateChanged(businessDate);
		}
	}

	QList<StrategyInterface*> StrategyHub::getCallbacks(int strategyId) const
	{
		QList<StrategyInterface*> ins;
		if (StrategyInterface* strategy = _idStrategies.value(strategyId))
		{
			ins.push_back(strategy);
		}
		if (StrategyInterface* strategy = _idStrategies.value(MONTOR_STRATEGYID_DEFAULT))
		{
			ins.push_back(strategy);
		}
		return ins;
	}

	void StrategyHub::onOrderNewDone(OrderReportNewDone * orderNewDone) {
		for (auto& strategy : getCallbacks(orderNewDone->strategyId()))
		{
			strategy->onOrderNewDone(orderNewDone);
		}
	}

	void StrategyHub::onOrderNewReject(OrderReportNewReject * orderNewReject) {
		for (auto& strategy : getCallbacks(orderNewReject->strategyId()))
		{
			strategy->onOrderNewReject(orderNewReject);
		}
	}

	void StrategyHub::onOrderFill(OrderReportFill * orderFill) {
		for (auto& strategy : getCallbacks(orderFill->strategyId()))
		{
			strategy->onOrderFill(orderFill);
		}
	}

	void StrategyHub::onOrderCancelDone(OrderReportCancelDone * orderCancelDone) {
		for (auto& strategy : getCallbacks(orderCancelDone->strategyId()))
		{
			strategy->onOrderCancelDone(orderCancelDone);
		}
	}

	void StrategyHub::onOrderCancelReject(OrderReportCancelReject * orderCancelReject) {
		for (auto& strategy : getCallbacks(orderCancelReject->strategyId()))
		{
			strategy->onOrderCancelReject(orderCancelReject);
		}
	}

	void StrategyHub::onOrderOtherReport(OrderReport * orderReport) {
		for (auto& strategy : getCallbacks(orderReport->strategyId()))
		{
			strategy->onOrderOtherReport(orderReport);
		}
	}



	void StrategyHub::onBarUpdate(CalcBarPtr bar) {
		auto id = bar->instrumentId();
		auto strategySet = _quoteStrategies.value(id);
		for (auto it = strategySet.constBegin(), itEnd = strategySet.constEnd(); it != itEnd; ++it) {
			StrategyInterface* strategy = (*it);
			strategy->onBarUpdate(bar);
		}
	}

	void StrategyHub::onOrderUpdate(Order * order) {
		for (auto& strategy : getCallbacks(order->strategyId()))
		{
			strategy->onOrderUpdate(order);
		}
	}

	void StrategyHub::onPositionUpdate(Position *pos) {
		auto id = pos->instrumentId();
		mts::InstrumentBaseProperty* prop = mts::InstrumentPropertyDb::instance()->find(id);
		if (!prop) {
			MTS_ERROR("can not find instrument prop:%s\n", qPrintable(id.toString()));
			return;
		}
		if (prop->createDate() < 0) { //鐏竵鏈変竴浜涜櫄鎷熶粨浣嶉渶瑕佸洖璋�
			for (auto it = _idStrategies.constBegin(), itEnd = _idStrategies.constEnd(); it != itEnd; ++it) {
				StrategyInterface* strategy = it.value();
				strategy->onPositionUpdate(pos);
			}
		} else {
			auto strategySet = _quoteStrategies.value(id);
			for (auto it = strategySet.constBegin(), itEnd = strategySet.constEnd(); it != itEnd; ++it) {
				StrategyInterface* strategy = (*it);
				strategy->onPositionUpdate(pos);
			}
		}
	}


}
