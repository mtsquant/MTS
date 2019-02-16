
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
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonObject>
#include "base/base_api.h"

#ifndef utf8Printable
#  define utf8Printable(string) string.toUtf8().constData()
#endif

class BASE_API MtsUtils
{
public:
	static QByteArray toUtf8(const char* str);
	static QString getUserName();
	static QString getLogFilePath(const QString& logPath, const QString& prefix);

	static bool touchFile(const QString& filePath);
	static void listFiles(const QString& path, QStringList& files);
	static QFileInfoList getMtsLogs(const QString& logDir);

	static QString getLastErrorMessage ();
	static QString getSymbolFromFileName(const QString&);
	static bool parseJsonFileAsObject(const QString& filePath, QJsonObject& jsonObj);
	static QJsonObject str2Json(const QString& str);
	static QString getBuildDateStr();
};
