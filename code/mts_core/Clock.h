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
#include "Component.h"
#include "base/DateTime.h"
#include "TimerCallback.h"
#include "DateCallback.h"

namespace mts
{
	//All Timers generator
	class MTS_CORE_API Clock:public Component
	{
	public:
		Clock(void);
		virtual ~Clock(void);
		virtual DateTime now() const = 0;
		virtual int currentTradingDay () const= 0;
		
		int createTimer(int beginTime,int interval, TimerCallback*); //if failed return 0
		bool destroyTimer(int id);

		void addDateCallback(DateCallback*);
	protected:
		virtual QObject* doCreateTimer(int id, qint64 beginTicks, int interval, TimerCallback*) = 0;
		virtual bool doDestroyTimer(QObject*) = 0;
		void clear();
		void notifyBusinessDateChanged(int newBuseinssDate);
	private:
		QList<QObject*> _ids;
		QList<DateCallback*> _dateCallbacks;
#ifdef MTS_UNIT_TEST
	public:
#endif
		//DateTime todayTime(int tm/*hhmmzz000*/) const;
		qint64 calcBeginUTCTicks(int beginTime, int interval) const;
	};

}
