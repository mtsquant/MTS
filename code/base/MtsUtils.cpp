
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
#include "MtsUtils.h"
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QStandardPaths>
#include <QtCore/QUrl>
#include "MtsPath.h"
#include "MtsLog.h"
#include "base/enum_ext.h"
#ifdef _WIN32
#include <Windows.h>
#endif

#include <QtCore/QJsonParseError>
#include <QtCore/QJsonDocument>
#include <QtCore/QSysInfo>

QString MtsUtils::getUserName() {
	QString userName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	userName = userName.section("/", -1, -1);
	return userName;
}

QByteArray MtsUtils::toUtf8(const char* str)
{
	return LS(str).toUtf8();
}

QString MtsUtils::getLogFilePath(const QString & logPath, const QString & prefix) {
	QString logFileDir = logPath;
	if (logPath.isEmpty()) {
		logFileDir = MtsPath::logDirPath();
	}

	if (QFileInfo(logFileDir).isDir()) {
		QString fileName;
		if (prefix.isEmpty()) {
			fileName = QString("mts_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz"));
		} else {
			fileName = QString("mts_%1.log").arg(prefix);
		}
		return QString("%1/%2").arg(logFileDir).arg(fileName);

	} else { //logPath is a file path
		if (prefix.isEmpty()) {
			return logFileDir;
		} else {
			QString filePath = logPath;
			int idx = filePath.lastIndexOf('.');
			if (idx > 0) {
				filePath = filePath.insert(idx, QString("_") + prefix);
			} else {
				filePath += ("_" + prefix);
			}
			return filePath;
		}
	}
}

bool MtsUtils::touchFile(const QString & filePath) {
	if (!QFile::exists(filePath)) {
		QDir parentDir(filePath+"/..");
		if (!parentDir.exists()) {
			if (!parentDir.mkpath(".")) {
				return false;
			}
		}
	} 
	QFile f(filePath);
	if (!f.open(QFile::WriteOnly)) {
		return false;
	}

	f.close();
	return true;
}

QString MtsUtils::getLastErrorMessage (){
#ifdef _WIN32
	DWORD e = GetLastError ();
	LPVOID lpMsgBuf;
	FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS ,
					NULL , e , MAKELANGID ( LANG_NEUTRAL , SUBLANG_DEFAULT ) ,(LPTSTR)&lpMsgBuf , 0 , NULL );
	std::wstring errMsg = (LPTSTR)lpMsgBuf;
	LocalFree ( lpMsgBuf );
	return QString::fromStdWString( errMsg );
#else
	return strerror ( errno );
#endif
}

QString MtsUtils::getSymbolFromFileName(const QString & file) {
	if (file.endsWith(".gz", Qt::CaseInsensitive)) {
		return QFileInfo(file.left(file.size() - 3)).completeBaseName();
	} else {
		return QFileInfo(file).completeBaseName();
	}
}

void MtsUtils::listFiles(const QString& path, QStringList& files) {
	QDir dir(path);
	QFileInfoList infos = dir.entryInfoList();
	for (int i = 0, size = infos.size(); i < size; ++i) {
		const QFileInfo& info = infos[i];
		if (info.isFile()) {
			files.append(info.absoluteFilePath());
		}
		else {
			if (info.fileName() == "." || info.fileName() == "..") {
				continue;
			}
			listFiles(info.absoluteFilePath(), files);
		}
	}
}

QFileInfoList MtsUtils::getMtsLogs(const QString& logDir) {
	QDir dir(logDir);
	QFileInfoList fileInfos = dir.entryInfoList(QStringList() << "*.log", QDir::Files, QDir::Time);
	QFileInfoList mtsLogs;
	QString filePath;
	for (int i = 0, size = fileInfos.size(); i < size; ++i) {
		const QFileInfo& info = fileInfos[i];
		filePath = info.absoluteFilePath();
		if (!filePath.endsWith("MatchModel.log", Qt::CaseInsensitive)) {
			mtsLogs.append(info);
		}
	}
	return mtsLogs;
}


bool MtsUtils::parseJsonFileAsObject(const QString & filePath, QJsonObject & jsonObj)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		MTS_ERROR("can not open file: '%s'\n", qPrintable(filePath));
		return false;
	}
	QByteArray jsonStr = file.readAll();
	file.close();
	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr, &jsonError);
	if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
		MTS_ERROR("can not parse json file : '%s'\n", qPrintable(filePath));
		return false;
	}
	jsonObj = jsonDoc.object();
	return true;
}

QJsonObject MtsUtils::str2Json(const QString& str)
{
	QJsonParseError error;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(qPrintable(str), &error);
	if (error.error == QJsonParseError::NoError && !jsonDoc.isNull()) {
		return jsonDoc.object();
	}
	else {
		return QJsonObject();
	}
}

QString MtsUtils::json2Str(const QJsonObject& obj)
{
	return QJsonDocument(obj).toJson(QJsonDocument::Compact).simplified();
}

QString MtsUtils::getBuildDateStr()
{
	QStringList monthList = QStringList() << "" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" 
		<< "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
	QString monthStr = QString(__DATE__).left(3);
	int month = monthList.indexOf(monthStr);
	if (month < 0) {
		return __DATE__;
	}
	QString dateStr = __DATE__;
	dateStr.remove(0,3);
	dateStr.prepend(QString::number(month));
	QDate dt = QDate::fromString(dateStr, "M d yyyy");
	if (!dt.isValid()) {
		return __DATE__;
	}
	return dt.toString("yyyyMMdd");
}

QString MtsUtils::getMachineHostName()
{
	QString hostName = QSysInfo::machineHostName();
	if (hostName.isEmpty()) {
		hostName = "UNKNOWN";
	}
	return hostName;
}

QString MtsUtils::buildUrlQueryString(const QMap<QString, QString>& params)
{
	QStringList items;
	for (auto it = params.constBegin(),itEnd = params.constEnd(); it != itEnd; ++it) {
		items.append(it.key() + "=" + QUrl::toPercentEncoding(it.value()));
	}
	return items.join('&');
}

QDateTime MtsUtils::getUTCTimeFromISO8601(const QString& tmsp)
{
	QDateTime utcDt = QDateTime::fromString(tmsp, "yyyy-MM-ddTHH:mm:ss.zzzZ");
	utcDt.setTimeSpec(Qt::UTC);
	return utcDt;
}

qint64 MtsUtils::getTimeStampFromISO8601(const QString& tmsp)
{
	QDateTime utcDt = getUTCTimeFromISO8601(tmsp);
	return utcDt.toMSecsSinceEpoch();
}

QString MtsUtils::getISO8601TimeString(qint64 tmsp)
{
	QDateTime utcDt = QDateTime::fromMSecsSinceEpoch(tmsp);
	utcDt.setTimeSpec(Qt::UTC);
	return utcDt.toString("yyyy-MM-ddTHH:mm:ss.zzzZ");
}
