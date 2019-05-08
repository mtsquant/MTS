
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

#include "QueryAction.h"

class QueryTradingAccountAction :public QueryAction<CThostFtdcQryTradingAccountField, CThostFtdcTradingAccountField>
{
	Q_OBJECT
public:
	QueryTradingAccountAction(const CThostFtdcQryTradingAccountField& qryTradingAccountField)
		:QueryAction(qryTradingAccountField, &CThostFtdcTraderApi::ReqQryTradingAccount)
	{
	}
	virtual ~QueryTradingAccountAction() {}

Q_SIGNALS:
	void responseQueryTradingAccount(const QList<CThostFtdcTradingAccountField>& settlementInfos);

public:
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {
		this->onResponse(pTradingAccount, pRspInfo, bIsLast);
	}
protected:
	virtual void emitResponseQueryResults(const QList<CThostFtdcTradingAccountField>& results) override {
		Q_EMIT responseQueryTradingAccount(results);
	}

};


Q_DECLARE_METATYPE(CThostFtdcTradingAccountField);
Q_DECLARE_METATYPE(QList<CThostFtdcTradingAccountField>);

