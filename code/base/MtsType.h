
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
	static bool isDoubleMax(double v);
	static bool isValidPrice(double v);
	static bool isZero(double);
	static QString toReadableString(double v,int prec=10);
	static QString toString(double v, int prec);
	static bool isEqual(double v1, double v2);
	static bool isGreaterThan(double v1, double v2);
	static bool isLessThan(double v1, double v2);
	static bool isGreaterThanOrEqual(double v1, double v2);
	static bool isLessThanOrEqual(double v1, double v2);
	static double floor(double val,int precision);
	static double ceil(double val, int precision);
	static int getPrecision(double val, int precision);
};



#define INSTRUMENT_ID_ENUM(declare)	\
	declare(TYPE_UNKNOWN,"UNKNOWN","无效标的类型")	\
	declare(TYPE_STK_SPOT,"STK","股票")	\
	declare(TYPE_FUTR,"FUT","期货") \
	declare(TYPE_OPTION,"OPT","期权") \
	declare(TYPE_HUOBI,"HUOBI","火币现货") \
	declare(TYPE_BITMEX,"BITMEX","BitMEX") \
	declare(TYPE_OKEX,"OKEX","OKEX")

enum InstrumentType { INSTRUMENT_ID_ENUM(SELECT_1_AND_COMMA_IN_3) INSTRUMENT_TYPE_MAX };

BASE_API const char* instrumentTypeName(InstrumentType);
BASE_API InstrumentType instrumentType(const char*);
BASE_API const char* instrumentTypeDescription(InstrumentType);

BASE_API int prec(InstrumentType);
}
