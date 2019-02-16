
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
#include <QtCore/QVariant>
#include <boost/function.hpp>
#include "mts_core/enums.h"

namespace mts
{
	class MtsMainThread;
	typedef boost::function<void()> InitFuntorInThread;
	typedef boost::function<MtsMainThread* ()>  ThreadCreator;
	class MtsMainThread
	{
	public:
		static MtsMainThread* instance(EnvironmentMode mode);
		virtual bool init(const QVariantMap & params,const InitFuntorInThread& initFunInThread) ;
		virtual void enterEventLoop() = 0;
		virtual void exitEventLoop()=0;
		//virtual void runEventLoopRunner() = 0;
	protected:
		bool initMgrs(const QVariantMap & params);

	private:
		InitFuntorInThread _initFunInThread;

	public:
		static int registerThread(EnvironmentMode mode, const ThreadCreator&);

	};





}

