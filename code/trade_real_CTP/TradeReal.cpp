
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

		UrlPath tradeFront = ConfigParams::instance()->tradeFront("CTP");
		QString userName = tradeFront.userName();
		QString password = tradeFront.password();
		MTS_DEBUG ( "Login Trade as '%s'\n" , qPrintable(userName) );
		_tradeOrderClient = new CTPOrderMgr(tradeFront.clearUserPassword(),
																ConfigParams::instance()->brokerId("CTP"),
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
        MTS_DEBUG("TradeReal::doNewOrder - %s\n", qPrintable(newAction->referenceId()));
		return _tradeOrderClient->newOrder(newAction);
	}

	bool TradeReal::sendOrderCancelAction(OrderActionCancel * cancelAction) {
        MTS_DEBUG("TradeReal::doCancelOrder - %s\n", qPrintable(cancelAction->referenceId()));
		return _tradeOrderClient->cancelOrder(cancelAction);
	}

	bool TradeReal::sendOrderOtherAction(OrderAction *) {
		return false;
	}

	QString TradeReal::createOrderId(int instanceId, int strategyId, int orderType, int directionSide, int priceType, const InstrumentId& instrumentId) {
		static int index = 0;
		OrderIdImplCtp orderIdCtp(_tradeOrderClient->frontId(), _tradeOrderClient->sessionId(), instanceId, strategyId, orderType,++index);
		assert(orderIdCtp.isMtsOrder());
		return  orderIdCtp.toString();
	}

	/*OrderId TradeReal::parseOrderId(const QString &idStr) {

		OrderIdImplCtp orderIdCtp;
		if (orderIdCtp.fromString(idStr) && orderIdCtp.isMtsOrder()) {
			return OrderId(idStr);
		}
		return OrderId();
	}*/

	bool TradeReal::isMtsOrder(const QString & idStr) {
		OrderIdImplCtp orderIdCtp;
		if (orderIdCtp.fromString( idStr ) && orderIdCtp.isMtsOrder()) {
			return true;
		}
		return false;
	}








}
