
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
#include "base/base_api.h"
#include "base/enum_ext.h"
#ifndef _WIN32
#include <values.h>
#endif

namespace mts
{
class BASE_API Double
{
public:
	static bool isValidPrice(double v){
		return !isEqual(v, DBL_MAX) && isGreaterThan(v,0);
	}
	static bool isZero(double);
	static QString toString(double v);
	static bool isEqual(double v1, double v2);
	static bool isGreaterThan(double v1, double v2);
	static bool isLessThan(double v1, double v2);
	static bool isGreaterThanOrEqual(double v1, double v2);
	static bool isLessThanOrEqual(double v1, double v2);

};



//TODO: 
//declare(TYPE_DC_SPOT,"DCS","加密数字货币") 
//declare(TYPE_DC_FUTR,"DCF","加密数字货币期货") 
#define INSTRUMENT_ID_ENUM(declare)	\
	declare(TYPE_UNKNOWN,"UNKNOWN","无效标的类型")	\
	declare(TYPE_STOCK,"STK","股票")	\
	declare(TYPE_FUTR,"FUT","期货") \
	declare(TYPE_OPTION,"OPT","期权") \
	declare(TYPE_HUOBI,"HUOBI","火币") \
	declare(TYPE_BITMEX,"BITMEX","BitMEX")

enum InstrumentType { INSTRUMENT_ID_ENUM(SELECT_1_AND_COMMA_IN_3) INSTRUMENT_TYPE_MAX };

BASE_API const char* instrumentTypeName(InstrumentType);
BASE_API InstrumentType instrumentType(const char*);
BASE_API const char* instrumentTypeDescription(InstrumentType);

}
