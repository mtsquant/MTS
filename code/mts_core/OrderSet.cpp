
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
#include "OrderSet.h"
#include "base/MtsLog.h"
#include "mts_core/Order.h"
#include "mts_core/Environment.h"

namespace mts
{
	QString orderExchIdString(const QString& id, ExchId exch) {
		return QString("%1@%2").arg(id).arg(exch);
	}

	OrderSet* OrderSet::_instance = nullptr;
	OrderSet::OrderSet()
	{
		//MTS_LOG("%p\n", this);
		//MTS_FILE("%p\n", this);
	}

	OrderSet::~OrderSet() {
		clear();
	}

	OrderSet * OrderSet::instance() {
		if (_instance == nullptr) {
			_instance = new OrderSet;
		}
		return _instance;
	}

	Order* OrderSet::getOrder(const OrderId& referentId, bool createIfNotExist)
	{
		auto it = _orders.find(referentId);
		if (it != _orders.cend())
		{
			return it.value();
		}
		if (createIfNotExist) {
			Order* ord=new Order;
			ord->setReferenceId(referentId);
			_orders.insert(referentId, ord);
			return ord;
		}
		return nullptr;
	}
	
	Order* OrderSet::getOrder(const QString& exchOrderId, ExchId exch)
	{
		auto it = _exchOrders.find(orderExchIdString(exchOrderId,exch));
		if (it != _exchOrders.cend())
		{
			return it.value();
		}
		return nullptr;
	}

	QList<Order*> OrderSet::allOrders() const
	{
		return _orders.values();
	}

	QList<Order*> OrderSet::allActiveOrders(const InstrumentId& id) const
	{
		auto it = _activeOrders.constFind(id);
		if (it != _activeOrders.cend()) {
			return it.value().values();
		}
		return QList<Order*>();
	}

	void OrderSet::updateOrder(Order* order, OrderReportType type)
	{
		if (type == OrderReportType::ORT_ORDER) {
			auto it = _orders.find(order->referenceId());
			if (it == _orders.end()) {
				_orders.insert(order->referenceId(), order);
			}
		}
		if (!order->orderExchId().isEmpty()) {
			_exchOrders.insert(orderExchIdString(order->orderExchId(),order->instrumentId().exchId), order);
		}
		if (order->isFinished()) {
			auto& map = _activeOrders[order->instrumentId()];
			auto it=map.find(order->referenceId());
			if (it != map.end()) {
				map.erase(it);
			}
		}
		else {
			if ( Environment::instance ()->isCurrentInstanceInstanceId ( order->instanceId() ) ){
				_activeOrders[order->instrumentId ()].insert ( order->referenceId(),order );
			}
		}
	}

	void OrderSet::clear()
	{
		qDeleteAll(_orders);
		_orders.clear();
		_exchOrders.clear();
		_activeOrders.clear();
	}

}