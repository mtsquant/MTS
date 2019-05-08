
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

#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <boost/function.hpp>
#include "base/base_api.h"
#include "base/Timezone.h"
#include "base/Ticks.h"

#define INVALID_DATE 99999999

class BASE_API DateTime
{
public:
	static DateTime now();
	static int time2LocalTicksFromMidnight(int time/*hhmmsszzz*/);
	static int localTicksFromMidnight2Time(int localTicksFromMidnight);
	static DateTime localMillisecsSinceEpoch(qint64 millsecs, TimezoneCode = TIMEZONE_CST);
	static qint64 nowToUTCMicrosecsSinceEpoch(); //NOTE: always from real time
	static DateTime fromQDateTime(const QDateTime&);


	DateTime();
	explicit DateTime(qint64 utcMillisecsSinceEpoch,TimezoneCode  =TIMEZONE_CST);
	DateTime(int time/*210101000*/,int date,TimezoneCode=TIMEZONE_CST);
	DateTime(int localHour,int localMinute,int localSecond,int localMillisec,int localDate,TimezoneCode=TIMEZONE_CST);
	DateTime(const DateTime&);
	~DateTime();
	qint64 toUTCMillisecsSinceEpoch ()const;

	qint64 operator- (const DateTime&) const;
	DateTime operator+ (qint64) const;
	DateTime operator- (qint64) const;
	bool operator==(const DateTime&) const;
	bool operator!=(const DateTime&) const;
	bool operator>(const DateTime&) const;
	bool operator>=(const DateTime&) const;
	bool operator<(const DateTime&) const;
	bool operator<=(const DateTime&) const;

	DateTime toNextDays(int days) const;

	int date() const;//yyyyMMdd
	void setDate(int);
	int time() const;//hhmmsszzz
	void setTime(int);
	int ticksSinceMidnight() const;

	int year() const; //1970~3000
	int month() const; //1~12
	int day() const;//1~31

	int hour() const;//0~23
	int minute() const;//0~59
	int second() const;//0~59
	int millisecond() const;//0~999

	int hoursTo(const DateTime& other) const;
	int minutesTo(const DateTime& other) const;
	int secondsTo(const DateTime& other, bool plusADayIfnegative) const;
	qint64 millisecondsTo(const DateTime& other) const;

public:

	bool isNull() const;
	bool isUTC() const;
	TimezoneCode countryCode() const;
	
	QString toString(QChar dateTimeSep=QChar(' ')) const;
	QString toLocalString(QChar dateTimeSep=QChar(' ')) const; //not include timezone

	DateTime toUTC() const;
	DateTime toLocal(TimezoneCode targetTimezone=TIMEZONE_CST) const;

	static bool initTimezoneDatabase();

	static const DateTime Null;

private:
	void addMilliseconds(qint64 milliseconds);
	void resetToMidnight();
	void resetToNoon();
	void resetTicksSinceMidnight(int ticksFromMidnight);
	void clear();

private:
	LocalTicks _localTicks; 
	UtcTicks _utcTicks; 

# ifndef NDEBUG
	int _date;
	int _time;
#endif

	static boost::function<DateTime ()> _nowFunctor;
public:
	static boost::function<DateTime()> setNowFunctor ( boost::function<DateTime ()> nowFunctor );
};

Q_DECLARE_METATYPE(DateTime);