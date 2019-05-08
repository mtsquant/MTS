
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
#include "./Currency.h"
#include <QtCore/QHash>
#include <QtCore/QString>

const char * currencyIdName(CurrencyId currency) {
	static const char* names[] = {
		CURRENCY_ID_ENUM(SELECT_2_AND_COMMA_IN_3)
	};
	if (currency >= 0 && currency < ARRAY_SIZE(names)) {
		return names[currency];
	} else {
		return names[0];
	}
}

CurrencyId currencyId(const char * currencyName) {
	static QHash<QString, CurrencyId> map;
	if (map.isEmpty()) {
		CURRENCY_ID_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
	}
	return map[currencyName];
	return CurrencyId();
}

