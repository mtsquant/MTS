#include "clock_real_api.h"
#include "ClockReal.h"

extern "C" {
	MTS_CLOCK_REAL_API mts::Clock*  createClock(const QVariantMap& params) {
		return new mts::ClockReal();
	}

	MTS_CLOCK_REAL_API void releaseClock(mts::Clock* clock) {
		delete clock;
	}
}