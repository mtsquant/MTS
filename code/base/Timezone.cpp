
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
#include "Timezone.h"
#include <QtCore/QHash>
#include <QtCore/QStringList>




const char * timezoneShortName(TimezoneCode timezone) {
	static const char* names[] = {
		TIMEZONE_CODE_ENUM(SELECT_2_AND_COMMA_IN_4)
	};
	if (timezone >= 0 && timezone < ARRAY_SIZE(names)) {
		return names[timezone];
	} else {
		return names[0];
	}
}

const char * timezoneLongName(TimezoneCode timezone) {
	static const char* names[] = {
		TIMEZONE_CODE_ENUM(SELECT_3_AND_COMMA_IN_4)
	};
	if (timezone >= 0 && timezone < ARRAY_SIZE(names)) {
		return names[timezone];
	} else {
		return names[0];
	}
}

const char * timezoneSessionName(TimezoneCode timezone) {
	static const char* names[] = {
		TIMEZONE_CODE_ENUM(SELECT_4_AND_COMMA_IN_4)
	};
	if (timezone >= 0 && timezone < ARRAY_SIZE(names)) {
		return names[timezone];
	} else {
		return names[0];
	}
}


TimezoneCode timezoneCodeFromShortName(const char * timezoneShortName) {
	static QHash<QString, TimezoneCode> map;
	if (map.isEmpty()) {
		TIMEZONE_CODE_ENUM(DEFINE_NAME_ENUM_MAP4_KEY_2);
	}
	return map[timezoneShortName];
}

TimezoneCode timezoneCodeFromSessiontName(const char * timezoneSessionName) {
	static QHash<QString, TimezoneCode> map;
	if (map.isEmpty()) {
		TIMEZONE_CODE_ENUM(DEFINE_NAME_ENUM_MAP4_KEY_4);
	}
	return map[timezoneSessionName];
}

//case US:
	//	return "America/New_York";
	//case UK:
	//	return "Europe/London";
	//case HK:
	//	return "Asia/Hong_Kong";
	//case CN:
	//	return "Asia/Shanghai";
	//case SG:
	//	return "Asia/Singapore";
	//case JP:
	//	return "Asia/Tokyo";
	//case KR:
	//	return "Asia/Seoul";
	//case FT:
	//	return "America/Chicago";
	//case DE:
	//	return "Europe/Berlin";
	//case IT:
	//	return "Europe/Rome";
	//case FR:
	//	return "Europe/Paris";
	//case SP:
	//	return "Europe/Madrid";
	//case BE:
	//	return "Europe/Brussels";
	//	break;
	//case NL:
	//	return "Europe/Amsterdam";
	//	break;
	//case AU:
	//	return "Australia/Sydney";
	//	break;
	//case IS:
	//	return "Atlantic/Reykjavik";

