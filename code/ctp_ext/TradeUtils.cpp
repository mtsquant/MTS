
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
#include "TradeUtils.h"
#include "mts_core/Order.h"
#include "mts_core/OrderReport.h"
#include <QtCore/QMessageAuthenticationCode>
#include <QtCore/QDateTime>
#include "mts_core/InstrumentPropertyDb.h"
#include "base/MtsType.h"
#include "base/MtsLog.h"
#include "mts_core/QuoteSet.h"

void TradeUtils::buildBasicOrderReport(mts::Order *pOrder, mts::OrderReport* orderReport)
{
	orderReport->setVolume(pOrder->volume());
	orderReport->setPrice(pOrder->price());
	orderReport->setFillVolume(pOrder->fillVolume());
	orderReport->setPriceType(pOrder->priceType());
	orderReport->setDirectonSide(pOrder->directionSide());
	orderReport->setOffsetFlag(pOrder->offsetFlag());
	orderReport->setTimeCondition(pOrder->timeCondition());
	orderReport->setCreateSrc(pOrder->createSrc());
	orderReport->setStrategyId(pOrder->strategyId());
	orderReport->setInstanceId(pOrder->instanceId());
	orderReport->setReferenceId(pOrder->referenceId());
	orderReport->setTradingAccountId(pOrder->tradingAccountId());
	orderReport->setInstrumentId(pOrder->instrumentId());
	orderReport->setBook(pOrder->book());
	orderReport->setTradingDay(pOrder->tradingDay());
	orderReport->setCreateTicksSinceEpoch(pOrder->createTicksSinceEpoch());
	orderReport->setOrderExchId(pOrder->orderExchId());
}

QString TradeUtils::bitmex_signature(const QString& secret, const QString& method, const QString& path, int timestamp, const QString& body)
{
	QByteArray bytes = (method + path + QString::number(timestamp) + body).toLocal8Bit();
	return QMessageAuthenticationCode::hash(bytes, secret.toUtf8(), QCryptographicHash::Sha256).toHex().constData();
}

int TradeUtils::bitmex_generate_expires()
{
	return QDateTime::currentSecsSinceEpoch() + 3600;
}

QString TradeUtils::okex_signature(const QString& secret, const QString& method, const QString& path, const QString& timestamp, const QString& body)
{
	QByteArray bytes = (timestamp + method + path +  body).toLocal8Bit();
	return QMessageAuthenticationCode::hash(bytes, secret.toUtf8(), QCryptographicHash::Sha256).toBase64().constData();
}

QString TradeUtils::okex_generate_timestamp()
{
	return mts::Double::toString(QDateTime::currentMSecsSinceEpoch() / 1000.0, 3);
}

QString TradeUtils::marginSymbol(const QString & coinPair, const QString & coin)
{
	return "margin_" + coinPair + "_" + coin;
}

int TradeUtils::getGZBufferSize(int gzipSize,int rate/*=15*/)
{
	int bufferSize = gzipSize*rate;
	int bufferSize2 = (bufferSize / 1024)*1024;
	if (bufferSize2 < bufferSize) {
		bufferSize2 += 1024;
	}
	return bufferSize2;
}

double TradeUtils::getLastPrice(const QString& symbol)
{
	mts::InstrumentId iid = mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol);
	if (iid.isNull()) {
		MTS_ERROR("can not find instrument from symbol:%s\n", qPrintable(symbol));
		return 0;
	}
	auto quote = mts::QuoteSetSingleton::instance()->getQuote(iid);
	if (!quote || quote->lastPrice() <= 0) {
		MTS_ERROR("can not find valid quote for:%s\n", qPrintable(symbol));
		return 0;
	}
	return quote->lastPrice();
}