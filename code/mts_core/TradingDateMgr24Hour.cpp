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
#include "TradingDateMgr24Hour.h"
#include "base/DateTime.h"

namespace mts {

	TradingDateMgr24Hour::TradingDateMgr24Hour()
	{
	}


	TradingDateMgr24Hour::~TradingDateMgr24Hour()
	{
	}

	int TradingDateMgr24Hour::nextDate(int dt) const
	{
		return DateTime(0,dt).toNextDays(1).date();
	}

	int TradingDateMgr24Hour::prevDate(int dt) const
	{
		return DateTime(0, dt).toNextDays(-1).date();
	}

	int TradingDateMgr24Hour::forceToDate(int dt) const
	{
		return dt;
	}

	int TradingDateMgr24Hour::tradingDate(const DateTime & realDateTime) const
	{
		return realDateTime.date();
	}

	bool TradingDateMgr24Hour::isTradingDate(int dt) const
	{
		return true;
	}

	bool TradingDateMgr24Hour::isHalfTradingDate(int dt) const
	{
		return false;
	}

	bool TradingDateMgr24Hour::isTradingDateTime(const DateTime & dt, TradingDateTimeCheckMode ) const
	{
		return true;
	}

	int TradingDateMgr24Hour::realDate(int tradingDate, int realTime) const
	{
		return tradingDate;
	}

	int TradingDateMgr24Hour::nextNDate(int dt, int n) const
	{
		return DateTime(0, dt).toNextDays(n).date();
	}

	int TradingDateMgr24Hour::countTradingDay(int begDt, int endDt)
	{
		return (DateTime(0, endDt) - DateTime(0, begDt))/ TICS_PER_DAY;
	}

	int TradingDateMgr24Hour::openTradingTime(int dt) const	{
		return 0;
	}

	int TradingDateMgr24Hour::closeTradingTime(int dt) const
	{
		return 235959999;
	}

	DateTime TradingDateMgr24Hour::nextTime(int futureTime, const DateTime & baseTime) const
	{
		if (futureTime < baseTime.time()) {
			return DateTime(futureTime, this->nextDate(baseTime.date()), baseTime.countryCode());
		}else {
			return DateTime(futureTime, baseTime.date(), baseTime.countryCode());
		}
	}


}