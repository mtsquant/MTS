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

	//===========================

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
		//if ( now.time () >= TT_NIGHT_BEGIN_TIME ){
		//	return TradingDateMgr::instance ()->nextDate ( now.date () );
		//} else{
		//	return TradingDateMgr::instance ()->forceToDate(now.date ());
		//}
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
		//no config for ClockReal
		return true;
	}


}