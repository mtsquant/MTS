
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
#pragma once
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
		OrderSet();
		virtual ~OrderSet();

		Order* getOrder(const QString& referentId, bool createIfNotExist = false);
		Order* getOrder(const QString& exchId, ExchId exch);
		QList<Order*> allOrders() const;
		QList<Order*> allActiveOrders(const InstrumentId&) const;
		void updateOrder(Order*, OrderReportType);
		void clear();

	private:
		QHash<QString, Order*> _orders;
		QHash<QString, Order*> _exchOrders;
		QHash<InstrumentId, QMap<QString,Order*> > _activeOrders;
	};


	class MTS_CORE_API OrderSetSingleton :public OrderSet
	{
	public:
		static OrderSet* instance();
	private:
		OrderSetSingleton();
		~OrderSetSingleton();
	};
}
