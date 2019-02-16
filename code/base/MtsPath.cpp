
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
#include "MtsPath.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include "MtsLog.h"

QString MtsPath::_scriptDirPath;

void MtsPath::setScriptDirPath(const QString & path) {
	_scriptDirPath = QDir::fromNativeSeparators(path);
    MTS_DEBUG("setScriptPath:%s\n",qPrintable(_scriptDirPath));
}

QString MtsPath::appDirPath() {
    const QString appDir= _scriptDirPath.isEmpty()? QCoreApplication::applicationDirPath(): _scriptDirPath;
    MTS_DEBUG("appDirPath:%s\n",qPrintable(appDir));
    return appDir;
}


QString MtsPath::mtsDirPath(bool hasColon) {
	QString path;
	if (_scriptDirPath.isEmpty()) {
		path=appDirPath();
	} else {
        path = searchDirPath(_scriptDirPath, "MTS");
#ifdef _WIN32
        QString dllSuffix="*.dll";
        QString pythonMtsLibPath = QCoreApplication::applicationDirPath()
#ifdef NDEBUG
			+ "/Lib/mts";
#else
			+ "/Lib/mts_d";
#endif
#else
        QString dllSuffix="*.so";
#ifdef NDEBUG
        QString pythonMtsLibPath = "/usr/lib/python3.6/mts";
#else
		QString pythonMtsLibPath = "/usr/lib/python3.6/mts_d";
#endif
#endif
        if (QDir(path).entryList({ dllSuffix }, QDir::Files).isEmpty()) {
            MTS_DEBUG("pythonMtsLibPath:%s\n",qPrintable(pythonMtsLibPath));

			if (QDir(pythonMtsLibPath).exists()) {
				path = pythonMtsLibPath;
			} 
		}
	}
    MTS_DEBUG("mtsDirPath:%s\n",qPrintable(path));

	if (!hasColon) {
		return path.remove(':');
	} else {
		return path;
	}
}


QString MtsPath::searchPath(const QString & basePath, const QString & dirName, const boost::function<bool(const QString&)>& isMatch) {
	QString path;
	QString parentPath;
	QString parent = "";
	do {
		parentPath = QDir::cleanPath(QString("%1/%2").arg(basePath).arg(parent));
		path = QString("%1/%2").arg(parentPath).arg(dirName);
		parent += "../";
	} while (!isMatch(path) && !QDir(parentPath).isRoot());

	if (!isMatch(path)) {
		return QString();
	} else {
		return path;
	}
}


QString MtsPath::searchDirPath(const QString& basePath, const QString & dirName) {
	return searchPath(basePath, dirName, [](const QString& path)
	{
		return QDir(path).exists();
	});
}

QString MtsPath::searchFilePath(const QString & basePath, const QString & dirName, const QString & fileName) {
	return searchPath(basePath, dirName, [fileName](const QString& path)
	{
		return QFile::exists(path+"/"+fileName);
	});
}

QString MtsPath::configDirPath() {
	return searchDirPath(appDirPath(),"config");
}

QString MtsPath::testDirPath(){
	return searchDirPath(appDirPath(), "test");
}

QString MtsPath::docDirPath() {
	return searchDirPath(appDirPath(), "doc");
}

QString MtsPath::logDirPath() {
	QString logPath;
	if (_scriptDirPath.isEmpty()) {
		logPath=searchDirPath(appDirPath(), "log");
		if (logPath.isEmpty()) {
			logPath=QDir::cleanPath(QString("%1/../log").arg(appDirPath()));
		}
	} else {
		logPath = QString("%1/log").arg(_scriptDirPath);
	}
	if (!QDir(logPath).exists()) {
		QDir().mkpath(logPath);
	}
	return logPath;
}

QString MtsPath::dataDirPath() {
	return searchDirPath(appDirPath(),"data");
}

QString MtsPath::fmdDirPath(mts::InstrumentType it, const QString& dataDirPath ) {
	return QString("%1/fmd/%2").arg(dataDirPath).arg(mts::instrumentTypeName(it));
}


#ifdef _WIN32
inline QString getFmdFilePath(const QString& path) {
	QString gzPath = path + ".gz";
	return QFile::exists(gzPath) ? gzPath : path;
}
#else
inline QString getFmdFilePath(const QString& pathPrefix, const QString & symbol, const QString& pathSuffix) {
	QString filePath = QString("%1/%2%3").arg(pathPrefix).arg(symbol).arg(pathSuffix);
	QString gzFilePath = filePath + ".gz";
	if (QFile::exists(gzFilePath)) {
		return gzFilePath;
	}
	if (QFile::exists(filePath)) {
		return filePath;
	}

	QString lowerFilePath = QString("%1/%2%3").arg(pathPrefix).arg(symbol.toLower()).arg(pathSuffix);
	QString lowerGzFilePath = lowerFilePath + ".gz";
	if (QFile::exists(lowerGzFilePath)) {
		return lowerGzFilePath;
	}
	if (QFile::exists(lowerFilePath)) {
		return lowerFilePath;
	}

	QString upperFilePath = QString("%1/%2%3").arg(pathPrefix).arg(symbol.toUpper()).arg(pathSuffix);
	QString upperGzFilePath = upperFilePath + ".gz";
	if (QFile::exists(upperGzFilePath)) {
		return upperGzFilePath;
	}
	if (QFile::exists(upperFilePath)) {
		return upperFilePath;
	}
	return gzFilePath;
}
#endif

QStringList MtsPath::fmdFilePath(mts::InstrumentType it , int date , const QString & symbol , const QString & dataDirPath /*= dataDirPath()*/){
#ifdef _WIN32
    QString pathPrefix=QString("%1/%2/fmd/%3/%4").arg(dataDirPath).arg(mts::instrumentTypeName(it)).arg(date).arg(symbol);
	QString singleFile = getFmdFilePath(QString("%1.fmd").arg(pathPrefix));
	if (QFile::exists(singleFile)) {
		return QStringList() << singleFile;
	}
    return QStringList() << getFmdFilePath(QString("%1_night.fmd").arg(pathPrefix))
						 << getFmdFilePath(QString("%1_day.fmd").arg(pathPrefix));

#else
	QString pathPrefix = QString("%1/%2/fmd/%3").arg(dataDirPath).arg(mts::instrumentTypeName(it)).arg(date);
	QString singleFile = getFmdFilePath(pathPrefix,symbol,".fmd");
	if (QFile::exists(singleFile)) {
		return QStringList() << singleFile;
	}
	return QStringList() << getFmdFilePath(pathPrefix, symbol, "_night.fmd")
						<< getFmdFilePath(pathPrefix, symbol, "_day.fmd");
#endif
}

QString MtsPath::metaInfoFilePath(mts::InstrumentType it, int date, const QString& dataType /*="fmd"*/, const QString& dataDirPath /*= dataDirPath()*/)
{
	QString globalMetaFile = QString("%1/%2/%3/.metainfo.json").arg(dataDirPath).arg(mts::instrumentTypeName(it)).arg(dataType);
	if (QFile::exists(globalMetaFile)) {
		return globalMetaFile;
	}
	return QString("%1/%2/%3/%4/.metainfo.json").arg(dataDirPath).arg(mts::instrumentTypeName(it)).arg(dataType).arg(date);
}

QString MtsPath::bkdDirPath(mts::InstrumentType it,int sec, const QString& dataDirPath ) {
	if (sec == BI_DAILY) {// daily bkd file
		return QString("%1/%2/bkd/daily").arg(dataDirPath).arg(mts::instrumentTypeName(it));
	} else { //bar bkd file 
		if (sec % BI_60M == 0) {
			return QString("%1/%2/bkd/60s").arg(dataDirPath).arg(mts::instrumentTypeName(it));
		} else if (sec % BI_15M == 0) {
			return QString("%1/%2/bkd/15s").arg(dataDirPath).arg(mts::instrumentTypeName(it));
		} else {
			return "";
		}
	}
}

QString MtsPath::bkdFilePath(mts::InstrumentType it , int sec , int date , const QString & symbol, const QString& dataDirPath){
	QString dirPath = MtsPath::bkdDirPath(it, sec,dataDirPath);
	if (dirPath.isEmpty()) {
		MTS_ERROR("Invalid bar interval (%d), it should be 15*n , 60*n or  %d \n", sec * 1000, DAILY_BAR_INTERVAL);
		return "";
	}
	QString bkdFilePath;
	if (sec == BI_DAILY) {
		if(it == mts::TYPE_FUTR){
			bkdFilePath = QString("%1/%2.bkd")
				.arg(dirPath)
				.arg(symbol)
				;
		} else{
			bkdFilePath = QString("%1/%2/%3.bkd")
				.arg(dirPath)
				.arg(date / 10000)
				.arg(symbol)
				;
		}
	} else {
		bkdFilePath = QString("%1/%2/%3.bkd")
			.arg(dirPath)
			.arg(date)
			.arg(symbol)
			;
	}
	if (!QFile::exists(bkdFilePath)) {
		bkdFilePath += ".gz";
	}
	return bkdFilePath;
}

QString MtsPath::mtsScriptsDirPath (){
	return searchDirPath ( appDirPath () , "scripts" );
}

QString MtsPath::workspaceDirPath() {
	return searchDirPath(appDirPath(), "workspace");
}


