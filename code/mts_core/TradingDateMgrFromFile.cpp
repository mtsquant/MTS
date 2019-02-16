
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
#include "TradingDateMgrFromFile.h"
#include "base/MtsLog.h"
#include <QtCore/QFile>
#include "mts_core/enums.h"
#include "mts_core/InstrumentProperty.h"



enum TradingTimePeriod //TODO: need to config by different exch ,now hardcode it for chna furture
{
	TP_NIGHT_PERIOD_BEGIN = 2100,
	TP_NIGHT_PERIOD_END = 230,

	TP_DAY_PERIOD_AM_BEGIN = 900,
	TP_DAY_PERIOD_AM_END = 1130,
	TP_DAY_PERIOD_PM_BEFIN = 1330,
	TP_DAY_PERIOD_PM_END = 1500,

	TP_DAY_PERIOD_BEGIN = TP_DAY_PERIOD_AM_BEGIN,
	TP_DAY_PERIOD_END = TP_DAY_PERIOD_PM_END,


	TP_DAY_PERIOD_ROUND_BEGIN = 845,
	TP_DAY_PERIOD_ROUND_END = 1515,

	TP_NIGHT_PERIOD_ROUND_BEGIN = 2045,
	TP_NIGHT_PERIOD_ROUND_END = 245


};

enum TradingTime
{
	TT_DAY_BEGIN_TIME = TP_DAY_PERIOD_ROUND_BEGIN*100000,
	//TT_NIGHT_BEGIN_TIME_BEFORE = 195959999,
	TT_NIGHT_BEGIN_TIME = TP_NIGHT_PERIOD_ROUND_BEGIN*100000,
	TT_END_TIME = TP_DAY_PERIOD_ROUND_END*100000
};


namespace mts {


	TradingDateMgrFromFile::TradingDateMgrFromFile()
	{
	}

	TradingDateMgrFromFile::~TradingDateMgrFromFile()
	{

	}


	bool TradingDateMgrFromFile::initialize(const QString& configDir)
	{
		if (this->isInitialized()) {
			return true;
		}

		QString configPath = configDir + "/trading_date_cn_fut.txt";
		QFile file(configPath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			MTS_ERROR("can not open file: '%s'\n", qPrintable(configPath));
			return false;
		}

		QString line;
		int preDate = INVALID_DATE;
		//int nextDate = INVALID_DATE;
		int lastDate = INVALID_DATE;
		bool halfTrade = false;
		while (!file.atEnd()) {
			line = file.readLine().simplified();
			if (line.isEmpty() || line.startsWith("#")) {
				continue;
			}
			QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
			int dt = fields[0].toInt();
			halfTrade = (fields.size() > 1) ? fields[1].toInt() : false;
			if (lastDate != INVALID_DATE) {
				_dateMap.insert(lastDate, { preDate,dt,halfTrade });
			}
			preDate = lastDate;
			lastDate = dt;
		}
		if (lastDate != INVALID_DATE) {
			_dateMap.insert(lastDate, { preDate,INVALID_DATE, halfTrade });
		}
		file.close();
		return true;
	}

	bool TradingDateMgrFromFile::isInitialized() const {
		return !_dateMap.isEmpty();
	}

	int TradingDateMgrFromFile::nextDate(int dt) const
	{
		auto it = _dateMap.constFind(dt);
		if (it == _dateMap.constEnd()) {
			MTS_ERROR("is not a trading date:%d\n", dt);
			return INVALID_DATE;
		}
		return it.value().next;
	}

	int TradingDateMgrFromFile::prevDate(int dt) const
	{
		auto it = _dateMap.constFind(dt);
		if (it == _dateMap.constEnd()) {
			MTS_ERROR("is not a trading date:%d\n", dt);
			return INVALID_DATE;
		}
		return it.value().prev;
	}

	int TradingDateMgrFromFile::forceToDate(int dt) const
	{
		auto it = _dateMap.find(dt);
		if (it == _dateMap.end()) {
			it = _dateMap.lowerBound(dt);
			if (it == _dateMap.end()) {
				MTS_ERROR("%d is out of trading date config\n", dt);
				if (_dateMap.isEmpty()) {
					return INVALID_DATE;
				}
				else {
					return _dateMap.lastKey();
				}
			}
		}
		return it.key();
	}
	int TradingDateMgrFromFile::tradingDate(const DateTime & realDateTime) const {
		/*
		1: [TT_DAY_BEGIN_TIME, TT_NIGHT_BEGIN_TIME)  return date if it is trading date
		2: [TT_NIGHT_BEGIN_TIME,235959999] return next trading date if it is trading date
		3: [000000000,TT_DAY_BEGIN_TIME) return yesterday's next  trading date if yesterday is trading date
		*/
		const int date = realDateTime.date();
		const int time = realDateTime.time();

		if (time >= TT_DAY_BEGIN_TIME && time < TT_NIGHT_BEGIN_TIME) {
			return isTradingDate(date) ? date : INVALID_DATE;
		}

		if (time >= TT_NIGHT_BEGIN_TIME && time <= 235959999) {
			return isTradingDate(date) ? nextDate(date) : INVALID_DATE;
		}

		if (time >= 0 && time < TT_DAY_BEGIN_TIME) {
			int yesterday = realDateTime.toNextDays(-1).date();
			return isTradingDate(yesterday) ? nextDate(yesterday) : INVALID_DATE;
		}

		return INVALID_DATE;
	}

	bool TradingDateMgrFromFile::isTradingDate(int dt) const {
		return this->forceToDate(dt) == dt;
	}

	bool TradingDateMgrFromFile::isHalfTradingDate(int dt) const {
		auto info = _dateMap.value(dt);
		if (info.isEmpty()) {
			return false;
		}
		else {
			return info.halfTrading;
		}
	}

	int TradingDateMgrFromFile::realDate(int tradingDate, int realTime) const {
		if (!isTradingDate(tradingDate)) {
			return INVALID_DATE;
		}
		if (realTime >= 0 && realTime < TT_NIGHT_BEGIN_TIME) {
			return tradingDate;
		}

		return prevDate(tradingDate);
	}

	int TradingDateMgrFromFile::nextNDate(int dt, int n) const {
		int baseDate = this->forceToDate(dt);
		if (baseDate == INVALID_DATE) {
			return INVALID_DATE;
		}

		auto it = _dateMap.constFind(baseDate);
		if (it == _dateMap.constEnd()) {
			MTS_ERROR("is not a trading date:%d\n", dt);
			return INVALID_DATE;
		}

		for (auto i = 0; i < n; ++i) {
			if (++it == _dateMap.constEnd()) {
				return INVALID_DATE;
			}
		}

		return it.key();
	}

	int TradingDateMgrFromFile::countTradingDay(int begDt, int endDt)
	{
		if (_dateMap.isEmpty()) {
			return 0;
		}
		if (begDt > endDt) {
			int temp = begDt;
			begDt = endDt;
			endDt = temp;
		}

		if (_dateMap.firstKey() > begDt || _dateMap.lastKey() < endDt) {
			return 0;
		}

		int count = 0;
		for (auto it = _dateMap.constBegin(), itEnd = _dateMap.constEnd(); it != itEnd; ++it) {
			if (it.key() >= begDt) {
				if (it.key() <= endDt) {
					++count;
				}
				else {
					break;
				}
			}
		}
		return count;
	}

	int TradingDateMgrFromFile::openTradingTime(int dt) const
	{
		if (dt == INVALID_DATE) {
			return TP_NIGHT_PERIOD_ROUND_BEGIN * 100000;
		}

		if (isHalfTradingDate(dt)) {
			return TP_DAY_PERIOD_ROUND_BEGIN*100000;
		}	else {
			return TP_NIGHT_PERIOD_ROUND_BEGIN * 100000;
		}
		
	}

	int TradingDateMgrFromFile::closeTradingTime(int dt) const
	{
		return TP_DAY_PERIOD_ROUND_END* 100000;
	}

	DateTime TradingDateMgrFromFile::nextTime(int futureTime, const DateTime& baseTime) const
	{
		DateTime n = baseTime;
		if (futureTime < n.time()) {
			if (n.time() >= TT_NIGHT_BEGIN_TIME && futureTime < TT_NIGHT_BEGIN_TIME) {
				if (futureTime < TT_DAY_BEGIN_TIME) {
					n = n.toNextDays(1);  //just be next date include no trading date
				}else {
					int date = this->nextDate(n.date());
					if (date == INVALID_DATE) {
						date = this->forceToDate(n.date());
					}
					n.setDate(date);
				}

			}
		}
		n.setTime(futureTime);
		return n;
	}

	bool TradingDateMgrFromFile::isTradingDateTime(const DateTime & dt, TradingDateTimeCheckMode tcm) const {
		static TradingSections  _allExchTradingSection = TradingSections()
			<< TradingTimeRange(tcm == TCM_EXACT ? TP_DAY_PERIOD_BEGIN : TP_DAY_PERIOD_ROUND_BEGIN, TP_DAY_PERIOD_AM_END, true)
			<< TradingTimeRange(TP_DAY_PERIOD_PM_BEFIN, tcm == TCM_EXACT ? TP_DAY_PERIOD_END : TP_DAY_PERIOD_ROUND_END, true)
			<< TradingTimeRange(tcm == TCM_EXACT ? TP_NIGHT_PERIOD_BEGIN : TP_NIGHT_PERIOD_ROUND_BEGIN, 2359, true)
			<< TradingTimeRange(0, tcm == TCM_EXACT ? TP_NIGHT_PERIOD_END : TP_NIGHT_PERIOD_ROUND_END, true);


		const int tradingDate = this->tradingDate(dt);
		if (tradingDate == INVALID_DATE) {
			return false;
		}

		const int time = dt.time() / 100000; //remove sec and millisec fields
		for (int i = 0, size = _allExchTradingSection.size(); i < size; ++i) {
			const TradingTimeRange& tradingRange = _allExchTradingSection[i];
			if (tradingRange.isInSection(time)) {
				return true;
			}
		}
		return false;
	}


}