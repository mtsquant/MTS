
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
#include "OrderId.h"
#include "mts_core/Environment.h"

namespace mts
{
	OrderId::OrderId()
	{
	}

	OrderId::OrderId(const OrderId & id) 
		:_id(id._id)
	{
	}

	OrderId::OrderId(const QString & idStr) 
		:_id(idStr)
	{
	}


	OrderId::~OrderId()
	{

	}

	bool OrderId::isValid() const {
		return !_id.isEmpty();
	}

	bool OrderId::isMtsOrder() const {
		return Environment::instance()->trade()->isMtsOrder(_id);
	}

	QString OrderId::toString() const {
		return _id;
	}

	/*bool OrderId::fromString(const QString & idStr) {
		_id=Environment::instance()->trade()->parseOrderId(idStr)._id;
		return isValid();
	}*/

	bool OrderId::operator==(const OrderId& id) const {
		return _id == id._id;
	}

	bool OrderId::operator>(const OrderId& id) const {
		return _id > id._id;
	}

	bool OrderId::operator<(const OrderId& id) const {
		return _id < id._id;
	}

	uint OrderId::hashCode() const {
		return qHash(_id);
    }

    uint qHash(const OrderId &id) {
        return id.hashCode();
    }

}
