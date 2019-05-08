
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
#include "Order.h"
#include "base/DateTime.h"
#include <QJsonDocument>

namespace mts {

	Order::Order()
		:OrderReportNewDone(OrderReportType::ORT_ORDER),_status(OrderStatus::OS_NULL), _lastCxlTimestamp(0), _notifyToStrategy(false)
	{

	}
	Order::Order(const Order& order)
        :OrderReportNewDone(order)
	{
		order.copyTo(this);
	}
	Order::~Order()
	{
	}

	double Order::leftVolume() const //TODO check double
	{
		if (isFinished()) {
			return 0;
		}
		return OrderReportNewDone::leftVolume();
	}

	int Order::status() const
	{
		return _status;
	}
	
	bool Order::isFinished() const
	{
		return (status()&OrderStatus::OS_NEW_REJECT) ||
				 (status()&OrderStatus::OS_CANCEL_DONE) ||
				 (status()&OrderStatus::OS_ORDER_DONE);
	}

	void Order::setStatus(int status)
	{
		_status = status;
	}

	void Order::processOrderActionNew(OrderActionNew* actionNew)
	{
		setTradingAccountId(actionNew->tradingAccountId());
		setInstrumentId(actionNew->instrumentId());
		setBook(actionNew->book());
		setTradingDay(actionNew->tradingDay());
		setOrderType(actionNew->orderType());
		setReferenceId(actionNew->referenceId());
		setVolume(actionNew->volume());
		setPrice(actionNew->price());
		setPriceType(actionNew->priceType());
		setDirectonSide(actionNew->directionSide());
		setOffsetFlag(actionNew->offsetFlag());
		setTimeCondition(actionNew->timeCondition());
		setCreateSrc(actionNew->createSrc());
		setStatus(status() | OrderStatus::OS_NEW_REQUEST);
		setCreateTicksSinceEpoch(actionNew->createTicksSinceEpoch());
		setStrategyId(actionNew->strategyId());
		setInstanceId(actionNew->instanceId());
		setPerfNote(actionNew->perfNote());
	}

	void Order::processOrderReportNewDone(OrderReportNewDone* newDone)
	{
		setOrderExchId(newDone->orderExchId());
        setStatus(status() | (OrderStatus::OS_NEW_DONE& ~OrderStatus::OS_NEW_REJECT));
		const QString& note = newDone->note();
		if (!note.isEmpty()) {
			setNote(note);
		}
		setPerfNote(perfNote()+"|"+newDone->perfNote());
	}

	void Order::processOrderReportNewReject(OrderReportNewReject* newRjt)
	{
        setStatus((status() & ~ OrderStatus::OS_NEW_REJECT)|OrderStatus::OS_NEW_REJECT);
		setNote(newRjt->note());
		setPerfNote(perfNote() + "|" + newRjt->perfNote());
	}

	void Order::processOrderActionCancel(OrderActionCancel* actionCxl)
	{
        setStatus((status() & ~OrderStatus::OS_CANCEL_REJECT) | OrderStatus::OS_CANCEL_REQUEST);
		setLastCancelSinceEpoch(DateTime::now().toUTCMillisecsSinceEpoch());
		setPerfNote(perfNote() + "|" + actionCxl->perfNote());
	}

	void Order::processOrderReportCancelDone(OrderReportCancelDone* cancelDone)
	{
        setStatus((status() & ~OrderStatus::OS_CANCEL_REJECT) | OrderStatus::OS_CANCEL_DONE);
		const QString& note = cancelDone->note();
		if (!note.isEmpty()) {
			setNote(note);
		}
		setPerfNote(perfNote() + "|" + cancelDone->perfNote());
	}

	void Order::processOrderReportCancelReject(OrderReportCancelReject* cancelRjt)
	{
        setStatus(status()|(OrderStatus::OS_CANCEL_REJECT &~OrderStatus::OS_CANCEL_DONE));
		setNote(cancelRjt->note());
	}




	void Order::processOrderReportFill(OrderReportFill* newfill, OrderReportFill* lastFillrpt)
	{
		double lastFillVolume = 0; //TODO check double
		double lastFillAmount = 0.0;
		if (lastFillrpt) {
			lastFillVolume = lastFillrpt->fillVolume();
			lastFillAmount = lastFillrpt->fillAmount();
		}
		setFillVolume(fillVolume() + newfill->fillVolume() - lastFillVolume);
		setFillAmount(fillAmount() + newfill->fillAmount() - lastFillAmount);
		if (std::abs(fillVolume()) >= std::abs(volume())) {
			setStatus(status() | OrderStatus::OS_ORDER_DONE);
		}
		if (!newfill->note().isEmpty()) {
			setNote(newfill->note());
		}
	}

	mts::Order* Order::clone() const
	{
		return new Order(*this);
	}

	void Order::copyTo(OrderReport* rpt) const
	{
		rpt->setTradingAccountId(tradingAccountId());
		rpt->setInstrumentId(instrumentId());
		rpt->setBook(book());
		rpt->setTradingDay(tradingDay());

		rpt->setReferenceId(referenceId());
		rpt->setVolume(volume());
		rpt->setPrice(price());
		rpt->setFillVolume(fillVolume());
		rpt->setFillAmount(fillAmount());
		rpt->setPriceType(priceType());
		rpt->setOffsetFlag(offsetFlag());
		rpt->setDirectonSide(directionSide());
		rpt->setTimeCondition(timeCondition());
		rpt->setCreateSrc(createSrc());
		rpt->setOrderExchId(orderExchId());
		rpt->setNote(note());
		rpt->setStrategyId(strategyId());
		rpt->setInstanceId(instanceId());
	}
	void Order::copyTo(Order* order) const
	{		
		copyTo((OrderReport*)order);
		order->setLastCancelSinceEpoch(order->lastCancelSinceEpoch());
		order->setStatus(status());
		order->setCreateTicksSinceEpoch(createTicksSinceEpoch());
	}

	void Order::copyTo(OrderActionCancel* cxl) const
	{
		cxl->setTradingAccountId(tradingAccountId());
		cxl->setInstrumentId(instrumentId());
		cxl->setBook(book());
		cxl->setTradingDay(tradingDay());
		cxl->setReferenceId(referenceId());
		cxl->setVolume(volume());
		cxl->setPrice(price());
		cxl->setFillVolume(fillVolume());
		cxl->setFillAmount(fillAmount());
		cxl->setPriceType(priceType());
		cxl->setOffsetFlag(offsetFlag());
		cxl->setDirectonSide(directionSide());
		cxl->setTimeCondition(timeCondition());
		cxl->setCreateSrc(createSrc());
		cxl->setStrategyId(strategyId());
		cxl->setInstanceId(instanceId());
	}

	QString Order::statusString() const
	{
		if (_status&OS_CANCEL_DONE) {
			if (fillVolume() != 0) {
				return "CancelFill";
			}
			return "Canceled";
		}
		if (_status&OS_ALL_FILL) {
			return "Filled";
		}
		if (_status&OS_NEW_DONE) {
			if (fillVolume() != 0) {
				return "PartailFill";
			}
			return "NewDone";
		}
		if (_status&OS_NEW_REJECT) {
			return "NewReject";
		}
		if (_status&OS_CANCEL_REJECT) {
			return "CancelReject";
		}
		return "Pending";
	}
	void Order::registerMetaType() {
		qRegisterMetaType<Order>("Order");
		qRegisterMetaType<OrderPtr>("OrderPtr");
	}

	void Order::setLastCancelSinceEpoch(qint64 ts)
	{
		_lastCxlTimestamp = ts;
	}

	qint64 Order::lastCancelSinceEpoch() const
	{
		return _lastCxlTimestamp;
	}
	
	QJsonObject& Order::toJson(QJsonObject& jsonObj) const
	{
		OrderReportNewDone::toJson(jsonObj);
		jsonObj.insert("status", _status);
		jsonObj.insert("statusString", statusString());
		return jsonObj;
	}

	void Order::setNotifyToStrategy(bool v)
	{
		this->_notifyToStrategy = v;
	}

	bool Order::needNotifyToStrategy() const {
		return _notifyToStrategy;
	}

}
