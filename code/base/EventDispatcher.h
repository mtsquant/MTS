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

#include "NotifiableObserver.h"
#include "Event.h"

class EventCallback 
{
public:
	virtual void onEvent(const EventPtr&) = 0;
};

class EventQueue;
//Dispatcher does not thread safe
class BASE_API EventDispatcher :public NotifiableObserver<EventCallback, EventPtr>
{
public:
	EventDispatcher(EventQueue*);
	~EventDispatcher();

	static EventDispatcher* defaultDispatcher();

	int processEvents(); //return processed events count
private:
	EventQueue* _queue;
};

