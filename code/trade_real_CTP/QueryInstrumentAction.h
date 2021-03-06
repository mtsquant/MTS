
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

class QueryInstrumentAction:public QueryAction<CThostFtdcQryInstrumentField, CThostFtdcInstrumentField>
{
	Q_OBJECT
public:
	QueryInstrumentAction(const CThostFtdcQryInstrumentField& qryField)
		:QueryAction(qryField,&CThostFtdcTraderApi::ReqQryInstrument)
	{
	}
	
	virtual ~QueryInstrumentAction() {}
Q_SIGNALS:
	void responseQueryInstruments(const QList<CThostFtdcInstrumentField>&);
public:
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override {
		this->onResponse(pInstrument, pRspInfo, bIsLast);
	}

protected:
	virtual void emitResponseQueryResults(const QList<CThostFtdcInstrumentField>& results) override {
		Q_EMIT responseQueryInstruments(results);
	}
};

Q_DECLARE_METATYPE(CThostFtdcInstrumentField);
Q_DECLARE_METATYPE(QList<CThostFtdcInstrumentField>);