
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

	class TradingDateMgr24Hour : public TradingDateMgr
	{
	public:
		TradingDateMgr24Hour();
		virtual ~TradingDateMgr24Hour();

		virtual int nextDate(int dt) const override;
		virtual int prevDate(int dt) const override;
		virtual int forceToDate(int dt) const override;
		virtual int tradingDate(const DateTime & realDateTime) const override;
		virtual bool isTradingDate(int dt) const override;
		virtual bool isHalfTradingDate(int dt) const override;
		virtual bool isTradingDateTime(const DateTime & dt, TradingDateTimeCheckMode = TCM_EXACT) const override;
		virtual int realDate(int tradingDate, int realTime) const override;
		virtual int nextNDate(int dt, int n) const override;
		virtual int countTradingDay(int begDt, int endDt) override;
		virtual int openTradingTime(int dt) const override;
		virtual int closeTradingTime(int dt ) const override;
		virtual DateTime nextTime(int futureTime, const DateTime& baseTime) const override;
	};

}
