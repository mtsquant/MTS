
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
#include "AbstractConfigParams.h"
#include <QtCore/QProcessEnvironment>
#include "base/MtsUtils.h"
#include <assert.h>

namespace mts
{


	void addSysEnvirVarsTo(QVariantMap* params) {
		auto envirs = QProcessEnvironment::systemEnvironment();
		QStringList envirkeys = envirs.keys();
		for (int i = 0, size = envirkeys.size(); i < size; ++i) {
			const QString& key = envirkeys[i];
			params->insert(key, envirs.value(key));
		}

		params->insert(MTS_VERSION, "mts_" + MtsUtils::getBuildDateStr());
		params->insert(MTS_OS,
#ifdef _WIN32
			"Windows"
#else
			"Linux"
#endif
		);
	}

	void copySingleFieldsTo(const QVariantMap& src, QVariantMap* target) {
		for (auto i = src.constBegin(); i != src.constEnd(); ++i) {
			if (i.value().type() != QVariant::Map && !target->contains(i.key())) {
				target->insert(i.key(), i.value());
			}
		}
	}

	QVariantMap AbstractConfigParams::expendAllSections(const QVariantMap& params) {
		QVariantMap wholeParams = params;
		addSysEnvirVarsTo(&wholeParams);
		for (auto i = wholeParams.begin(); i != wholeParams.end(); ++i) {
			if (i.value().type() == QVariant::Map) {
				QVariantMap sectionParams = i.value().toMap();
				copySingleFieldsTo(wholeParams, &sectionParams);
				wholeParams[i.key()] = sectionParams;
			}
		}
		return wholeParams;
	}


	AbstractConfigParams::AbstractConfigParams()
		:_instanceId(999), _strategyPosCheck(0), _positionPersistenceEnable(false)
	{
	}
	AbstractConfigParams::~AbstractConfigParams() 
	{

	}

	bool AbstractConfigParams::load(const QVariantMap & params) {

		QVariantMap wholeParams = AbstractConfigParams::expendAllSections(params);
#ifndef NDEBUG
#ifndef MTS_UNIT_TEST
		assert(wholeParams.size() == params.size());
#endif
#endif

		if (doLoad(wholeParams)) {
			_allValues = wholeParams;
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
		_positionPersistenceEnable = params.value(POS_PERSISTENCE_ENABLE, 0).toInt();
		_instanceId = params.value(INSTANCE_ID, (_mode == ENVIR_SIMU) ? 888 : -1).toInt();
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

	EnvironmentMode AbstractConfigParams::componentMode(MtsComponent c) const {
		if (_mode == ENVIR_REAL) {
			return ENVIR_REAL;
		} else if (_mode == ENVIR_SIMU) {
			return ENVIR_SIMU;
		} else if (_mode == ENVIR_BETA) {
			if (c == MTS_COMP_CLOCK || c == MTS_COMP_FEEDS) {
				return ENVIR_REAL;
			} else {
				return ENVIR_SIMU;
			}
		} else {
			return ENVIR_UNKNOWN;
		}
	}

	int AbstractConfigParams::instanceId() const {
		return _instanceId;
	}
	
	int AbstractConfigParams::strategyPosCheck() const
	{
		return _strategyPosCheck;
	}

	bool AbstractConfigParams::positionPersistenceEnable() const
	{
		return _positionPersistenceEnable;
	}

	QVariant AbstractConfigParams::value(const QString & key, const QVariant & defaultValue) const {
		return _allValues.value(key,defaultValue);
	}

	QStringList AbstractConfigParams::keys() const {
		return _allValues.keys();
	}

	QVariantMap AbstractConfigParams::rawParams() const {
		return _allValues;
	}



}