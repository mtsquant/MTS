
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
#include "QuoteMgr.h"
#include "mts_core/Environment.h"
#include "MtsMainThread.h"
#include "PositionMgr.h"

namespace mts
{


	QuoteMgr::QuoteMgr() {
		assert(Environment::instance()->isCurrentMtsThread());
	}


	QuoteMgr::~QuoteMgr() {
		//Environment::instance()->feeds()->removeCallback(this);
	}

	QuoteMgr * QuoteMgr::instance() {
		static QuoteMgr qtMgr;
		return &qtMgr;
	}

	void QuoteMgr::setStrategy(StrategyInterface * notify) {
		this->_notify = notify;
	}

	int QuoteMgr::subscribe(const QList<InstrumentId>& instrumentIds) {
		if (!Environment::instance()->feeds()->hasCallback(this)) {
			Environment::instance()->feeds()->addCallback(this);
		}
		return Environment::instance()->feeds()->subscribe(instrumentIds);
	}

	int QuoteMgr::unsubscribe(const QList<InstrumentId>& instrumentIds) {
		return Environment::instance()->feeds()->unsubscribe(instrumentIds);
	}

	void QuoteMgr::onQuoteUpdate(QuotePtr qt) {
		auto cacheQuote=_quoteSet.updateQuote(qt);
		if (_notify) {
			_notify->onQuoteSnapshotUpdate(cacheQuote);
		}
		QList<Position*> poss = PositionMgr::instance()->getStrategyPositions(cacheQuote->instrumentId());
		for(auto&pos:poss)
		{
			pos->processQuote(cacheQuote);
		}
	}

	void QuoteMgr::onBarUpdate(CalcBarPtr bar)
	{
		if (_notify) {
			_notify->onBarUpdate(bar);
		}
		//TODO:?
		//QList<Position*> poss = PositionMgr::instance()->getStrategyPositions(cacheQuote->instrumentId());
		//for (auto&pos : poss)
		//{
		//	pos->processBar(bar);
		//}
	}

	Quote* QuoteMgr::getQuote(const InstrumentId& instrumentId) {
		return _quoteSet.getQuote(instrumentId).data();
	}

	int QuoteMgr::subscribedInstrumentCount() const {
		return _quoteSet.size();
	}

	void QuoteMgr::onBusinessDateChanged(int businessDate) {

	}


}