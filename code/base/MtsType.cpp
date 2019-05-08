
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
#include "MtsType.h"
#include <QtCore/QHash>
#include <QtCore/QtMath>
#include "base/number.h"



namespace mts
{
	bool Double::isDoubleMax(double v)
	{
		return isEqual(v, DBL_MAX);
	}
	inline bool Double::isValidPrice(double v) {
		return !isDoubleMax(v) && isGreaterThan(v, 0);
	}

	bool Double::isZero(double v) {
		return isEqual(v,0);
	}

	QString Double::toReadableString(double v,int prec){
		if(isDoubleMax(v)){
			return "-";
		} else{
			return Double::toString(v,prec);
		}
	}

	QString Double::toString(double v, int prec)
	{
		QString str = QString::number(v, 'f', prec);
		if (!str.contains('.')) {
			return str;
		}
		while (str.endsWith('0')) {
			str.chop(1);
		}
		if (str.endsWith('.')) {
			str.chop(1);
		}
		return str;
	}


	bool Double::isEqual(double v1 , double v2){
		return abs(v1 - v2) < PRICE_TINY;
	}

	bool Double::isGreaterThan(double v1, double v2) {
		return (v1-v2)>PRICE_TINY;
	}

	bool Double::isLessThan(double v1, double v2) {
		return (v2-v1)<-PRICE_TINY;
	}

	bool Double::isGreaterThanOrEqual(double v1, double v2) {
		return (v1 - v2) > -PRICE_TINY;
	}

	bool Double::isLessThanOrEqual(double v1, double v2) {
		return (v2 - v1) < PRICE_TINY;
	}

	double Double::floor(double val, int precision)
	{
		double multi = qPow(10.0, precision);
		qint64 int64Val = qint64(val*multi);
		return int64Val / multi;
	}

	double Double::ceil(double val, int precision)
	{
		double multi = qPow(10.0, precision);
		double doubleVal = val*multi;
		qint64 int64Val = qint64(doubleVal);
		if (int64Val < doubleVal) {
			++int64Val;
		}
		return int64Val / multi;
	}

	int Double::getPrecision(double val, int precision) {
		QString str = QString::number(val, 'f', precision);
		int dotIndex = str.indexOf('.');
		if (dotIndex < 0) {
			return 0;
		}
		while (str.endsWith("0")) {
			str.chop(1);
		}
		return str.length() - (dotIndex + 1);
	}

	/* TODO test
	{
	double d0 = Double::floor(123.1234567891, 0);
	QString str0 = QString::number(d0, 'f', 0);
	double d1 = Double::floor(123.1234567891, 1);
	QString str1 = QString::number(d1, 'f', 1);
	double d2 = Double::floor(123.1234567891, 2);
	QString str2 = QString::number(d2, 'f', 2);
	double d3 = Double::floor(123.1234567891, 3);
	QString str3 = QString::number(d3, 'f', 3);
	double d4 = Double::floor(123.1234567891, 4);
	QString str4 = QString::number(d4, 'f', 4);
	double d5 = Double::floor(123.1234567891, 5);
	QString str5 = QString::number(d5, 'f', 5);
	double d6 = Double::floor(123.1234567891, 6);
	QString str6 = QString::number(d6, 'f', 6);
	double d7 = Double::floor(123.1234567891, 7);
	QString str7 = QString::number(d7, 'f', 7);
	double d8 = Double::floor(123.1234567891, 8);
	QString str8 = QString::number(d8, 'f', 8);
	double d9 = Double::floor(123.1234567891, 9);
	QString str9 = QString::number(d9, 'f', 9);
	double d10 = Double::floor(123.1234567891, 10);
	QString str10 = QString::number(d10, 'f', 10);
	}
	{
	double d0 = Double::ceil(123.1234567891, 0);
	QString str0 = QString::number(d0, 'f', 0);
	double d1 = Double::ceil(123.1234567891, 1);
	QString str1 = QString::number(d1, 'f', 1);
	double d2 = Double::ceil(123.1234567891, 2);
	QString str2 = QString::number(d2, 'f', 2);
	double d3 = Double::ceil(123.1234567891, 3);
	QString str3 = QString::number(d3, 'f', 3);
	double d4 = Double::ceil(123.1234567891, 4);
	QString str4 = QString::number(d4, 'f', 4);
	double d5 = Double::ceil(123.1234567891, 5);
	QString str5 = QString::number(d5, 'f', 5);
	double d6 = Double::ceil(123.1234567891, 6);
	QString str6 = QString::number(d6, 'f', 6);
	double d7 = Double::ceil(123.1234567891, 7);
	QString str7 = QString::number(d7, 'f', 7);
	double d8 = Double::ceil(123.1234567891, 8);
	QString str8 = QString::number(d8, 'f', 8);
	double d9 = Double::ceil(123.1234567891, 9);
	QString str9 = QString::number(d9, 'f', 9);
	double d10 = Double::ceil(123.1234567891, 10);
	QString str10 = QString::number(d10, 'f', 10);
	}
	*/



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


	BASE_API int prec(InstrumentType type)
	{
		switch (type)
		{
		case mts::TYPE_STK_SPOT:
		case mts::TYPE_HUOBI:
		case mts::TYPE_BITMEX:
		case mts::TYPE_OKEX:
			return 8;
			break;
		default:
			break;
		}
		return 2;
	}

}