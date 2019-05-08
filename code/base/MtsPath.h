
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
#include <QtCore/QString>
#include <boost/function.hpp>
#include "base/base_api.h"
#include "MtsType.h"


#define DAILY_BAR_INTERVAL 86400000  //millisecs

class BASE_API MtsPath
{
public:
	static QString appDirPath();   //return exe or script path if running on python

	static QString mtsDirPath(bool hasColon = true);   //return mts.dll path. it is appDirPath() if the app start from exe else return scriptDirPath()/mts while start from script
	static QString configDirPath();
	static QString testDirPath();
	static QString docDirPath();
	static QString logDirPath();
	
	static QString metaInfoFilePath(mts::InstrumentType, int date, const QString& dataType = "fmd", const QString& dataDirPath = MtsPath::dataDirPath());

	static QString fmdDirPath(mts::InstrumentType, const QString& dataDirPath = MtsPath::dataDirPath());
	static QStringList fmdFilePath(mts::InstrumentType , int date , const QString& symbol,const QString& dataDirPath = MtsPath::dataDirPath());
	enum BarInterval {BI_15M=15,BI_60M=60,BI_DAILY= DAILY_BAR_INTERVAL};
	static QString bkdDirPath(mts::InstrumentType,int sec, const QString& dataDirPath = MtsPath::dataDirPath());
	static QString bkdFilePath(mts::InstrumentType , int sec,int date, const QString& symbol, const QString& dataDirPath = MtsPath::dataDirPath());


	static QString mtsScriptsDirPath ();
	static QString workspaceDirPath();
	static QString searchDirPath(const QString& basePath, const QString& dirName);
	static QString dataDirPath();
public:
	static void setScriptDirPath(const QString&);  //just be called by python wrap
private:
	static QString searchFilePath(const QString& basePath, const QString& dirName, const QString& fileName);
	static QString searchPath(const QString& basePath, const QString& dirName, const boost::function<bool(const QString&)>& isMatch);
	static QString _scriptDirPath;
};

