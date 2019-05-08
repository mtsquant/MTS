
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

class QuerySettlementInfoAction:public QueryAction<CThostFtdcSettlementInfoConfirmField, CThostFtdcSettlementInfoConfirmField>
{
	Q_OBJECT
public:
	QuerySettlementInfoAction(const CThostFtdcSettlementInfoConfirmField& qrySettlementField)
		:QueryAction(qrySettlementField, &CThostFtdcTraderApi::ReqSettlementInfoConfirm) 
	{
	}
	virtual ~QuerySettlementInfoAction() {}

Q_SIGNALS:
	void responseQuerySettlementInfo(const QList<CThostFtdcSettlementInfoConfirmField>& settlementInfos);

public :
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {
		this->onResponse(pSettlementInfo, pRspInfo, bIsLast);
	}
protected:
	virtual void emitResponseQueryResults(const QList<CThostFtdcSettlementInfoConfirmField>& results) override {
		Q_EMIT responseQuerySettlementInfo(results);
	}

};


Q_DECLARE_METATYPE(CThostFtdcSettlementInfoConfirmField);
Q_DECLARE_METATYPE(QList<CThostFtdcSettlementInfoConfirmField>);

