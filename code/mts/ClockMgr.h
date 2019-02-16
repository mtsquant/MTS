
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
#include <QtCore/QObject>
#include "mts_core/TimerCallback.h"
#include "base/DateTime.h"

namespace mts
{

	class ClockMgr :public QObject
	{
		Q_OBJECT
		ClockMgr();
		~ClockMgr();
	public:
		static ClockMgr* instance();


	public Q_SLOTS:
		int createTimer(int beginTime,int interval, TimerCallback*);
		bool destoryTimer(int timerId);
		DateTime now () const;
		int currentTradingDay () const;
		//bool generateNextTick();
	};

}