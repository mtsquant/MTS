
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
#include "QueryAction.h"

class QueryFillAction :public QueryAction<CThostFtdcQryTradeField, CThostFtdcTradeField>
{
	Q_OBJECT
public:
	QueryFillAction(const CThostFtdcQryTradeField& qryTradeField)
		:QueryAction(qryTradeField, &CThostFtdcTraderApi::ReqQryTrade) 
	{
	}
	virtual ~QueryFillAction() {}

Q_SIGNALS:
	void responseQueryFills(const QList<CThostFtdcTradeField>& fills);

public:
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {
		this->onResponse(pTrade, pRspInfo, bIsLast);
	}
protected:
	virtual void emitResponseQueryResults(const QList<CThostFtdcTradeField>& results) override {
		Q_EMIT responseQueryFills(results);
	}

};

Q_DECLARE_METATYPE(CThostFtdcTradeField);
Q_DECLARE_METATYPE(QList<CThostFtdcTradeField>);
