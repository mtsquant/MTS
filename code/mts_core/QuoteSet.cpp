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
#include "QuoteSet.h"

namespace mts
{

	QuoteSet::QuoteSet() 
	{
	}


	QuoteSet::~QuoteSet() 
	{
	}

	QuoteSnapshotPtr QuoteSet::updateQuote(QuotePtr qt) {
		QuoteSnapshotPtr& cacheQt = _quotes[qt->instrumentId()];
		if (cacheQt == nullptr) {
			cacheQt = QuoteSnapshotPtr(new QuoteSnapshot(*qt));
		} else {
			cacheQt->merge(qt.data());
		}
		return cacheQt;
	}

	QuoteSnapshotPtr QuoteSet::getQuote(const InstrumentId & instrumentId) {
		return _quotes[instrumentId];
	}

	int QuoteSet::size() const {
		return _quotes.size();
	}

	void QuoteSet::clear()
	{
		_quotes.clear();
	}

}