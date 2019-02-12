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
#include "QueryAction.h"

class QueryOrderAction :public QueryAction<CThostFtdcQryOrderField, CThostFtdcOrderField>
{
	Q_OBJECT
public:
	QueryOrderAction(const CThostFtdcQryOrderField & field)
		:QueryAction(field, &CThostFtdcTraderApi::ReqQryOrder)
	{
	}
	virtual ~QueryOrderAction() {}

Q_SIGNALS:
	void responseQueryOrders(const QList<CThostFtdcOrderField>& orders);

public:
///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)  override {
		this->onResponse(pOrder, pRspInfo, bIsLast);
	}
protected:
	virtual void emitResponseQueryResults(const QList<CThostFtdcOrderField>& results) override {
		Q_EMIT responseQueryOrders(results);
	}
};

Q_DECLARE_METATYPE(CThostFtdcOrderField);
Q_DECLARE_METATYPE(QList<CThostFtdcOrderField>);