
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
#include "Feeds.h"
#include <QtCore/QObject>

namespace mts
{
	
	FeedsBase::FeedsBase()
		:Component()
	{
	}

	FeedsBase::~FeedsBase()
	{
	}

	Feeds::Feeds() 
		:FeedsBase()
	{
	}


	Feeds::~Feeds(void) {
	}

    int Feeds::addCallback(FeedsCallback * cb) {
		int id = findCallback(cb);
		if (id != -1) {
			return  id;
		}
        _callbacks.push_back(cb);
        return _callbacks.size();
    }

    int Feeds::addFrontCallback(FeedsCallback *cb)
    {
        int id = findCallback(cb);
        if (id != -1) {
            return  id;
        }
        _callbacks.push_front(cb);
        return _callbacks.size();
    }

    int Feeds::findCallback(FeedsCallback * cb) {
		for (int i = 0; i < _callbacks.size(); ++i) {
			if (_callbacks[i] == cb) {
				return i;
			}
		}
		return -1;
	}

    bool Feeds::hasCallback(FeedsCallback * cb) {
		int id = findCallback(cb);
		return id != -1;
	}

    bool Feeds::removeCallback(FeedsCallback * cb) {
		int id = findCallback(cb);
		if (id == -1) {
			return false;
		}
		_callbacks[id] = nullptr;
		return true;
	}


	void Feeds::onBusinessDateChanged(int businessDate) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			FeedsCallback* cb = _callbacks[i];
			if (cb) {
				cb->onBusinessDateChanged(businessDate);
			}
		}
	}


	void Feeds::notifyAll(QuotePtr qt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			FeedsCallback* cb = _callbacks[i];
			if (cb) {
				cb->onQuoteUpdate(qt);
			}
		}
	}

	void Feeds::notifyAll(CalcBarPtr bar){
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			FeedsCallback* cb = _callbacks[i];
			if (cb) {
				cb->onBarUpdate(bar);
			}
		}
	}

	void Feeds::notifyOne(int callbackIdx, CalcBarPtr bar)	{
		assert(callbackIdx < _callbacks.size() || callbackIdx >= 0);
		if (callbackIdx >= _callbacks.size() || callbackIdx < 0) {
			return;
		}
		FeedsCallback* cb = _callbacks[callbackIdx];
		if (cb) {
			cb->onBarUpdate(bar);
		}
	}

	int Feeds::callbacksCount() const	{
		return _callbacks.size();
	}



}
