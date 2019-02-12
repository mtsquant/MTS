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
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "ctp_ext/CTPClientCommon.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/Quote.h"
//#include <future>

class CTPFeedsClient:public QObject, public CTPClientCommon<CThostFtdcMdApi,CThostFtdcMdSpi>
{
	Q_OBJECT
public:
	CTPFeedsClient(const QString& address, const QString& brokerId, const QString& userId, const QString& password);
	~CTPFeedsClient();
	virtual QString name() const override {
		return "CTPFeedsClient";
	}

	virtual void onLoginSuccess() override;

	void setSubscribeInstruments(const  QList<mts::InstrumentId>&);

	virtual int subscribe() override;


Q_SIGNALS:
	void loginDone();
	void loginError(const QString&);
	//void quoteUpdate(mts::QuotePtr);

protected:
	void onDepthMarketData(CThostFtdcDepthMarketDataField*);

private:

	QStringList _instruments;
private:	
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)  override;
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)  override;

	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)  override;
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)  override;

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)  override;

	///ѯ��֪ͨ
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) override;
};

