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
#include "Quote.h"
#include <QtCore/QJsonDocument>
#include "base/DateTime.h"

namespace mts
{
	

	Quote::Quote()
		:InstrumentObject(),
		_tradingDay(0),
		_ticksSinceEpoch(0),
		_preClosePrice(0),
		_openPrice(0),
		_highPrice(0),
		_lowPrice(0),
		_closePrice(0),
		_lastPrice(0),
		_lastVolume(0),
		_totalVolume(0),
		_bidPrice(0),
		_askPrice(0),
		_bidVolume(0),
		_askVolume(0),
		_upperLimitPrice(0),
		_lowerLimitPrice(0),
		_preSettlementPrice(0),
        _settlementPrice(0),
        _turnover(0),
		_openInterest(0),
		_status(0)
	{
		
	}

	Quote::~Quote()
	{

	}
	
	void Quote::registerMetaType() {
		//qRegisterMetaType<Quote>("Quote");
		qRegisterMetaType<QuotePtr>("QuotePtr");
	}

	QString Quote::toJsonString() const {
		QJsonDocument jsonDoc(this->toJson());
		return jsonDoc.toJson().simplified();
	}

	QJsonObject Quote::toJson() const {
		QJsonObject jsonObj = toSimpleJson ();
		jsonObj.insert("instrumentType", instrumentTypeName(this->instrumentId().typeId));
		jsonObj.insert("exchId", exchIdName(this->instrumentId().exchId));
		this->tradingDayToJson(jsonObj);
		this->preClosePriceToJson(jsonObj);
		this->upperLimitPriceToJson(jsonObj);
		this->lowerLimitPriceToJson(jsonObj);
		this->preSettlementPriceToJson(jsonObj);
		this->settlementPriceToJson(jsonObj);
        this->turnoverToJson(jsonObj);
		this->openInterestToJson(jsonObj);
		this->statusToJson(jsonObj);
		return jsonObj;
	}

	QJsonObject Quote::toSimpleJson () const{
		QJsonObject jsonObj;
		jsonObj.insert ( "symbol" , this->instrumentId ().symbol );
		this->ticksSinceEpochToJson ( jsonObj );
		if ( this->ticksSinceEpoch () > 0 ){
			jsonObj.insert ( "dateTime" , DateTime ( this->ticksSinceEpoch () ).toLocalString () );
		} else{
			jsonObj.insert ( "dateTime" , "0" );
		}
		this->openPriceToJson ( jsonObj );
		this->highPriceToJson ( jsonObj );
		this->lowPriceToJson ( jsonObj );
		this->closePriceToJson ( jsonObj );
		this->lastPriceToJson ( jsonObj );
		this->lastVolumeToJson ( jsonObj );
		this->totalVolumeToJson ( jsonObj );
		this->bidPriceToJson ( jsonObj );
		this->askPriceToJson ( jsonObj );
		this->bidVolumeToJson ( jsonObj );
		this->askVolumeToJson ( jsonObj );
		return jsonObj;
	}

	bool Quote::isNull() const {
		return this->instrumentId().isNull();
	}

	Quote * Quote::clone() const{
		return new Quote(*this);;
	}


	//////

	enum QuoteFieldFlag {
		QF_NULL = 0,
		QF_BID = 0x1,
		QF_ASK = 0x2,
		QF_TRADE = 0x4
	};

	QuoteSnapshot::QuoteSnapshot()
		:Quote(),_fieldChg(0)
	{
	}

	QuoteSnapshot::QuoteSnapshot(const Quote & qt)
		:Quote(qt), _fieldChg(0)
	{

	}

	QuoteSnapshot::~QuoteSnapshot()
	{
	}

	int QuoteSnapshot::priceCompare(double p1, double p2)
	{
		double p = p1 - p2;
		if (p > PRICE_TINY) {
			return 1;
		}
		else if (p < -PRICE_TINY) {
			return -1;
		}
		return 0;
	}

	bool QuoteSnapshot::isBidChg() const
	{
		return _fieldChg&QF_BID;
	}

	bool QuoteSnapshot::isAskChg() const
	{
		return _fieldChg&QF_ASK;
	}

	bool QuoteSnapshot::isQuoteChg() const
	{
		return isBidChg()||isAskChg();
	}

	bool QuoteSnapshot::isTradeChg() const
	{
		return _fieldChg&QF_TRADE;
	}

	QuoteSnapshot * QuoteSnapshot::clone() const
	{
		return new QuoteSnapshot(*this);
	}

	QJsonObject QuoteSnapshot::toJson() const {
		QJsonObject jsonObj = Quote::toJson();
		jsonObj.insert("fieldChangeFlag", _fieldChg);
		return jsonObj;
	}

	void QuoteSnapshot::registerMetaType() {
		qRegisterMetaType<QuoteSnapshot>("QuoteSnapshot");
		qRegisterMetaType<QuoteSnapshotPtr>("QuoteSnapshotPtr");
	}

	double QuoteSnapshot::fairPrice() const {
		return (this->bidPrice()*this->askVolume() + this->askPrice()*this->bidVolume()) / (this->askVolume() + this->bidVolume());
	}

	void QuoteSnapshot::merge(Quote * qt) {
		_fieldChg = 0;
		if (qt->tradingDay() > 0) {
			this->setTradingDay(qt->tradingDay());
		}
		if (qt->ticksSinceEpoch() > 0) {
			this->setTicksSinceEpoch(qt->ticksSinceEpoch());
		}
		if (qt->preClosePrice() > 0) {
			this->setPreClosePrice(qt->preClosePrice());
		}
		if (qt->openPrice() > 0) {
			this->setOpenPrice(qt->openPrice());
		}
		if (qt->highPrice() > 0) {
			this->setHighPrice(qt->highPrice());
		}
		if (qt->lowPrice() > 0) {
			this->setLowPrice(qt->lowPrice());
		}
		if (qt->closePrice() > 0) {
			this->setClosePrice(qt->closePrice());
		}

		if (qt->lastPrice() > 0) {
			if (priceCompare(lastPrice(),qt->lastPrice())) {
				doTradeChg();
			}
			this->setLastPrice(qt->lastPrice());
		}
		if (qt->lastVolume() > 0) {
			if (lastVolume() != qt->lastVolume()) {
				doTradeChg();
			}
			this->setLastVolume(qt->lastVolume());
		}
		if (qt->totalVolume() > 0) {
            if (this->totalVolume()<qt->totalVolume()){
                doTradeChg();
            }
			this->setTotalVolume(qt->totalVolume());
		}
		if (qt->bidPrice() > 0) {
			if (priceCompare(bidPrice(), qt->bidPrice())) {
				doBidChg();
			}
			this->setBidPrice(qt->bidPrice());
		}
		if (qt->bidVolume() > 0) {
			if (bidVolume() != qt->bidVolume()) {
				doBidChg();
			}
			this->setBidVolume(qt->bidVolume());
		}
		if (qt->askPrice() > 0) {
			if (priceCompare(askPrice(), qt->askPrice())) {
				doAskChg();
			}
			this->setAskPrice(qt->askPrice());
		}
		if (qt->askVolume() > 0) {
			if (askVolume() != qt->askVolume()) {
				doAskChg();
			}
			this->setAskVolume(qt->askVolume());
		}
		if (qt->upperLimitPrice() > 0) 
			this->setUpperLimitPrice(qt->upperLimitPrice());
		if (qt->lowerLimitPrice() > 0) 
			this->setLowerLimitPrice(qt->lowerLimitPrice());
		if (qt->preSettlementPrice() > 0) 
			this->setPreSettlementPrice(qt->preSettlementPrice());
		if (qt->settlementPrice() > 0) 
			this->setSettlementPrice(qt->settlementPrice());
        if (qt->turnover()>0){
            this->setTurnover(qt->turnover());
        }
		if (qt->openInterest() > 0) {
			this->setOpenInterest(qt->openInterest());
		}
		if (qt->status() > 0) 
			this->setStatus(qt->status());
		this->setRevMicrosecond(qt->revMicrosecond());

	}

	void QuoteSnapshot::doBidChg()
	{
		_fieldChg|=QF_BID;
	}

	void QuoteSnapshot::doAskChg()
	{
		_fieldChg |= QF_ASK;
	}

	void QuoteSnapshot::doTradeChg()
	{
		_fieldChg |= QF_TRADE;
	}

	//================================
	//FutQuote::FutQuote()
	//	:Quote()
	//{

	//}

	//FutQuote::~FutQuote()
	//{

	//}
	////void FutQuote::merge(Quote * otherQt) {
	////	Quote::merge(otherQt);
	////}
	//FutQuote * FutQuote::clone() const {
	//	return new FutQuote(*this);
	//}
	////=========================================
	//StockQuote::StockQuote()
	//	:Quote() 
	//{

	//}

	//StockQuote::~StockQuote()
	//{

	//}

	////void StockQuote::merge(Quote * otherQt) {
	////	Quote::merge(otherQt);
	////}

	//StockQuote * StockQuote::clone() const {
	//	return new StockQuote(*this);
	//}

}
