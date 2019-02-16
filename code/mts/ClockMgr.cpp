
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
#include "ClockMgr.h"
#include "mts_core/Environment.h"
#include "MtsMainThread.h"

namespace mts
{


	ClockMgr::ClockMgr() {
		assert(Environment::instance()->isCurrentMtsThread());
	}


	ClockMgr::~ClockMgr() {
	}

	ClockMgr * ClockMgr::instance() {
		static ClockMgr mgr;
		return &mgr;
	}

	bool ClockMgr::destoryTimer(int timerId){
		return  Environment::instance()->clock()->destroyTimer(timerId);
	}

	DateTime ClockMgr::now() const {
		return Environment::instance()->clock()->now();
	}

	int ClockMgr::currentTradingDay () const{
		return Environment::instance ()->clock ()->currentTradingDay();
	}


	int ClockMgr::createTimer(int beginTime, int interval, TimerCallback * callback) {
		return Environment::instance()->clock()->createTimer(beginTime,interval,callback);
	}

}