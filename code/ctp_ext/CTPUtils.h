
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

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtCore/QJsonObject>

#include "mts_core/Quote.h"

#ifdef _WIN32
#include "../../api/win64/CTP/ThostFtdcTraderApi.h"
#else
#include "../../api/linux64/CTP/ThostFtdcTraderApi.h"
#include <float.h>
#endif
#include "ctp_ext_api.h"


class CTP_EXT_API CTPUtils
{
public:
	static QString toJsonString(const CThostFtdcRspInfoField*);
	static QString toJsonString(const CThostFtdcInputOrderField*);
	static QString toJsonString(const CThostFtdcOrderField*);
	static QString toJsonString(const CThostFtdcTradeField*);
	static QString toJsonString(const CThostFtdcInputOrderActionField*);
	static QString toJsonString(const CThostFtdcOrderActionField*);
	static QString toJsonString(const CThostFtdcInstrumentField*);
	//static QString toString(*);
	//static QString toString(*);
	//static QString getFeedsFrontAddress(const QVariantMap & params, QString* feedsFrontAddress); //return error string 
	//static QString getUserName(const QVariantMap & params,QString* userName); //return error string 
	//static QString getBrokerId(const QVariantMap & params, QString* borkerId);
	//static QString getPassword(const QVariantMap & params,QString* password);

	static void toJson(const CThostFtdcTradingAccountField& account, QJsonObject& json);
	static void toJson(const CThostFtdcInvestorPositionField& pos, QJsonObject& json);
	static void toJson(const CThostFtdcInvestorPositionDetailField& pos, QJsonObject& json);
	static void toJson(const CThostFtdcOrderField& order, QJsonObject& json);
	static void toJson(const CThostFtdcTradeField& fill, QJsonObject& json);
	static void toJson(const CThostFtdcInstrumentField& instrument, QJsonObject& json);
	static void toJson(const CThostFtdcInstrumentMarginRateField& field, QJsonObject& json);
	static void toJson(const CThostFtdcInstrumentCommissionRateField& field, QJsonObject& json);
	static void toJson ( const CThostFtdcDepthMarketDataField & , QJsonObject& json );



	static QString depthMarketData2String(const CThostFtdcDepthMarketDataField* f);

	static void printDepthMarketData(CThostFtdcDepthMarketDataField* f);

	static QString investorPositionField2String(const CThostFtdcInvestorPositionField* pos);

	static bool convertMarketData2Quote(CThostFtdcDepthMarketDataField * md, mts::Quote* qt, const QString& asSymbol);
};

