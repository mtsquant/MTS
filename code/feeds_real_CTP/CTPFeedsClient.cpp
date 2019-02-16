
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
#include "CTPFeedsClient.h"
#include <iostream>
#include <QtCore/QThread>
#include <QtCore/QCoreApplication>
#include "base/MtsLog.h"
#include <QtCore/QDir>
#include "ctp_ext/CTPUtils.h"
#include "base/EventQueue.h"
#include "QuoteEvent.h"
#include "base/PerfTest.h"



char** allocStringArray(const QStringList& items) {
	char** instruArray = new char*[items.size()];
	for (int i = 0; i < items.size(); ++i) {
		QString instrument = items[i];
		instruArray[i] = new char[instrument.size() + 1];
		strcpy(instruArray[i], qPrintable(instrument));
	}
	return instruArray;
}

void freeStringArray(char** strArray, int sz) {
	for (int i = 0; i < sz; ++i) {
		delete[] strArray[i];
	}
	delete[] strArray;
}



//

CTPFeedsClient::CTPFeedsClient(const QString& address, const QString& brokerId, const QString& userId, const QString& password)
	: CTPClientCommon<CThostFtdcMdApi, CThostFtdcMdSpi>(address, brokerId, userId, password)
{
	const char* pszFlowPath = "./feed_real_temp/";
	if (!QDir().exists(pszFlowPath)) {
		QDir().mkpath(pszFlowPath);
	}
	_userApi = CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath);
	_userApi->RegisterSpi(this);
}


CTPFeedsClient::~CTPFeedsClient() 
{
	_userApi->Release();
	_userApi = nullptr;
}

void CTPFeedsClient::onLoginSuccess() {
	Q_EMIT loginDone();
}

void CTPFeedsClient::setSubscribeInstruments(const QList<mts::InstrumentId> & instruments) {
	_instruments.clear();
	for (int i = 0, size = instruments.size(); i < size; ++i) {
		_instruments.append(instruments[i].symbol);
	}
}


void CTPFeedsClient::onDepthMarketData(CThostFtdcDepthMarketDataField * md) {
	mts::Quote* qt = new mts::Quote();
	static QString nullStr;
	if (!CTPUtils::convertMarketData2Quote(md, qt, nullStr) ) {
		MTS_ERROR("Failed to convert FutQuote from CThostFtdcDepthMarketDataField\n");
		delete qt;
		return;
	}
	//emit quoteUpdate(mts::QuotePtr(qt));
	EventQueue::defaultQueue()->push(EventPtr(new QuoteEvent(mts::QuotePtr(qt))));
}

int CTPFeedsClient::subscribe() {
	if (_instruments.isEmpty()) {
		return 0;
	}
	MTS_DEBUG("Subscribe instruments: %s\n ",qPrintable(_instruments.join(",")));
	char** instruArray = allocStringArray(_instruments);
	int ret = _userApi->SubscribeMarketData(instruArray,_instruments.size());
	freeStringArray(instruArray, _instruments.size());
	if (ret == 0) {//done
		return _instruments.size();
	} else {
		return 0;// ret;
	}
}

void CTPFeedsClient::OnRspSubMarketData(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	if (processRspInfoField(pRspInfo, QString("subscribe ")+pSpecificInstrument->InstrumentID)) {
	}
}

void CTPFeedsClient::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField * pDepthMarketData) {
	//std::cout << "OnRtnDepthMarketData" << std::endl;
	onDepthMarketData(pDepthMarketData);
}

void CTPFeedsClient::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	processRspInfoField(pRspInfo, QString("unsubscribe ") + pSpecificInstrument->InstrumentID);
}

void CTPFeedsClient::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	processRspInfoField(pRspInfo, "subscribe quote");
}

void CTPFeedsClient::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	processRspInfoField(pRspInfo, "unsubscribe quote");
}

void CTPFeedsClient::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField * pForQuoteRsp) {
	
}
