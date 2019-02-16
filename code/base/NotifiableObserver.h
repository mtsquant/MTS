
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
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <QtCore/QList>
#include "base/base_api.h"

template<class _Tcallback, class _Tobject>
class NotifiableObserver
{
public:

	template<typename _TnotifyFun>
	NotifiableObserver(_TnotifyFun notifyFun) {
		_notifyFunction = boost::bind(notifyFun, _1, _2);
	}
	virtual ~NotifiableObserver() {}

	int addCallback(_Tcallback* cb) {
		int id = findCallback(cb);
		if (id!=-1) {
			return  id;
		}
		_callbacks.append(cb);
		return _callbacks.size();
	}
	int findCallback(_Tcallback* cb) {
		for (int i = 0; i < _callbacks.size(); ++i) {
			if (_callbacks[i] == cb) {
				return i;
			}
		}
		return -1;
	}
	bool hasCallback(_Tcallback* cb) {
		int id = findCallback(cb);
		return id != -1;
	}
	bool removeCallback(_Tcallback* cb) {
		int id = findCallback(cb);
		if (id == -1) {
			return false;
		}
		_callbacks[id] = nullptr;
		return true;
	}

	bool destroyCallback(int id) {
		if (id > _callbacks.size() || id <= 0) {
			return false;
		}
		auto cb = _callbacks[id - 1];
		delete cb;
		_callbacks[id - 1] = nullptr;
		return true;
	}

protected:
	void notifyAll(_Tobject obj) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			_Tcallback* cb = _callbacks[i];
			if (cb) {
				_notifyFunction(cb, obj);
			}
		}
	}
private:
	boost::function<void(_Tcallback*, _Tobject)> _notifyFunction;
	QList<_Tcallback*> _callbacks;
};

