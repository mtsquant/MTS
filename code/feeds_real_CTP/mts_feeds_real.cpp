
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
#include "feeds_real_api.h"
#include "FeedsReal.h"
#include "mts_core/enums.h"

using namespace mts;


extern "C" {
	MTS_FEEDS_REAL_API mts::FeedsBase*  createFeeds() {
		return new mts::FeedsReal();
	}

	MTS_FEEDS_REAL_API void releaseFeeds(mts::Feeds* feeds) {
		delete feeds;
	}

	MTS_FEEDS_REAL_API QList<ExchId>* supportExchs() {
		static QList<ExchId> exchs = QList<ExchId>()	<< EXCH_SHFE
											<< EXCH_DCE
											<< EXCH_CZCE
											<< EXCH_CFFEX
											<< EXCH_INE;
		return &exchs;
	}

}