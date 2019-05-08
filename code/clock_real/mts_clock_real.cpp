
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
#include "clock_real_api.h"
#include "ClockReal.h"
#include "mts_core/enums.h"

extern "C" {
	MTS_CLOCK_REAL_API mts::Clock*  createClock(mts::EnvironmentMode mode) {
		if (mode != mts::ENVIR_REAL) {
			return nullptr;
		}
		return new mts::ClockReal();
	}

	MTS_CLOCK_REAL_API void releaseClock(mts::Clock* clock) {
		delete clock;
	}
}