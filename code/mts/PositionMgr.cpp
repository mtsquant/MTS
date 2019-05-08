
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
#include "PositionMgr.h"
#include "mts_core/Position.h"
#include "mts_core/Environment.h"
#include "base/MtsLog.h"
#include "MtsMainThread.h"
#include <assert.h>

namespace mts
{
	class PosInnerMgr
	{
	public:
		PosInnerMgr(const QString& id)
			:_id(id)
		{
		}
		~PosInnerMgr()
		{
			qDeleteAll(_posHash);
		}
		Position* getPosition(const InstrumentId& id, GetMode gm= GM_GET) 
		{
			PosHash::const_iterator it = _posHash.constFind(id);
			if (it != _posHash.constEnd()) {
				return it.value();
			}
			Position* pos = nullptr;
			if (gm == GM_GET_OR_CREATE) {
				pos = new Position(id);
				_posHash[id] = pos;
			}
			return pos;
		}
		void clear()
		{
			auto it = _posHash.begin();
			if (it != _posHash.end()) {
				delete it.value();
				it = _posHash.erase(it);
			}
		}
		QList<Position*> allPositions() const
		{
			return _posHash.values();
		}
		QList<Position*> allValidPositions() const
		{
			QList<Position*> positions;
			for (auto it = _posHash.constBegin(), itEnd = _posHash.constEnd(); it != itEnd; ++it) {
				Position* pos = it.value();
				if (!pos->isNull()) {
					positions << pos;
				}
			}
			return positions;;
		}
		void businessDateChanged(int businessDate)
		{
			for (auto it = _posHash.constBegin(), itEnd = _posHash.constEnd(); it != itEnd; ++it) {
				Position* pos = it.value();
				MTS_FILE("Position:%s\n", qUtf8Printable(pos->toJsonString()));
				pos->businessDateChanged(businessDate);
			}
		}
	private:
		typedef QHash<InstrumentId/*sym*/, Position*> PosHash;
		PosHash _posHash;
		const QString _id;
	};

	PositionMgr* PositionMgr::_mgr = nullptr;

	PositionMgr::PositionMgr()
	{
		assert(Environment::instance()->isCurrentMtsThread());
	}

	PositionMgr::~PositionMgr() 
	{
		qDeleteAll(_posHash);
	}

	PositionMgr* PositionMgr::instance() 
	{
		if (!_mgr) {
			_mgr = new PositionMgr();
		}
		return _mgr;
	}

	QString PositionMgr::getStrategyKey(int instanceId,int strategyId)
	{
		return QString("%1@%2").arg(instanceId).arg(strategyId);
	}

	void PositionMgr::businessDateChanged(int businessDate)
	{
		for (auto it = _posHash.constBegin(), itEnd = _posHash.constEnd(); it != itEnd; ++it) {
			PosInnerMgr* innerMgr = it.value();
			innerMgr->businessDateChanged(businessDate);
		}
	}

	Position* PositionMgr::getPosition(const InstrumentId& id, GetMode gm)
	{
		return getPosition(id,Environment::instance()->instanceId(),DEFAULT_STRATEGY_ID, gm);
	}

	mts::Position* PositionMgr::getPosition(const InstrumentId& id, int instanceId, int strategyId, GetMode gm/*= GM_GET*/)
	{
		if (instanceId<=0 ||(strategyId!= DEFAULT_STRATEGY_ID && (strategyId <= 0 || strategyId > 9))) {
			return nullptr;
		}
		QString key = getStrategyKey(instanceId, strategyId);
		PosInnerMgr* innerMgr = nullptr;
		auto it = _posHash.constFind(key);
		if (it == _posHash.constEnd()) {
			if (gm == GM_GET_OR_CREATE) {
				innerMgr = new PosInnerMgr(key);
				_posHash.insert(key, innerMgr);
			}
			else {
				return nullptr;
			}
		}
		else {
			innerMgr = it.value();
		}
		return innerMgr->getPosition(id,gm);
	}

	QList<Position*> PositionMgr::getStrategyPositions(const InstrumentId& id) const
	{
		QList<Position*> poss;
		for (auto& it : _posHash)
		{
			if (auto* pos = it->getPosition(id, GetMode::GM_GET)) {
				poss.push_back(pos);
			}
		}
		return poss;			
	}

	void PositionMgr::clear() {
		_posHash[getStrategyKey()]->clear();
	}

	QList<Position*> PositionMgr::allPositions(int strategyId) const
	{
		auto it = _posHash.constFind(getStrategyKey(Environment::instance()->instanceId(),strategyId));
		if (it == _posHash.constEnd()) {
			return QList<Position*>();
		}
		return it.value()->allPositions();
	}

	QList<Position*> PositionMgr::allValidPositions(int strategyId) const
	{
		auto it = _posHash.constFind(getStrategyKey(Environment::instance()->instanceId(), strategyId));
		if (it == _posHash.constEnd()) {
			return QList<Position*>();
		}
		return it.value()->allValidPositions();
	}


}