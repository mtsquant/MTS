
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
#include "./MtsMainThread.h"
#include "base/MtsLog.h"
#include "mts_core/Environment.h"
#include "mts_core/ContinuousSymbolMgr.h"
#include "mts_core/ConfigParams.h"



namespace mts
{

	QMap<EnvironmentMode, ThreadCreator>* threadCreatorMap() {
		static QMap<EnvironmentMode, ThreadCreator> threadCreatorMap;
		return &threadCreatorMap;
	}

	int MtsMainThread::registerThread(EnvironmentMode mode, const ThreadCreator& c) {
		auto* map = threadCreatorMap();
		map->insert(mode,c);
		return map->size();
	}


	MtsMainThread * MtsMainThread::instance(EnvironmentMode mode) {
		static MtsMainThread * mtsThread = nullptr;
		if (mtsThread == nullptr) {
			auto func=threadCreatorMap()->value(mode);
			if (!func) {
				MTS_ERROR("Not support mode '%s'\n", environmentModeName(mode));
				return nullptr;
			}
			mtsThread = func();
			assert(mtsThread);
		}
		return mtsThread;
	}

	bool MtsMainThread::init(const QVariantMap & params, const InitFuntorInThread & initFunInThread) {
		_initFunInThread = initFunInThread;
		return true;
	}

	bool MtsMainThread::initMgrs(const QVariantMap & params) {
		assert(ConfigParams::isInitialized());

        ThreadNameRegister::instance()->registerCurrentThread("MtsThread");
		Environment::instance();//should be initialize first


		auto continuousContractFile = ConfigParams::instance()->continuousContractFile();
		if (!ContinuousSymbolMgr::instance()->init(continuousContractFile)) {
			MTS_WARN("Failed to init ContinuousSymbolMgr by reading '%s'\n",qPrintable(continuousContractFile));
		}

		if (_initFunInThread) {
			_initFunInThread();
		}
		return true;
	}

}
