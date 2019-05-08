
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
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QTime>
#include <QtCore/QByteArray>
#include <stdio.h>
#include <stdlib.h>
#include "base/base_api.h"

#ifdef NDEBUG
#endif


class BASE_API ThreadNameRegister
{
public:
    static ThreadNameRegister* instance();
    void registerThread(qlonglong threadId, const QString& threadName);
    qlonglong registerCurrentThread(const QString& threadName);
    QString findThreadName(qlonglong threadid);
    QString currentThreadName();
private:
    QMutex _mutex;
    QHash<qlonglong, QString> _threadNameMap;
};


#define LOGEX(format) "[%s@%s] " format,__FUNCTION__,qPrintable(ThreadNameRegister::instance()->currentThreadName())

void BASE_API SET_MTS_LOG_LEVEL(int);  //DEBUG/INFO/WARNING level - 2: log to screen and file (default) ; 1: only to file ; 0: non output
int BASE_API MTS_LOG_LEVEL();

BASE_API void MTS_SET_FP_PATH ( const char* logFilePath );  //if logFilePath is a dir,write log file into the dir. 

BASE_API FILE *MTS_FP();

#define TIMESTAMP(format) "[%s] " format , qPrintable(QTime::currentTime().toString("hh:mm:ss.zzz"))

BASE_API FILE *MTS_FP_PREFIX(const char* prefix);

#ifdef MTS_LOG_DISABLED
    #ifdef _WIN32
        #define MTS_LOG __noop
        #define MTS_WARN __noop
        #define MTS_ERROR __noop
        #define MTS_FILE __noop
        #define MTS_FILE_RAW __noop
        #define MTS_DEBUG __noop
        #define MTS_LOG_FILE __noop
    #else
        #define MTS_LOG
        #define MTS_WARN
        #define MTS_ERROR
        #define MTS_FILE
        #define MTS_FILE_RAW
        #define MTS_DEBUG
        #define MTS_LOG_FILE
    #endif
#else
#define MTS_LOG(format, ...) qInfo(LOGEX(format),##__VA_ARGS__)
#define MTS_WARN(format, ...) qWarning(LOGEX(format),##__VA_ARGS__)
#define MTS_ERROR(format, ...) qCritical(LOGEX(format),##__VA_ARGS__)
#define MTS_FILE(format, ...) qInfo(LOGEX(format),##__VA_ARGS__)
#define MTS_FILE_RAW(format, ...) \
	fprintf(MTS_FP(),format,##__VA_ARGS__);\
	fflush(MTS_FP());

#define MTS_LOG_FILE(prefix,format,...) \
	fprintf(MTS_FP_PREFIX(prefix),format,##__VA_ARGS__);\
	fflush(MTS_FP_PREFIX(prefix));

#ifndef NDEBUG
#define MTS_DEBUG(format, ...)  qDebug(format,##__VA_ARGS__)
#else
#ifdef _WIN32
#define MTS_DEBUG __noop
#else
#define MTS_DEBUG
#endif
#endif
#endif
#define MTS_PERF_FILE(format,...) \
	fprintf(MTS_FP_PREFIX("perf"),format,##__VA_ARGS__);\
	fflush(MTS_FP_PREFIX("perf"));


typedef void(*LogToFixtureFunc)(const QString&);

BASE_API LogToFixtureFunc setLogFixtureHook(QtMsgType type, LogToFixtureFunc hook);

BASE_API  void clearLogFixtureHook(QtMsgType type);

