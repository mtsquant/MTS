/*****************************************************************************
* Copyright [2018-2019] [3fellows]
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
#include "TradeReal.h"
#include "CTPOrderMgr.h"
#include "mts_core/ConfigParams.h"
#include "OrderIdImplCtp.h"
#include "base/EventType.h"
#include "TradeEvent.h"

namespace mts
{

	TradeReal::TradeReal()
		:Trade(), _tradeOrderClient(nullptr)
	{
	}


	TradeReal::~TradeReal() 
	{
	}

	bool TradeReal::initialize(const QVariantMap & params) {
		assert(ConfigParams::isInitialized());

		UrlPath tradeFront = ConfigParams::instance()->tradeFront();
		QString userName = tradeFront.userName();
		QString password = tradeFront.password();
		MTS_DEBUG ( "Login Trade as '%s'\n" , qPrintable(userName) );
		_tradeOrderClient = new CTPOrderMgr(tradeFront.clearUserPassword(),
																ConfigParams::instance()->brokerId(),
																userName,
																password);
		connect(_tradeOrderClient, SIGNAL(loginDone()), this, SLOT(emitInitializeDone()));
		connect(_tradeOrderClient, SIGNAL(loginError(const QString&)), this, SLOT(emitInitializeError(const QString&)));

		connect(_tradeOrderClient, SIGNAL(signalTradingAccount(mts::TradingAccount*)), this, SLOT(notifyAll(mts::TradingAccount*)));

		connect(_tradeOrderClient, SIGNAL(signalOrderReport(mts::OrderReportNewDone*)), this, SLOT(notifyAll(mts::OrderReportNewDone*)));
		connect(_tradeOrderClient, SIGNAL(signalOrderReport(mts::OrderReportNewReject*)), this, SLOT(notifyAll(mts::OrderReportNewReject*)));
		connect(_tradeOrderClient, SIGNAL(signalOrderReport(mts::OrderReportFill*)), this, SLOT(notifyAll(mts::OrderReportFill*)));
		connect(_tradeOrderClient, SIGNAL(signalOrderReport(mts::OrderReportCancelDone*)), this, SLOT(notifyAll(mts::OrderReportCancelDone*)));
		connect(_tradeOrderClient, SIGNAL(signalOrderReport(mts::OrderReportCancelReject*)), this, SLOT(notifyAll(mts::OrderReportCancelReject*)));
		
		connect(_tradeOrderClient, SIGNAL(signalRestoreOrder(mts::Order*)), this, SLOT(notifyAll(mts::Order*)));
		connect(_tradeOrderClient, SIGNAL(signalRestorePosition(mts::Position*)), this, SLOT(notifyAll(mts::Position*)));


		bool ok = _tradeOrderClient->connectFront();
		if (!ok) {
			return false;
		}

		return true;
	}

	bool TradeReal::sendOrderNewAction(OrderActionNew * newAction) {
        MTS_DEBUG("TradeReal::doNewOrder - %s\n", qPrintable(newAction->referenceId().toString()));
		return _tradeOrderClient->newOrder(newAction);
	}

	bool TradeReal::sendOrderCancelAction(OrderActionCancel * cancelAction) {
        MTS_DEBUG("TradeReal::doCancelOrder - %s\n", qPrintable(cancelAction->referenceId().toString()));
		return _tradeOrderClient->cancelOrder(cancelAction);
	}

	bool TradeReal::sendOrderOtherAction(OrderAction *) {
		return false;
	}

	OrderId TradeReal::createOrderId(int instanceId, int strategyId, int orderType, const InstrumentId& instrumentId) {
		static int index = 0;
		OrderIdImplCtp orderIdCtp(_tradeOrderClient->frontId(), _tradeOrderClient->sessionId(), instanceId, strategyId, orderType,++index);
		assert(orderIdCtp.isMtsOrder());
		return  OrderId(orderIdCtp.toString());
	}

	OrderId TradeReal::parseOrderId(const QString &idStr) {

		OrderIdImplCtp orderIdCtp;
		if (orderIdCtp.fromString(idStr) && orderIdCtp.isMtsOrder()) {
			return OrderId(idStr);
		}
		return OrderId();
	}

	bool TradeReal::isMtsOrder(const QString & idStr) {
		OrderIdImplCtp orderIdCtp;
		if (orderIdCtp.fromString( idStr ) && orderIdCtp.isMtsOrder()) {
			return true;
		}
		return false;
	}

	//void TradeReal::onEvent(const EventPtr & event) {
	//	int evtType = event->type();
	//	if (evtType == EVT_QUOTE) {
	//		return;
	//	}

	//	if (evtType == EVT_RPT_ORDER_NEW_REJECT) {
	//		auto evt = qSharedPointerDynamicCast<OrderNewRejectEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//	if (evtType == EVT_RPT_ORDER_FILL) {
	//		auto evt = qSharedPointerDynamicCast<OrderFillEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}

	//	if (evtType == EVT_RPT_ORDER_NEW_DONE) {
	//		auto evt = qSharedPointerDynamicCast<OrderNewDoneEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//	if (evtType == EVT_RPT_ORDER_NEW_REJECT) {
	//		auto evt = qSharedPointerDynamicCast<OrderNewRejectEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//	if (evtType == EVT_RPT_ORDER_FILL) {
	//		auto evt = qSharedPointerDynamicCast<OrderFillEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//	if (evtType == EVT_RPT_ORDER_CANCEL_DONE) {
	//		auto evt = qSharedPointerDynamicCast<OrderCancelDoneEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//	if (evtType == EVT_RPT_ORDER_CANCEL_REJECT) {
	//		auto evt = qSharedPointerDynamicCast<OrderCancelRejectEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}


	//	if (evtType == EVT_ORDER) {
	//		auto evt = qSharedPointerDynamicCast<OrderEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}

	//	if (evtType == EVT_POSITION) {
	//		auto evt = qSharedPointerDynamicCast<PositionEvent>(event);
	//		this->notifyAll(evt->data());
	//		delete evt->data();
	//		return;
	//	}
	//}


}
