
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
class QFile;
namespace mts
{
	class Position;
	class PositionPersistence
	{
	public:
		PositionPersistence(const QString& strategyKey, const QString& dir);
		~PositionPersistence();
		QList<Position*> getYesterdayPos()const;
		void updateTodayPosition(Position*);
	private:
		QString getYesterdayFile() const;
		void closeWrite();
		const QString _strategyKey;
		const QString _dir;
		QFile* _write;
		int _tradingDay;
	};
}