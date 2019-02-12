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

/*****************************************************************************
* Copyright [2018] [3fellows]
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

#ifndef __Currency_H__
#define __Currency_H__
#pragma once

#include "base_api.h"
#include "enum_ext.h"


#define CURRENCY_ID_ENUM(declare) \
	declare(CURRENCY_UNKNOWN , "UNKNOWN", "��Ч����") \
	declare(CURRENCY_CNY , "CNY", "�����") \
	declare(CURRENCY_USD , "USD", "��Ԫ")	\
	declare(CURRENCY_HKD , "HKD", "��Ԫ")	\
	declare(CURRENCY_EUR , "EUR", "ŷԪ")	\
	declare(CURRENCY_GBP , "GBP", "Ӣ��") 

enum CurrencyId { CURRENCY_ID_ENUM(SELECT_1_AND_COMMA_IN_3) CURRENCY_MAX };

BASE_API const char*  currencyIdName(CurrencyId currency);
BASE_API CurrencyId currencyId(const char* currencyName);



#endif
