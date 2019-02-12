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
#include "OrderIdImplCtp.h"
#include "base/MtsLog.h"


#define REF_INDEX_MAX 100000

namespace mts
{
	OrderIdImplCtp::OrderIdImplCtp() 
		:_frontId(0), _sessionId(0), _refId(0)
	{
	}

	OrderIdImplCtp::OrderIdImplCtp(int frontId, int sessionId, int instanceId, int strategyId, int orderType,int index)
		:_sessionId(sessionId),_frontId(frontId)
	{
		int pid = instanceId*REF_INDEX_MAX;
		//<13  because the ctp reference id is 'typedef char TThostFtdcOrderRefType[13];'
		//3 bytes instance id +5bytes inc count+2 byte millisecond (mm)+1 byte orderType +1 byte strategy id
		if (index >= REF_INDEX_MAX - 1) {
			MTS_ERROR("Too many orders , should be less than %d\n", REF_INDEX_MAX);
			return ;//��ʱ���ܳ���99999��order ��10�����
		}
		int msec = QTime::currentTime().msec() % 100;
		_refId = ((qint64(pid + index)) * 1000 + msec * 10 + orderType) * 10 + strategyId;
	}
	OrderIdImplCtp::OrderIdImplCtp(int frontId , int sessionId , qint64 referenceId )
		:_sessionId(sessionId),_frontId(frontId), _refId(referenceId)
	{
	}

	OrderIdImplCtp::OrderIdImplCtp(const mts::OrderId& id)
	{
		fromString(id.toString());
	}

	OrderIdImplCtp::~OrderIdImplCtp()
	{
	}

	QString OrderIdImplCtp::ctpOrderReferenceId() const {
		return QString::number(_refId);
	}

	mts::OrderId OrderIdImplCtp::id() const
	{
		return mts::OrderId(toString());
	}

	int OrderIdImplCtp::orderType() const {
		return _refId % 100 / 10;
	}

	int OrderIdImplCtp::instanceId() const {
		return _refId / 1000000000;;
	}

	int OrderIdImplCtp::strategyId() const {
		return  _refId % 10;
	}

	bool OrderIdImplCtp::isMtsOrder() const {
		if (!isInstanceIdValid(this->instanceId())) {
			return false;
		}
		if (!isStrategyIdValid(this->strategyId())) {
			return false;
		}
		return orderType() != OT_UNKNOWN;
	}

	QString OrderIdImplCtp::toString() const {
		return QString("%1_%2_%3").arg(_frontId).arg(uint(_sessionId)).arg(_refId);
	}

	bool OrderIdImplCtp::fromString(const QString & idStr) { //MTS just recap orders
		_frontId = idStr.section('_', 0, 0).toInt();
		_sessionId= idStr.section('_', 1, 1).toInt();
		_refId = idStr.section('_', 2, 2).toLongLong();
		return _refId!=0;
	}

}