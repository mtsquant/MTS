
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
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include "mts_core/const.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/Environment.h"
#include "mts/mts_api.h"


namespace mts
{

	class Position;
	class PosInnerMgr;
	//NOTS: the class should not be export
	class MTS_API PositionMgr:public QObject
	{
		Q_OBJECT
	public:
		static PositionMgr* instance();
		static QString getStrategyKey(int instanceId = Environment::instance()->instanceId(), int strategyId = DEFAULT_STRATEGY_ID);
		void businessDateChanged(int businessDate);
	public Q_SLOTS:
		Position* getPosition(const InstrumentId& id, GetMode = GM_GET);
		Position* getPosition(const InstrumentId& id,int instanceId, int strategyId, GetMode = GM_GET);
		QList<Position*> getStrategyPositions(const InstrumentId& id) const;
		void clear();
		QList<Position*> allPositions(int strategyId= DEFAULT_STRATEGY_ID) const;
		QList<Position*> allValidPositions(int strategyId = DEFAULT_STRATEGY_ID) const;
	private:
		PositionMgr();
		~PositionMgr();
		typedef QHash<QString/*strategyKey*/, PosInnerMgr*> PosHash;
		PosHash _posHash;
		static PositionMgr* _mgr;
	};

}