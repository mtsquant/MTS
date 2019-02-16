
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
#include "ClockReal.h"
#include <QtCore/QDateTime>
#include "mts_core/TradingDateMgr.h"
#include "base/MtsLog.h"
#include "mts_core/enums.h"

namespace mts
{

	ClockTimerReal::ClockTimerReal(int id, TimerCallback* cb, QObject * parent /*= 0*/)
		:QTimer(parent), _callback(cb) ,_id(id)
	{
		this->setTimerType(Qt::PreciseTimer);
		connect(this, SIGNAL(timeout()), this, SLOT(onTimeout()));
	}

	ClockTimerReal::~ClockTimerReal() {

	}

	void ClockTimerReal::onTimeout() {
		if (TradingDateMgr::instance()->isTradingDateTime(DateTime::now(), mts::TCM_ROUND)) {
			_callback->onTimeout(_id);
		}
	}


	ClockReal::ClockReal(void)
	{
	}


	ClockReal::~ClockReal(void) 
	{
		this->clear();
	}

	DateTime ClockReal::now() const {
		return DateTime::fromQDateTime( QDateTime::currentDateTime() );
	}

	int ClockReal::currentTradingDay () const{
		auto now=this->now();
		return TradingDateMgr::instance()->tradingDate(now);
	}

	QObject* ClockReal::doCreateTimer(int id, qint64 beginTicks, int interval, TimerCallback* cb) {
		QTimer* timer = new ClockTimerReal(id,cb);
		timer->setInterval(interval);
		if (interval == 0) {
			timer->setSingleShot(true);
		}
		int beginInterval = beginTicks - this->now().toUTCMillisecsSinceEpoch();
		assert ( beginInterval > 0 );
		MTS_DEBUG ("New real timer (%d) which will be start in %d ms\n",interval,beginInterval);
		QTimer::singleShot(beginInterval, timer, SLOT(start()));
		return timer;
	}

	bool ClockReal::doDestroyTimer(QObject* tmr) {
		QTimer* timer = dynamic_cast<QTimer*>(tmr);
		if (timer) {
			timer->stop();
			delete timer;
			return true;
		} else {
			return false;
		}
	}

	bool ClockReal::initialize(const QVariantMap & params) {
		return true;
	}


}