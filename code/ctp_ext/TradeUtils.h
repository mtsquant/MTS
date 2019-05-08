
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
#include "ctp_ext/ctp_ext_api.h"
#include <QtCore/QString>
#include <QtCore/QHash>

namespace mts {
	class Order;
	class OrderReport;
}


struct CTP_EXT_API SpotInfo {
	SpotInfo()
		:baseMinLoan(0.001), quoteMinLoan(0.001)
	{
	}
	SpotInfo(const QString& spot, const QString& base, const QString& quote)
		:baseMinLoan(0.001), quoteMinLoan(0.001)
	{
		this->spot = spot;
		this->base = base;
		this->quote = quote;
	}
	void setMinLoan(const QString& coin, double val) {
		if (coin == base) {
			baseMinLoan = val;
		}
		else if (coin == quote) {
			quoteMinLoan = val;
		}
	}

	double getMinLoan(const QString& coin) {
		if (coin == base) {
			return baseMinLoan;
		}
		else if (coin == quote) {
			return quoteMinLoan;
		}
		return 0.001;
	}

	bool isBase(const QString& coin) {
		return this->base == coin;
	}

	bool isQuote(const QString& coin) {
		return this->quote == coin;
	}
	QString spot;
	QString base;
	QString quote;
	double baseMinLoan;
	double quoteMinLoan;
};

struct CTP_EXT_API CurrencyInfo {
	CurrencyInfo()
		:trade(0), frozen(0), loan(0), interest(0), canWithdraw(0)
	{
	}
	double trade; //可用
	double frozen; //冻结
	double loan;//借贷 //margin account
	double interest;//利息 //margin account
	double canWithdraw; //可转 //margin account
};

struct CTP_EXT_API AccountInfo {
	AccountInfo()
		:id(""), type(""), spot(""), state("")
	{
	}

	AccountInfo(const QString& id, const QString& type, const QString& spot, const QString& state)
	{
		this->id = id;
		this->type = type;
		this->spot = spot;
		this->state = state;
	}
	CurrencyInfo* getCurrencyInfo(const QString& coin, bool createIfNotExists = false) {
		auto it = currencyInfoMap.constFind(coin);
		if (it == currencyInfoMap.constEnd()) {
			if (createIfNotExists) {
				CurrencyInfo* info = new CurrencyInfo();
				currencyInfoMap[coin] = info;
				return info;
			}
			return nullptr;
		}
		return it.value();
	}
	~AccountInfo()
	{
		qDeleteAll(currencyInfoMap);
	}

	QString id;
	QString	type;
	QString	spot;
	QString	state;
	QHash<QString/*coin*/, CurrencyInfo*> currencyInfoMap;
};

struct CTP_EXT_API LoanOrder
{
	LoanOrder(qint64 timeStamp, const QString& id, const QString& spot, const QString& coin, double totalVolume) {
		this->timeStamp = timeStamp;
		this->id = id;
		this->spot = spot;
		this->coin = coin;
		this->totalVolume = totalVolume;
		this->leftVolume = totalVolume;
		this->totalInterest = 0;
		this->leftInterest = 0;
	}
	qint64 timeStamp;
	QString id;
	QString spot;
	QString coin;
	double totalVolume;//借币总数量
	double leftVolume;//未还借币量
	double totalInterest;//利息总数量
	double leftInterest; //未还利息
};

class CTP_EXT_API TradeUtils
{
public:
	static void buildBasicOrderReport(mts::Order *pOrder, mts::OrderReport* orderReport);

	static QString bitmex_signature(const QString& secret, const QString& method, const QString& path, int timestamp, const QString& body);
	static int bitmex_generate_expires();

	static QString okex_signature(const QString& secret, const QString& method, const QString& path,const QString& timestamp, const QString& body);
	static QString okex_generate_timestamp();

	static QString marginSymbol(const QString& coinPair, const QString& coin);

	static int getGZBufferSize(int gzipSize,int rate=15);

	static double getLastPrice(const QString& symbol);

};
