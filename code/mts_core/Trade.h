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
#include <QtCore/QVariant>
#include "Component.h"
#include "base/NotifiableObserver.h"
#include "mts_core/OrderAction.h"
#include "mts_core/OrderReport.h"
#include "mts_core/Order.h"
#include "TradeCallback.h"
#include "mts_core/OrderId.h"

namespace mts
{

	class TradeNotifier;
	//ͳһ���׽ӿ�
	class MTS_CORE_API TradeBase:public Component
	{
	public:
		TradeBase();
		virtual ~TradeBase();
		virtual OrderId createOrderId(int instanceId, int strategyId, int orderType, const InstrumentId& instrumentId) = 0;
		virtual OrderId parseOrderId(const QString& idStr) = 0;
		virtual bool isMtsOrder(const QString& idStr) = 0;

		virtual int addCallback(TradeCallback*)=0;
		//virtual bool removeCallback(TradeCallback*)=0;

		virtual bool sendOrderNewAction(OrderActionNew*) = 0;
		virtual bool sendOrderCancelAction(OrderActionCancel*) = 0;

		virtual bool sendOrderOtherAction(OrderAction*) = 0;
	};


	class MTS_CORE_API Trade : public TradeBase
	{
		Q_OBJECT
	public:
		Trade();
		virtual ~Trade();

		virtual int addCallback(TradeCallback*) override;
		//virtual bool removeCallback(TradeCallback*) override;

	protected Q_SLOTS:
		void notifyAll(mts::OrderReportNewDone*);
		void notifyAll(mts::OrderReportNewReject*);
		void notifyAll(mts::OrderReportFill*);
		void notifyAll(mts::OrderReportCancelDone*);
		void notifyAll(mts::OrderReportCancelReject*);
		void notifyAll(mts::Order*);
		void notifyAll(mts::Position*);
		void notifyAll(mts::TradingAccount*);
	private:
		QList<TradeCallback*> _callbacks;

	};

}
