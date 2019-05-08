
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
#include "OrderType.h"
#include <QtCore/QHash>
#include "base/enum_ext.h"



/*
#define ORDER_TYPE_MODE_ENUM(declare)	\
declare(OT_UNKNOWN,"UNKNOWN","无效报单类型")	\
declare(OT_DIRECT,"direct","直接发单")	\
declare(OT_SMART,"smart","开平仓自转")\
declare(OT_SMARTER,"smarter","开平自转可拆分")

enum OrderType { ORDER_TYPE_MODE_ENUM(SELECT_1_AND_COMMA_IN_3) OT_NAX };

const char* orderTypeName(OrderType type) {
	static const char* names[] = {
		ORDER_TYPE_MODE_ENUM(SELECT_2_AND_COMMA_IN_3)
	};
	if (type >= 0 && type < ARRAY_SIZE(names)) {
		return names[type];
	}
	return names[0];
}

OrderType orderType(const char* typeName) {
	static QHash<QString, OrderType> map;
	if (map.isEmpty()) {
		ORDER_TYPE_MODE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
	}
	return map[typeName];
}

const char* orderTypeDescription(OrderType type) {
	static const char* names[] = {
		ORDER_TYPE_MODE_ENUM(SELECT_3_AND_COMMA_IN_3)
	};
	if (type >= 0 && type < ARRAY_SIZE(names)) {
		return names[type];
	} else {
		return names[0];
	}
}
*/

enum
{
	MAX_INSTANCE_ID = 999,
	MAX_STRATEGY_ID = 9,
	MAX_ORDER_TYPE = 9
};


bool MTS_CORE_API mts::isInstanceIdValid(int instanceId) {
	return instanceId > 0 && instanceId <= MAX_INSTANCE_ID;
}

bool MTS_CORE_API mts::isStrategyIdValid(int strategyId) {
	return strategyId > 0 && strategyId <= MAX_STRATEGY_ID;
}
