
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
#include "Clock.h"
#include "mts_core/TradingDateMgr.h"
#include "mts_core/enums.h"
#include "base/MtsLog.h"
#ifndef _WIN32
#include <math.h>
#endif


namespace mts
{

	Clock::Clock(void) 
		:Component()
	{
		DateTime::initTimezoneDatabase();
	}


	Clock::~Clock(void) {
	}



	qint64 Clock::calcBeginUTCTicks(int beginTime, int interval) const {
		auto nowDt= this->now();
		auto dt = TradingDateMgr::instance()->nextTime(beginTime, nowDt);

		auto beginUTCTicks = dt.toUTCMillisecsSinceEpoch();
		auto nowUTCTicks = nowDt.toUTCMillisecsSinceEpoch();

		if (beginUTCTicks < nowUTCTicks) {
			if (interval == 0) {//ignore the only once timer which begintime < now
				return -1;
			} else {
				beginUTCTicks += (ceil((nowUTCTicks - beginUTCTicks) / (interval*1.0))*interval);
			}
		}
		if (beginUTCTicks - nowUTCTicks < 1000) { //空出1 second (timer 精确到秒足够)，防止下次计算begin 超过了当时的now
			beginUTCTicks += interval;
		}
		assert(beginUTCTicks >= nowUTCTicks);
		return beginUTCTicks;
	}

	int Clock::createTimer(int beginTime, int interval, TimerCallback* cb) {
		auto beginUTCTicks = calcBeginUTCTicks(beginTime, interval);
		if (beginUTCTicks == -1) { //invalid
			MTS_ERROR("Failed to create single timer earlier than now\n");
			return 0;
		}
		QObject* timer = doCreateTimer(_ids.size()+1, beginUTCTicks ,interval, cb);
		if (timer) {
			_ids.append(timer);
			return _ids.size();
		} else {
			MTS_ERROR("Failed to create the timer (%d,%d)\n",beginTime,interval);
			return 0;
		}
	}

	bool Clock::destroyTimer(int id) {
		if (id > _ids.size() || id <= 0) {
			return false;
		}
		QObject* timer = _ids.at(id - 1);
		if (!timer) {
			return false;
		}
		if (!doDestroyTimer(timer)) {
			return false;
		}
		_ids[id - 1] = nullptr;
		return true;
	}

	void Clock::addDateCallback(DateCallback * dc) {
		_dateCallbacks.append(dc);
	}

	void Clock::notifyBusinessDateChanged(int newBuseinssDate) {
		for (int i = 0, size = _dateCallbacks.size(); i < size; ++i) {
			_dateCallbacks[i]->onBusinessDateChanged(newBuseinssDate);
		}
	}

	void Clock::clear() // Just only be called in subclass' destroy function
	{
		for (int i = 0, size = _ids.size(); i < size; ++i) {
			QObject* timer = _ids[i];
			if (timer) {
				doDestroyTimer(timer);
			}
		}
		_ids.clear();
	}


}
