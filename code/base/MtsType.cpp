
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
#include "MtsType.h"
#include <QtCore/QHash>



namespace mts
{
	bool Double::isZero(double v) {
		return isEqual(v,0);
	}

	QString Double::toString(double v){
		if(!isValidPrice(v)){
			return "-";
		} else{
			return QString::number(v , 'f' , 2);
		}
	}

	const double EPSINON = 0.00000001;

	bool Double::isEqual(double v1 , double v2){
		return abs(v1 - v2) < EPSINON;
	}

	bool Double::isGreaterThan(double v1, double v2) {
		return (v1-v2)>EPSINON;
	}

	bool Double::isLessThan(double v1, double v2) {
		return (v2-v1)<-EPSINON;
	}

	bool Double::isGreaterThanOrEqual(double v1, double v2) {
		return (v1 - v2) > -EPSINON;
	}

	bool Double::isLessThanOrEqual(double v1, double v2) {
		return (v2 - v1) < EPSINON;
	}




	const char * instrumentTypeName(InstrumentType type) {
		static const char* names[] = {
			INSTRUMENT_ID_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		} else {
			return names[0];
		}
	}

	InstrumentType instrumentType(const char * typeName) {
		static QHash<QString, InstrumentType> map;
		if (map.isEmpty()) {
			INSTRUMENT_ID_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char * instrumentTypeDescription(InstrumentType type) {
		static const char* names[] = {
			INSTRUMENT_ID_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		} else {
			return names[0];
		}
	}

}