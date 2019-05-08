
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
#include "./MtsMainThread.h"
#include <QtCore/QAtomicInt>
#include <QtCore/QThread>
#include <QtCore/QMutex>


namespace mts
{

	class MtsMainThreadReal :public MtsMainThread, protected QThread {
	public:
		MtsMainThreadReal();
		virtual ~MtsMainThreadReal();
		virtual bool init(const QVariantMap & params, const InitFuntorInThread& initFunInThread) override;
		virtual void enterEventLoop() override;
		virtual void exitEventLoop() override;

	private:
		void waitForEnterEventLoop();

	private:
		virtual void run() override;
		QMutex _enterEventLoopMutex;
		QVariantMap _params;
		bool _initDone;
		QAtomicInt _quitFlag;
	};


}