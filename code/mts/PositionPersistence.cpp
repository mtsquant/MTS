
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
#include "PositionPersistence.h"
#include "mts_core/Position.h"
#include "mts_core/Environment.h"

#include <QtCore/QDir>

namespace mts
{
	PositionPersistence::PositionPersistence(const QString& strategyKey, const QString& dir)
		:_strategyKey(strategyKey), _dir(dir), _write(nullptr), _tradingDay(0)
	{

	}

	PositionPersistence::~PositionPersistence()
	{
		closeWrite();
	}

	QList<Position*> PositionPersistence::getYesterdayPos() const
	{
		QString filename = getYesterdayFile();
		if (filename.isEmpty())
		{
			return QList<Position*>();
		}
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return QList<Position*>();
		}

		QList<Position*> rets;
		while (!file.atEnd())
		{
			QByteArray jsonStr = file.readLine();
			if (jsonStr.isEmpty()) {
				continue;;
			}
			QJsonParseError jsonError;
			QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr, &jsonError);
			if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject()) {
				auto obj = jsonDoc.object();
				rets.append(Position::fromJsonString(obj));
			}
		}
		return rets;
	}

	void PositionPersistence::updateTodayPosition(Position* pos)
	{
		int tradingday = Environment::instance()->clock()->currentTradingDay();
		if (_tradingDay != tradingday) {
			closeWrite();
			_tradingDay = tradingday;
		}
		if (_write == nullptr) {
			_write = new QFile(QString("%1/%2_%3_pos.txt").arg(_dir).arg(_tradingDay).arg(_strategyKey));
			_write->open(QFile::Append);
		}
		if (_write->isOpen()) {
			_write->write(pos->toJsonString().toLocal8Bit());
			_write->write("\n");
			_write->flush();
		}
	}

	QString PositionPersistence::getYesterdayFile() const
	{
		int tradingday = Environment::instance()->clock()->currentTradingDay();
		QFileInfoList files = QDir(_dir).entryInfoList(QDir::Files, QDir::Name);
		QRegExp reg(QString("^\\d{8}_%1_pos.txt$").arg(_strategyKey));
		QString lastFile;
		for (auto it = files.constBegin(); it != files.constEnd(); ++it)
		{
			auto name = it->fileName();
			if (reg.exactMatch(name)) {
				int date = name.left(8).toInt();
				if (date < tradingday) {
					lastFile = it->absoluteFilePath();
					break;
				}
			}
		}
		return lastFile;
	}

	void PositionPersistence::closeWrite()
	{
		if (_write) {
			_write->close();
			delete _write;
			_write = nullptr;
		}
	}

};