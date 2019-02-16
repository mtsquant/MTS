
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
#include "OrderFillMgr.h"

namespace mts
{
	OrderFillMgr* OrderFillMgr::_instance = nullptr;
	OrderFillMgr::OrderFillMgr()
	{
	}


	OrderFillMgr::~OrderFillMgr()
	{
	}

	OrderFillMgr* OrderFillMgr::instance()
	{
		if (_instance == nullptr) {
			_instance = new OrderFillMgr;
		}
		return _instance;
	}

	QList<OrderReportFill*> OrderFillMgr::allFills() const
	{
		return _fillSet.allFills();
	}
	
	OrderReportFill* OrderFillMgr::getFill(const QString& fillId, DirectionSide side, ExchId exch) const
	{
		return _fillSet.getFill(fillId, side, exch);
	}

	void OrderFillMgr::updateFill(OrderReportFill* fill)
	{
		_fillSet.updateFill(fill);
	}
}