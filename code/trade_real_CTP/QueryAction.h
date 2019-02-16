
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
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "AbstractAction.h"

template<typename _CTPQryField,typename _CTPRspField>
class QueryAction :public AbstractAction
{
public:

	template<typename _CTPQryFun>
	QueryAction(const _CTPQryField& qryField, _CTPQryFun qryFun)
        :AbstractAction()
	{
        memcpy(&_qryField,&qryField,sizeof(_CTPQryField));
		_requestFunction = boost::bind(qryFun, _1, &_qryField, _2);
	}

	virtual ~QueryAction() {
	}

protected:
	virtual int doRequest(CThostFtdcTraderApi* tradeApi, int requestId) override {
		_rspFieldList.clear();
		return _requestFunction(tradeApi, requestId);
	}

	virtual void onProcessResult(int actionId, const QByteArray& data) override {
		switch (actionId) {
		case 1:
			emitActionError((CThostFtdcRspInfoField*)data.data(), OL_FRONT);
			break;
		case 2:
			_rspFieldList.append(*(_CTPRspField*)data.data());
			break;
		case 3:
			emitResponseQueryResults(_rspFieldList);
			break;
		default:
			return ;
		};
	}

	void onResponse(_CTPRspField* rspField, CThostFtdcRspInfoField* pRspInfo, bool bIsLast) {
		if (pRspInfo && pRspInfo->ErrorID != 0) {
			QMetaObject::invokeMethod(this, "onProcessResult", 
				Q_ARG(int,1),
				Q_ARG(QByteArray, QByteArray((char*)pRspInfo, sizeof(CThostFtdcRspInfoField)))
			);
			return;
		} 
		if (rspField) {
			QMetaObject::invokeMethod(this, "onProcessResult",
				Q_ARG(int, 2),
				Q_ARG(QByteArray, QByteArray((char*)rspField, sizeof(_CTPRspField)))
			);
		}
		if (bIsLast) { // if no result, CTP will callback with nullptr but the bIsLast is true
			QMetaObject::invokeMethod(this, "onProcessResult",
				Q_ARG(int, 3),
				Q_ARG(QByteArray, QByteArray())
			);
		}
	}
	virtual void emitResponseQueryResults(const QList<_CTPRspField>& results) = 0;
protected:
	_CTPQryField _qryField;
private:
	boost::function<int(CThostFtdcTraderApi*, int)> _requestFunction;
	QList<_CTPRspField>  _rspFieldList;
};

