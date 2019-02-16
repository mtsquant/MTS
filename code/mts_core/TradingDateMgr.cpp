
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
#include "TradingDateMgr.h"
#include "TradingDateMgrFromFile.h"
#include "TradingDateMgr24Hour.h"


namespace mts
{

	TradingDateMgr* TradingDateMgr::_instance = nullptr;

	bool TradingDateMgr::init(const QString & configDir, bool is24H) {
		if (_instance != nullptr) {
			return true;
		}
		if (is24H) {
			_instance = new TradingDateMgr24Hour();
			return true;
		}else {
			TradingDateMgrFromFile* tdm = new TradingDateMgrFromFile();
			if (!tdm->initialize(configDir)) {
				delete tdm;
				return false;
			}
			_instance = tdm;
			return true;
		}
	}

	mts::TradingDateMgr* TradingDateMgr::instance()
	{
		assert(TradingDateMgr::_instance);
		return TradingDateMgr::_instance;
	}

	
}
