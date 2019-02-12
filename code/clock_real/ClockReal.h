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

		// Í¨¹ý Clock ¼Ì³Ð
		virtual bool initialize(const QVariantMap & params) override;
	};

}