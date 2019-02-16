
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
#include "AbstractConfigParams.h"
//#include "MtsMode.h"
#include <QtCore/QProcessEnvironment>
#include "base/MtsUtils.h"

namespace mts
{
	AbstractConfigParams::AbstractConfigParams()
		:_instanceId(999), _strategyPosCheck(0)
	{
	}
	AbstractConfigParams::~AbstractConfigParams() 
	{

	}

	bool AbstractConfigParams::load(const QVariantMap & params) {
		//merge system environment
		QVariantMap allParams;
		auto envirs=QProcessEnvironment::systemEnvironment();
		QStringList envirkeys = envirs.keys();
		for (int i = 0, size = envirkeys.size(); i < size; ++i) {
			const QString& key = envirkeys[i];
			allParams.insert(key, envirs.value(key));
		}
		
		for (auto it = params.constBegin(), itEnd = params.constEnd(); it != itEnd; ++it) {
			allParams.insert(it.key(), it.value());
		}

		allParams[MTS_VERSION] = "mts_" + MtsUtils::getBuildDateStr();
#ifdef _WIN32
		allParams[MTS_OS] = "Windows";
#else
		allParams[MTS_OS] = "Linux";
#endif
		
		if (doLoad(allParams)) {
			_allValues = allParams;
			return true;
		} else {
			return false;
		}
	}

	bool AbstractConfigParams::doLoad(const QVariantMap & params) {
		QString modeStr = params.value(MODE_PARAM_NAME).toString();
		_mode = environmentMode(qPrintable(modeStr));
		if (_mode == ENVIR_UNKNOWN) {
			MTS_ERROR("Unknown mode string '%s'\n",qPrintable(modeStr));
			return false;
		}
		_strategyPosCheck = params.value(STRATEGY_POS_CHECK, 0).toInt();
		_instanceId = params.value(INSTANCE_ID, (_mode == ENVIR_SIMU) ? 888 : -1).toInt();
		//const int defaultInstanceId=MtsMode::getInstance(_mode)->defaultInstanceId();
		//_instanceId = params.value(INSTANCE_ID, defaultInstanceId).toInt();
		if (_instanceId > 0 && _instanceId <= 999) {
			return true;
		} else {
			MTS_ERROR("Invalid instance id , it should be 1~999\n");
			return true;
		}
	}

	EnvironmentMode AbstractConfigParams::mode() const {
		return _mode;
	}

	int AbstractConfigParams::instanceId() const {
		return _instanceId;
	}
	
	int AbstractConfigParams::strategyPosCheck() const
	{
		return _strategyPosCheck;
	}

	QVariant AbstractConfigParams::value(const QString & key, const QVariant & defaultValue) const {
		return _allValues.value(key,defaultValue);
	}

	QStringList AbstractConfigParams::keys() const {
		return _allValues.keys();
	}

	bool AbstractConfigParams::isSimu() const {
		return _mode== ENVIR_SIMU;
	}


}