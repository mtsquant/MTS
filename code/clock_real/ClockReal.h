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
#include "base/DateTime.h"
#include "mts_core/Clock.h"
#include <QtCore/QTimer>


namespace mts
{

	class ClockTimerReal :public QTimer
	{
		Q_OBJECT
	public:
		ClockTimerReal(int id,TimerCallback*, QObject * parent = 0);
		virtual ~ClockTimerReal();

	private Q_SLOTS:
		void onTimeout();
	private:
		const int _id;
		TimerCallback* _callback;
	};


	class ClockReal :public Clock
	{
	public:
		ClockReal(void);
		virtual ~ClockReal(void);

		virtual DateTime now() const override;
		virtual int currentTradingDay () const override;
	protected:
		virtual QObject* doCreateTimer(int id, qint64 beginTicks, int interval, TimerCallback*) override;
		virtual bool doDestroyTimer(QObject*) override;

		// ͨ�� Clock �̳�
		virtual bool initialize(const QVariantMap & params) override;
	};

}