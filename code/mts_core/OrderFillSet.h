
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
#include "mts_core/OrderReport.h"

namespace mts {
	class MTS_CORE_API OrderFillSet
	{
	public:
		OrderFillSet();
		~OrderFillSet();
		QList<OrderReportFill*> allFills()const;
		OrderReportFill* getFill(const QString& fillId, DirectionSide side, ExchId exch) const;
		void updateFill(OrderReportFill*);
		void clear();
	private:
		QHash<QString, OrderReportFill*> _fillrpts;
	};
}
