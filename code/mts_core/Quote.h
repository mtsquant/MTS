
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
#include<QtCore/QtGlobal>
#include <QtCore/QSharedPointer>

#include "InstrumentObject.h"
#include "base/PerfTest.h"


#define PRICE_TINY 0.00000001
namespace mts {
	class MTS_CORE_API Quote:public InstrumentObject
	{
	public:
		Quote();
		virtual ~Quote();

		MemberCopyMethodDefine(int, tradingDay, setTradingDay);//交易日期

		MemberCopyMethodDefine(qint64, ticksSinceEpoch, setTicksSinceEpoch);//当前日期时间
		MemberCopyMethodDefine(double, preClosePrice, setPreClosePrice);//昨日收盘价
		MemberCopyMethodDefine(double, openPrice, setOpenPrice);//开盘价格
		MemberCopyMethodDefine(double, highPrice, setHighPrice);//最高价格
		MemberCopyMethodDefine(double, lowPrice, setLowPrice);//最低价格
		MemberCopyMethodDefine(double, closePrice, setClosePrice);//今日收盘价
		MemberCopyMethodDefine(double, lastPrice, setLastPrice);//最新价格
		MemberCopyMethodDefine(double, lastVolume, setLastVolume);//最新成交量 //TODO check double		
		MemberCopyMethodDefine(double, totalVolume, setTotalVolume);//成交总量 //TODO check double
		MemberCopyMethodDefine(double, bidPrice, setBidPrice);
		MemberCopyMethodDefine(double, askPrice, setAskPrice);
		MemberCopyMethodDefine(double, bidVolume, setBidVolume); //TODO check double
		MemberCopyMethodDefine(double, askVolume, setAskVolume); //TODO check double

		MemberCopyMethodDefine(double, upperLimitPrice, setUpperLimitPrice);//最高限价
		MemberCopyMethodDefine(double, lowerLimitPrice, setLowerLimitPrice);//最低限价
		MemberCopyMethodDefine(double, preSettlementPrice, setPreSettlementPrice);//上次结算价
		MemberCopyMethodDefine(double, settlementPrice, setSettlementPrice);//本次结算价
        MemberCopyMethodDefine(double, turnover, setTurnover);//成交金额
		MemberCopyMethodDefine(double, openInterest, setOpenInterest);//持仓量

        MemberCopyMethodDefine(int, status, setStatus);//行情状态
		MemberCopyMethodDefine(qint64, revMicrosecond, setRevMicrosecond);//

	public:
		QString toJsonString() const;
		virtual QJsonObject toJson() const;
		QJsonObject toSimpleJson () const;
		bool isNull() const;
		virtual Quote* clone() const ;

		static void registerMetaType();
	};

	typedef QSharedPointer<Quote> QuotePtr;


	class MTS_CORE_API QuoteSnapshot :public Quote
	{
	public:
		QuoteSnapshot();
		QuoteSnapshot(const Quote&);
		virtual ~QuoteSnapshot();
	public:
		static int priceCompare(double p1, double p2);
		void merge(Quote* otherQt);
		bool isBidChg()const;
		bool isAskChg()const;
		bool isQuoteChg()const;
		bool isTradeChg()const;

		virtual QuoteSnapshot* clone() const override;
		virtual QJsonObject toJson() const override;

		static void registerMetaType();

		double fairPrice() const;
	private:
		void doBidChg();
		void doAskChg();
		void doTradeChg();
		int _fieldChg;
	};

	typedef QSharedPointer<QuoteSnapshot> QuoteSnapshotPtr;

}

Q_DECLARE_METATYPE(mts::QuotePtr);
Q_DECLARE_METATYPE(mts::QuoteSnapshotPtr);

