
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
#include <QtCore/QString>
#include <boost/shared_ptr.hpp>
#include "mts_core/mts_core_api.h"
#include "mts_core/OrderType.h"

namespace mts
{

	class MTS_CORE_API OrderId
	{
	public:
		OrderId();
		OrderId(const OrderId&);
		explicit OrderId(const QString&); //any string
		
		~OrderId();

		bool isValid() const;
		bool isMtsOrder() const;

		QString toString() const;
		bool fromString(const QString& idStr); //should be mts order string

		bool operator==(const OrderId& id) const;
		bool operator>(const OrderId& id) const;
		bool operator<(const OrderId& id) const;
		uint hashCode() const;
	private:
		QString _id;
	};
    //inline
	MTS_CORE_API uint qHash(const mts::OrderId& id);
}


