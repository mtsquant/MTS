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
#include "SysConfigParams.h"
#include "base/MtsPath.h"
#include "mts_core/const.h"
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

namespace mts
{


	SysConfigParams::SysConfigParams() 
		:AbstractConfigParams()
		, _configDir(MtsPath::configDirPath())
		, _docDir(MtsPath::docDirPath())
		,_mtsDir(MtsPath::mtsDirPath())
		, _continuousContractFile(QString("%1/%2").arg(MtsPath::configDirPath()).arg(CONTINUOUS_CONTRACT_FILE))
		, _riskSettingFile(QString("%1/%2").arg(MtsPath::configDirPath()).arg(RISK_SETTING_FILE))
	{
		if (!QDir(MtsPath::logDirPath()).exists()) {
			QDir().mkpath(MtsPath::logDirPath());
		}
		_logFile = QString("%1/mts_%2_%3.log")
			.arg(MtsPath::logDirPath())
			.arg(instanceId())
			.arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz"));
	}


	SysConfigParams::~SysConfigParams() 
	{
	}

	bool SysConfigParams::doLoad(const QVariantMap & params) {
		if (!AbstractConfigParams::doLoad(params)) {
			return false;
		}
		_configDir = params.value(CONFIG_DIR, MtsPath::configDirPath()).toString();
		_docDir = params.value(DOC_DIR, MtsPath::docDirPath()).toString();
		_mtsDir = params.value(LIB_DIR, MtsPath::mtsDirPath()).toString();

		_continuousContractFile= params.value(CONTINUOUS_CONTRACT, QString("%1/%2").arg(_configDir).arg(CONTINUOUS_CONTRACT_FILE)).toString();

		_riskSettingFile = params.value(RISK_SETTING, QString("%1/%2").arg(_configDir).arg(RISK_SETTING_FILE)).toString();

		if (!params.contains(LOG_FILE)) {
			if (!QDir(MtsPath::logDirPath()).exists()) {
				QDir().mkpath(MtsPath::logDirPath());
			}
			_logFile = QString("%1/mts_%2_%3.log")
				.arg(MtsPath::logDirPath())
				.arg(instanceId())
				.arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz"));
		} else {
			QString logDirOrFile = params.value(LOG_FILE).toString();
			if (QFileInfo(logDirOrFile).isDir() || (!QFile::exists(logDirOrFile) && !logDirOrFile.endsWith(".log"))) {
				QDir().mkpath(logDirOrFile);
				QString fileName = QString("mts_%1_%2.log").arg(instanceId()).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz"));
				_logFile = logDirOrFile + "/" + fileName;
			} else {
				_logFile = logDirOrFile;
			}

		}
		return true;
	}

	QString SysConfigParams::configDir() const {
		return _configDir;
	}

	QString SysConfigParams::docDir() const {
		return _docDir;
	}

	QString SysConfigParams::mtsDir() const {
		return _mtsDir;
	}

	QString SysConfigParams::logFile() const {
		return _logFile;
	}

	QString SysConfigParams::continuousContractFile() const {
		return _continuousContractFile;
	}

	QString SysConfigParams::instrumentDbFile() const {
		return _configDir + "/" INSTRUMENT_DB_FILE;
	}

	QString SysConfigParams::riskSettingFile() const {
		return _riskSettingFile;
	}

	QString SysConfigParams::logDir() const {
		return QFileInfo(_logFile).absolutePath();
	}



}