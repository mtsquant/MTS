
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
#include "LogFixtureMgr.h"
#include <assert.h>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMutexLocker>
#include "MtsPath.h"
#include "MtsUtils.h"
#include "MtsLog.h"

static FILE* openLogFile(const QString& logPath, const QString& prefix = QString()) {
	auto filePath = MtsUtils::getLogFilePath(logPath, prefix);
	QDir fileDir = QFileInfo(filePath).absoluteDir();
	if (!fileDir.mkpath(fileDir.path())) {
		fprintf(stderr, "Failed to make dir:'%s'\n", qPrintable(fileDir.path()));
		exit(1);
	}
	FILE* fp = fopen(qPrintable(filePath), "a");
	if (!fp) {
		fprintf(stderr, "Failed to open the log file:'%s'\n", qPrintable(filePath));
		exit(1);
	}
#ifndef MTS_UNIT_TEST
	if (MTS_LOG_LEVEL() >= 2) {
		printf("Open log file :'%s'\n", qPrintable(filePath));
	}
#endif
	return fp;
}

LogFixtureMgr * LogFixtureMgr::instance(const QString & logPath) { //logPath can be dir/file
	static LogFixtureMgr mgr(logPath);
	return &mgr;
}

FILE * LogFixtureMgr::getLogFile() {
	return _defaultLogFile;
}

FILE * LogFixtureMgr::getLogFile(const QString & prefix) {
	QMutexLocker l(&_mutex);

	assert(!prefix.isEmpty());
	FILE*& f = _logFiles[prefix];
	if (f == nullptr) {
		f = openLogFile(_logPath, prefix);
	}
	return f;
}

bool LogFixtureMgr::closeLogFile(const QString & prefix) {
	QMutexLocker l(&_mutex);
	FILE*& f = _logFiles[prefix];
	if (f != nullptr) {
		fclose(f);
	} else {
		return false;
	}
	_logFiles.remove(prefix);
	return true;
}

LogFixtureMgr::LogFixtureMgr(const QString & logPath)
	:_logPath(logPath), _defaultLogFile(nullptr) {
	assert(!logPath.isEmpty());
	QMutexLocker l(&_mutex);
	_defaultLogFile = openLogFile(_logPath);
}
