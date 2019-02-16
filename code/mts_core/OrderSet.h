
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
#include "mts_core/OrderId.h"
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <set>
#include "mts_core/OrderReport.h"
#include "mts_core/StrategyInterface.h"

namespace mts {
	class Trade;
	class Order;

	class MTS_CORE_API OrderSet
	{
	public:
		static OrderSet* instance();
		Order* getOrder(const OrderId& referentId, bool createIfNotExist = false);
		Order* getOrder(const QString& exchId, ExchId exch);
		QList<Order*> allOrders() const;
		QList<Order*> allActiveOrders(const InstrumentId&) const;
		void updateOrder(Order*, OrderReportType);
		void clear();

	protected:
		OrderSet();
		virtual ~OrderSet();
	private:
		QHash<OrderId, Order*> _orders;
		QHash<QString, Order*> _exchOrders;
		QHash<InstrumentId, QMap<OrderId,Order*> > _activeOrders;
		static OrderSet* _instance;
	};
}
