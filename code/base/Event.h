
/*****************************************************************************
* Copyright [2017-2019] [MTSQuant]
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
#include <QtCore/QSharedPointer>
#include "base/base_api.h"

class BASE_API Event {
public:
	virtual ~Event();
	int type() const;
protected:
	Event(int type);
private:
	const int _type;
};

typedef QSharedPointer<Event>  EventPtr;


template<class _Ty,int _EvtType>
class DataEvent :public Event {
public:
	DataEvent(const _Ty& data) 
		:Event(_EvtType), _data(data)
	{
	}
	virtual ~DataEvent() {

	}

	_Ty data() const {
		return _data;
	}
private:
	_Ty  _data;
};

