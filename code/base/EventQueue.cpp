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
#include "EventQueue.h"



EventQueue::EventQueue() {
}


EventQueue::~EventQueue() {
}

EventQueue * EventQueue::defaultQueue() {
	static EventQueue queue;
	return &queue;
}

//just push threads call the push()

void EventQueue::push(const EventPtr & evt) {
	QMutexLocker l(&_mutex);
	_pushedQueue.append(evt);
}

//the following functions shoule be called by pop thread

EventPtr EventQueue::pop() {
	if (_popedQueue.isEmpty()) {
		QMutexLocker l(&_mutex);
		if (!_pushedQueue.isEmpty()) {
			_popedQueue.swap(_pushedQueue);
		}
	}
	if (_popedQueue.isEmpty()) {
		return EventPtr();
	} else {
		EventPtr evt = _popedQueue.front();
		_popedQueue.pop_front();
		return evt;
	}
}

bool EventQueue::isEmpty() const {
	if (!_popedQueue.isEmpty()) {
		return false;
	}
	QMutexLocker l(&_mutex);
	return _pushedQueue.isEmpty();
}
