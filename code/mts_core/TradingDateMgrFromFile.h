
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
#include "mts_core/TradingDateMgr.h"

namespace mts {

	class MTS_CORE_API TradingDateMgrFromFile :public TradingDateMgr
	{
	public:
		TradingDateMgrFromFile();
		virtual ~TradingDateMgrFromFile();

		bool initialize(const QString& configDir);
		bool isInitialized() const;

		virtual int nextDate(int dt) const override;  //next trading date
		virtual int prevDate(int dt) const override;  //prev trading date
		virtual int forceToDate(int dt) const override; //if  dt is a trading date return it, else return the next date

		virtual int tradingDate(const DateTime&  realDateTime) const  override; //return INVALID_DATE means it is not trading date
		virtual bool isTradingDate(int dt) const  override;
		virtual bool isHalfTradingDate(int dt) const  override;
		virtual bool isTradingDateTime(const DateTime& dt, TradingDateTimeCheckMode = TCM_EXACT) const override;

		virtual int realDate(int tradingDate, int realTime) const override;

		virtual int nextNDate(int dt, int n) const override;
		virtual int countTradingDay(int begDt, int endDt) override;

		virtual int openTradingTime(int dt) const override;
		virtual int closeTradingTime(int dt) const override;
		virtual DateTime nextTime(int futureTime, const DateTime& baseTime) const override;
	private:
		struct TradingDateInfo
		{
			TradingDateInfo() :
				prev(INVALID_DATE), next(INVALID_DATE), halfTrading(false)
			{
			}
			TradingDateInfo(int p, int n, bool half) :
				prev(p), next(n), halfTrading(half)
			{
			}
			bool isEmpty() const {
				return prev == INVALID_DATE && next == INVALID_DATE;
			}
			int prev;
			int next;
			bool halfTrading;
		};
		QMap<int/*date*/, TradingDateInfo > _dateMap;
	};
}
