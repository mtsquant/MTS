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
#include <QtCore/QString>
#include <QtCore/QObject>
#include "base/base_api.h"
#ifdef _WIN32
#include <windows.h>

class BASE_API Library
{
public:
	Library ( QObject *parent = Q_NULLPTR );
	Library(const QString &fileName, QObject *parent = Q_NULLPTR);
	~Library();

	QString errorString() const;
	QString fileName() const;
	bool isLoaded() const;
	bool load();
	QFunctionPointer resolve(const char* symbol);
	void setFileName(const QString&);
	bool unload ();

private:
	QString _fileName;
	QString _errorString;
	HMODULE _dllHandle;
};

#else
#include <QtCore/QLibrary>

typedef QLibrary Library;

#endif