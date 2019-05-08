
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
#include <QtCore/QtGlobal>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#include <QtCore/QJsonObject>

#include "InstrumentObject.h"
#include "base/PerfTest.h"


#define MarketMethodDefine(Type,Container,name,setName,getName)	public:\
				Type name() const{return _##name##s[0];};\
				Type getName(int depth) const{return _##name##s[depth-1];};\
				void setName(Type t,int depth=1){_##name##s[depth-1]=t;}\
				const Container& getName##s() const{return _##name##s;}\
				void setName##s(const Container& vals){ _##name##s=vals;}\
				void name##ToJson(QJsonObject& jsonObj,int depth=1) const {jsonObj.insert(QString("%1%2").arg(#name).arg(depth),_##name##s[depth-1]);} \
			private:\
				Container _##name##s;

namespace mts {
	class MTS_CORE_API Quote:public InstrumentObject
	{
	public:
		Quote();
		virtual ~Quote();

		MemberCopyMethodDefine(int, tradingDay, setTradingDay);//交易日期

		MemberCopyMethodDefine(qint64, ticksSinceEpoch, setTicksSinceEpoch);//报价时间戳
		MemberCopyMethodDefine(qint64, receiveTicksSinceEpoch, setReceiveTicksSinceEpoch);//接收到报价时间戳
		MemberCopyMethodDefine(double, preClosePrice, setPreClosePrice);//昨日收盘价
		MemberCopyMethodDefine(double, openPrice, setOpenPrice);//开盘价格
		MemberCopyMethodDefine(double, highPrice, setHighPrice);//最高价格
		MemberCopyMethodDefine(double, lowPrice, setLowPrice);//最低价格
		MemberCopyMethodDefine(double, closePrice, setClosePrice);//今日收盘价
		MemberCopyMethodDefine(double, lastPrice, setLastPrice);//最新价格
		MemberCopyMethodDefine(double, lastVolume, setLastVolume);//最新成交量 //TODO check double		
		MemberCopyMethodDefine(DirectionSide, lastDirection, setLastDirection); //最新成交方向
		MemberCopyMethodDefine(double, totalVolume, setTotalVolume);//成交总量 //TODO check double

		MarketMethodDefine(double, QVector<double>, bidPrice, setBidPrice, getBidPrice);
		MarketMethodDefine(double, QVector<double>, askPrice, setAskPrice, getAskPrice);
		MarketMethodDefine(double, QVector<double>, bidVolume, setBidVolume, getBidVolume); //TODO check double
		MarketMethodDefine(double, QVector<double>, askVolume, setAskVolume, getAskVolume); //TODO check double

		MemberCopyMethodDefine(double, upperLimitPrice, setUpperLimitPrice);//最高限价
		MemberCopyMethodDefine(double, lowerLimitPrice, setLowerLimitPrice);//最低限价
		MemberCopyMethodDefine(double, preSettlementPrice, setPreSettlementPrice);//上次结算价
		MemberCopyMethodDefine(double, settlementPrice, setSettlementPrice);//本次结算价
        MemberCopyMethodDefine(double, turnover, setTurnover);//成交金额
		MemberCopyMethodDefine(double, openInterest, setOpenInterest);//持仓量

        MemberCopyMethodDefine(int, status, setStatus);//行情状态
		MemberCopyMethodDefine(qint64, revMicrosecond, setRevMicrosecond);//for performance test

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

