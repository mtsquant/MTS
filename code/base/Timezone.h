
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
#ifndef __FeCountry_H__
#define __FeCountry_H__
#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include "Currency.h"
#include "base_api.h"


#define TIMEZONE_CODE_ENUM(declare) \
	declare(TIMEZONE_UNKNOWN , "UNKNOWN", "无效时区","UNKNOWN") \
	declare(TIMEZONE_CST , "CST", "中国标准时间","Asia/Shanghai") \
	declare(TIMEZONE_EST , "EST", "美国东部时间", "America/New_York")	\
	declare(TIMEZONE_UTC , "UTC", "格林威治时间", "Atlantic/Reykjavik")	

enum TimezoneCode { TIMEZONE_CODE_ENUM(SELECT_1_AND_COMMA_IN_4) COUNTRY_MAX };

BASE_API const char*  timezoneShortName(TimezoneCode);
BASE_API const char*  timezoneLongName(TimezoneCode);
BASE_API const char*  timezoneSessionName(TimezoneCode);
BASE_API TimezoneCode timezoneCodeFromShortName(const char* timezoneShortName);
BASE_API TimezoneCode timezoneCodeFromSessiontName(const char* timezoneSessionName);

#endif
