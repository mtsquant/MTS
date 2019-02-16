
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
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QList>
#include "Event.h"


//EventQueue for multi-writers and single reader
class BASE_API EventQueue 
{
public:
	EventQueue();
	~EventQueue();

	static EventQueue* defaultQueue();

	//just push threads call the push()
	void  push(const EventPtr& evt);

	//the following functions shoule be called by pop thread
	EventPtr pop();
	bool isEmpty() const;

private:
	QList<EventPtr>  _pushedQueue;
	QList<EventPtr>  _popedQueue;
	mutable QMutex _mutex;



};

