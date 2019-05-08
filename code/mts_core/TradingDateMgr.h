
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
#include "mts_core_api.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QPair>
#include "base/DateTime.h"
#include "mts_core/enums.h"


namespace mts
{
	enum TradingDateTimeCheckMode {
		TCM_EXACT,		//exact trading time period
		TCM_ROUND		//15 minutes before begin point and 15 min after end point
	};

	class MTS_CORE_API TradingDateMgr
	{
	public:
		static bool init(const QString& configDir, bool is24H); //It can be called multiable times
		static TradingDateMgr* instance();

		virtual int nextDate(int dt) const=0;  //next trading date
		virtual int prevDate(int dt) const = 0;  //prev trading date
		virtual int forceToDate(int dt) const = 0; //if  dt is a trading date return it, else return the next date

		virtual int tradingDate(const DateTime&  realDateTime) const = 0; //return INVALID_DATE means it is not trading date
		virtual bool isTradingDate(int dt) const = 0;
		virtual bool isHalfTradingDate(int dt) const = 0;
		virtual bool isTradingDateTime(const DateTime& dt, TradingDateTimeCheckMode = TCM_EXACT) const = 0;

		virtual int realDate(int tradingDate, int realTime) const = 0;

		virtual int nextNDate(int dt, int n) const = 0;
		virtual int countTradingDay(int begDt, int endDt) = 0;

		virtual int openTradingTime(int dt=INVALID_DATE) const = 0; //get the open market time on the date ( diff on half trading date)
		virtual int closeTradingTime(int dt = INVALID_DATE) const = 0;
		virtual DateTime nextTime(int futureTime, const DateTime& baseTime) const = 0;// 根据baseTime 得到未来时间 futureTime 对应的完整日期和时间（timer 用）
		virtual bool is24Hour() const { return false; };
	private:
		static TradingDateMgr* _instance;
	};




}