
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
#include "./MtsMainThreadReal.h"
#include <QtCore/qcoreapplication.h>
#include "base/EventDispatcher.h"
#include <QtCore/QAbstractEventDispatcher>

namespace mts
{

	static int threadIdReal = MtsMainThread::registerThread(ENVIR_REAL, []() {
		return new MtsMainThreadReal();
	}); 

	static int threadIdBeta = MtsMainThread::registerThread(ENVIR_BETA, []() {
		return new MtsMainThreadReal();
	}); 

		
	MtsMainThreadReal::MtsMainThreadReal()
		:QThread(),/* _evtLoopRunner(nullptr),*/ _initDone(false), _quitFlag(0) {
		_enterEventLoopMutex.lock();
	}


	MtsMainThreadReal::~MtsMainThreadReal() {
		exitEventLoop();
	}


	bool MtsMainThreadReal::init(const QVariantMap & params, const InitFuntorInThread& initFunInThread) {
		MtsMainThread::init(params, initFunInThread);
		_params = params;
		start();
		waitForEnterEventLoop();
		return _initDone;
	}

	//mts main function
	void MtsMainThreadReal::run() {

		_initDone = MtsMainThread::initMgrs(_params);
		_enterEventLoopMutex.unlock();
		//enter thread event loop, so the signal and timer can be work well
		//this->exec();
		auto * qtEventDispatcher = this->eventDispatcher();
		auto * mtsEventDispatcher = EventDispatcher::defaultDispatcher();
		while (_quitFlag == 0) {
			qtEventDispatcher->processEvents(QEventLoop::AllEvents);
			mtsEventDispatcher->processEvents();

		}
		_enterEventLoopMutex.lock();
	}

	void MtsMainThreadReal::enterEventLoop() {

	}

	void MtsMainThreadReal::exitEventLoop() {
		//TODO: exit evt loop runner
		//if (_evtLoopRunner) {
		//	_evtLoopRunner->exit();
		//}
		this->quit();
		_quitFlag = 1;
		this->wait();
	}

	//void MtsMainThreadReal::runEventLoopRunner() {
	//	assert(false);
	//}



	void MtsMainThreadReal::waitForEnterEventLoop() {
		_enterEventLoopMutex.lock();
		_enterEventLoopMutex.unlock();
	}

}