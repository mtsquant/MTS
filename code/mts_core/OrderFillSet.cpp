
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
#include "OrderFillSet.h"

namespace mts
{
	OrderFillSet::OrderFillSet()
	{
	}


	OrderFillSet::~OrderFillSet()
	{
		clear();
	}
	
	QList<OrderReportFill*> OrderFillSet::allFills() const
	{
		return _fillrpts.values();
	}

	QString fillidString(const QString& id, DirectionSide side, ExchId exch) {
		return QString("%1@%2@%3").arg(id).arg(side).arg(exch);
	}

	OrderReportFill* OrderFillSet::getFill(const QString& fillId, DirectionSide side, ExchId exch) const
	{
		QString key = fillidString(fillId,side,exch);
		auto it = _fillrpts.constFind(key);
		if (it != _fillrpts.cend()) {
			return it.value();
		}
		return nullptr;
	}

	void OrderFillSet::updateFill(OrderReportFill* fill)
	{
		QString key= fillidString(fill->fillId(),fill->directionSide(),fill->instrumentId().exchId);
		OrderReportFill* lastFillrpt =_fillrpts[key];
		if (lastFillrpt != nullptr) {
			lastFillrpt->setFillAmount(fill->fillAmount());
			lastFillrpt->setFillVolume(fill->fillVolume());
			lastFillrpt->setFillTicksSinceEpoch(fill->fillTicksSinceEpoch());
		}
		else {
			_fillrpts[key] = new OrderReportFill(*fill);
		}
	}

	void OrderFillSet::clear()
	{
		qDeleteAll(_fillrpts);
		_fillrpts.clear();
	}

}