
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
#include "./AbsStrategy.h"
#include "mts/StrategyMgr.h"
#include "mts/CrossThreadNotifier.h"

AbsStrategy::AbsStrategy(const std::string& name , int strategyId, const std::string& strategyPath)
	:QObject() , _notifier(new mts::CrossThreadNotifier(name,strategyId,strategyPath))
{
   	_notifier->connectSignalInitializedl(this);
	StrategyMgr::instance()->registerStrategy(_notifier);
}


AbsStrategy::~AbsStrategy()
{
	StrategyMgr::instance()->unregisterStrategy(_notifier);
	delete _notifier;
	_notifier = nullptr;
}


int AbsStrategy::newTimer(int beginTime, int interval) {
	return StrategyMgr::instance()->newTimer(_notifier, beginTime, interval);
}

int AbsStrategy::newOnceTimer(int beginTime) {
	return StrategyMgr::instance()->newOnceTimer(_notifier, beginTime);
}

bool AbsStrategy::removeTimer(int id) {
	return StrategyMgr::instance()->removeTimer(_notifier, id);
}


