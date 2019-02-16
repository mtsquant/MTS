
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
#pragma once
#include <QtCore/QHash>
#include <QtCore/QObject>
#include "mts_core/OrderSet.h"
#include "mts_core/StrategyInterface.h"
//#include "mts_api.h"
//#include "../mts_core/DateCallback.h"

namespace mts {
	class Trade;
	class Order;
	class PositionPersistence;

	//all functions will be called in the main thread, so the class does not need any mutex
	class OrderMgrBase :public QObject, public TradeCallback
	{
		Q_OBJECT
	public:
		//void setTrade(Trade* trader);
		void setStrategy(StrategyInterface* notify);
	public Q_SLOTS:
		QList<Order*> allOrders()const;
		QList<Order*> allActiveOrders(const InstrumentId&)const;
		Order* getOrder(const mts::OrderId& referenceId);
		Order* getOrder(const QString& exchId, ExchId exch);

		mts::OrderId sendOrder(OrderActionNew*, int orderType = OT_DIRECT); //return refrenceId if success, return -1 if failed
		bool cancelOrder(OrderActionCancel*);

		void initStrategyPos(int strategyId, const QString& dir);

		virtual void onOrderNewDone(OrderReportNewDone*) override;
		virtual void onOrderNewReject(OrderReportNewReject*) override;
		virtual void onOrderFill(OrderReportFill*) override;
		virtual void onOrderCancelDone(OrderReportCancelDone*) override;
		virtual void onOrderCancelReject(OrderReportCancelReject*) override;
		virtual void onOrderUpdate(Order*) override;
		virtual void onPositionUpdate(Position*) override;
		virtual void onTradingAccountUpdate(TradingAccount*) override;
		virtual void onOrderOtherReport(OrderReport*) override {}
		virtual void onBusinessDateChanged(int businessDate) override;
	protected:
		OrderMgrBase();
		virtual ~OrderMgrBase();
	private:
		OrderMgrBase& operator = (const OrderMgrBase&);
		OrderMgrBase(const OrderMgrBase&);
		void doProcessOrder(Order* current, Order* last, Position* pos = nullptr);
		bool doSendOrder(OrderActionNew*, Position*);
		bool checkStrategyPos(OrderActionNew*);
		void doPositionUpdate(Position*, int instanceId, int strategyId = DEFAULT_STRATEGY_ID);
		void doPersistPos(Position* pos, int instanceId, int strategyId = DEFAULT_STRATEGY_ID);
		Position* getPosition(const InstrumentId&);
		Position* getPosition(const InstrumentId&, int instanceId, int strategyId);

		virtual bool doCheckRisk(OrderActionNew* newaction, Position* pos);

		StrategyInterface* _notify;
		//Trade* _trader;
		QMap<QString, PositionPersistence*> _persistence;


	};
}
