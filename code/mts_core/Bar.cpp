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
#include "Bar.h"
#include "base/DateTime.h"
#include "base/MtsLog.h"
#include "mts_core/InstrumentPropertyDb.h"
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#ifdef MTS_UNIT_TEST
	#ifndef NDEBUG
		#include "./GeneratedFiles/DebugWithTest/CoreBar_generated.h"
	#else
		#include "./GeneratedFiles/ReleaseWithTest/CoreBar_generated.h"
	#endif
#else
	#ifndef NDEBUG
		#include "./GeneratedFiles/Debug/CoreBar_generated.h"
	#else
		#include "./GeneratedFiles/Release/CoreBar_generated.h"
	#endif
#endif


#define MemberBarCopyMethodImpl(Type,name,setName)	\
                Type Bar::name() const{return _coreBar->name;};\
                void Bar::setName(const Type& t){_coreBar->name=t;}\
                void Bar::name##ToJson(QJsonObject& jsonObj) const {jsonObj.insert(#name,(Type)_coreBar->name);}


namespace mts {
	Bar::Bar()
		: _coreBar(new CoreBarT())
		, _openTotalVolume(0), _openTrunover(0)
		, _currentTotalVolume(0), _openInterest(0)
	{

	}

	Bar::~Bar()
	{
	}

	//barʱ���ȣ�60�룬300���

	//qint64 Bar::beginTicksSinceEpoch () const{ //��ʼʱ��
	//	return _coreBar->endTicksSinceEpoch - _intervalSec * 1000;
	//}

	MemberBarCopyMethodImpl(qint64, endTicksSinceEpoch, setEndTicksSinceEpoch); //����ʱ��
	MemberBarCopyMethodImpl(double, openPrice, setOpenPrice);//���̼�
	MemberBarCopyMethodImpl(double, highPrice, setHighPrice);//��߼۸�
	MemberBarCopyMethodImpl(double, lowPrice, setLowPrice);//��ͼ۸�
	MemberBarCopyMethodImpl(double, closePrice, setClosePrice);//���̼�

	//MemberBarCopyMethodImpl(double, volume, setVolume);//�ɽ����� //TODO check double
	double Bar::volume() const {
		return *((double*)(&_coreBar->volume));
	};
	void Bar::setVolume(const double& volume) {
		_coreBar->volume = *((qint64*)(&volume));
	}
	void Bar::volumeToJson(QJsonObject& jsonObj) const {
		jsonObj.insert("volume", this->volume());
	}

	MemberBarCopyMethodImpl(double, openInterest, setOpenInterest);//�ֲ���
	MemberBarCopyMethodImpl(double, fairPrice, setFairPrice);
	MemberBarCopyMethodImpl(double, turnover, setTurnover);

	MemberBarCopyMethodImpl(double, midPrice, setMidPrice); //for daily it is settlement price
	MemberBarCopyMethodImpl(double, spread, setSpread);
	MemberBarCopyMethodImpl(int, bidAskDepth, setBidAskDepth);



	double Bar::vwap(double volumeMultiple) const {
		if (this->volume() == 0 || this->turnover() == 0) {
			return 0;
		}
		return this->turnover() / this->volume() / volumeMultiple;
	}

	//double Bar::spread() const {
	//	int count = quoteUpdateCount();
	//	if (count == 0) {
	//		return 0;
	//	}
	//	return spreadSum() / count;
	//}

	//int Bar::quoteSize() const {
	//	int volumeSum = quoteVolumeSum();
	//	int updateCount = quoteUpdateCount();
	//	if (volumeSum < 2 || updateCount == 0) {
	//		return 0;
	//	}
	//	return volumeSum / 2 / updateCount;
	//}

	void Bar::mergeFrom(const Bar & bar) {
		if (this->isEmpty()) {
			*this = bar;
		} else {
			setEndTicksSinceEpoch(bar.endTicksSinceEpoch());
			setHighPrice(qMax(this->highPrice(), bar.highPrice()));
			setLowPrice(qMin(this->lowPrice(), bar.lowPrice()));
			setClosePrice(bar.closePrice());

			setVolume(this->volume() + bar.volume());
			setTurnover(this->turnover() + bar.turnover());
			setOpenInterest(this->openInterest() + bar.openInterest());
			setFairPrice(bar.fairPrice());

			setSpread(bar.spread());
			setMidPrice(bar.midPrice());
			setBidAskDepth(bar.bidAskDepth());

			//setSpreadSum(this->spreadSum() + bar.spreadSum());
			//setQuoteVolumeSum(this->quoteVolumeSum() + bar.quoteVolumeSum());
			//setQuoteUpdateCount(this->quoteUpdateCount() + bar.quoteUpdateCount());

			_currentTotalVolume = bar._currentTotalVolume;
		}
	}

	Bar Bar::operator+(const Bar & bar) const {
		Bar mergedBar(*this);
		mergedBar.mergeFrom(bar);
		return mergedBar;
	}

	Bar * Bar::clone() const {
		Bar* bar = new Bar(*this);
		bar->_coreBar.reset(new CoreBarT(*this->_coreBar));
		return bar;
	}

	bool Bar::fromJson(const QJsonObject& jsonObj) {
		int date = jsonObj.value("date").toInt();
		auto timeValue = jsonObj.value("time");

		int time = timeValue.isString() ? timeValue.toString().toInt() : timeValue.toInt();
		this->setEndTicksSinceEpoch(DateTime(time / 10000000, time % 10000000 / 100000, time % 100000 / 1000, time % 1000, date).toUTCMillisecsSinceEpoch());

		this->setOpenPrice(jsonObj.value("openPrice").toDouble());
		this->setHighPrice(jsonObj.value("highPrice").toDouble());
		this->setLowPrice(jsonObj.value("lowPrice").toDouble());
		this->setClosePrice(jsonObj.value("closePrice").toDouble());

		this->setVolume(jsonObj.value("volume").toDouble()); //TODO check double
		this->setOpenInterest(jsonObj.value("openInterest").toDouble());
		this->setFairPrice(jsonObj.value("fairPrice").toDouble());
		this->setTurnover(jsonObj.value("turnover").toDouble());
		this->setSpread(jsonObj.value("spread").toDouble());
		this->setMidPrice(jsonObj.value("midPrice").toDouble());
		this->setBidAskDepth(jsonObj.value("bidAskDepth").toInt());

		return true;
	}

	QJsonObject  Bar::toJson() const {
		QJsonObject barObj;
		//barObj.insert("symbol", this->instrumentId().symbol);
		//barObj.insert("instrumentType", instrumentTypeName(this->instrumentId().typeId));
		//barObj.insert("exchId", exchIdName(this->instrumentId().exchId));
		DateTime dt = DateTime(this->endTicksSinceEpoch()).toLocal();
		barObj.insert("date", dt.date());
		barObj.insert("time", QString::number(dt.time()));
		this->openPriceToJson(barObj);
		this->highPriceToJson(barObj);
		this->lowPriceToJson(barObj);
		this->closePriceToJson(barObj);
		this->volumeToJson(barObj);
		this->openInterestToJson(barObj);
		this->fairPriceToJson(barObj);
		this->turnoverToJson(barObj);
		this->spreadToJson(barObj);
		this->midPriceToJson(barObj);
		this->bidAskDepthToJson(barObj);
		//barObj.insert ( "vwap" ,  this->vwap()); 
		//barObj.insert("spread", this->spread());
		//barObj.insert("quoteSize", this->quoteSize());
		return barObj;
	}

	bool Bar::fromJsonString(const QString & jsonStr) {
		QJsonParseError  error;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toLocal8Bit(), &error);
		if (error.error != QJsonParseError::NoError) {
			MTS_ERROR("Failed to parse bar json object '%s'\n", qPrintable(jsonStr));
			return false;
		}

		QJsonObject jsonObj = jsonDoc.object();
		return fromJson(jsonObj);
	}

	QString Bar::toJsonString() const {
		auto barObj = this->toJson();
		QJsonDocument jsonDoc(barObj);
		return jsonDoc.toJson().simplified();
	}

	QByteArray Bar::toByteArray() const {
		flatbuffers::FlatBufferBuilder fbb;
		fbb.Finish(CoreBar::Pack(fbb, _coreBar.data()));
		return QByteArray((const char*)fbb.GetBufferPointer(), fbb.GetSize());
	}

	bool Bar::fromByteArray(const QByteArray & bytes) {
		auto coreBar = GetCoreBar(bytes.constData());
		coreBar->UnPackTo(_coreBar.data());
		return true;
	}

	bool Bar::updateQuote(QuoteSnapshot *qt, int interval) {
		qint64 qtTicks = qt->ticksSinceEpoch();
		if (qtTicks < this->endTicksSinceEpoch() - interval) { // ǰ�պ�
			return false;
		}

		if (qtTicks >= this->endTicksSinceEpoch()) {
			return false;
		}

		const double qtLastPrice = qt->lastPrice();
		if (qt->isTradeChg() && (_currentTotalVolume != qt->totalVolume())) {
			_currentTotalVolume = qt->totalVolume();
			if (_openTotalVolume == 0) { //first update
				this->setOpenPrice(qtLastPrice);
				this->setHighPrice(qtLastPrice);
				this->setLowPrice(qtLastPrice);
				this->setVolume(qt->lastVolume());
				_openTotalVolume = qt->totalVolume();
				_openTrunover = qt->turnover();
				_openInterest = qt->openInterest();
			}
			else {
				if (qtLastPrice > this->highPrice()) {
					this->setHighPrice(qtLastPrice);
				}
				if (qtLastPrice < this->lowPrice()) {
					this->setLowPrice(qtLastPrice);
				}
				this->setVolume(qt->totalVolume() - _openTotalVolume);
				this->setTurnover(qt->turnover() - _openTrunover);
			}
			this->setClosePrice(qtLastPrice);
			this->setOpenInterest(qt->openInterest() - _openInterest);
		}

		if (qt->isQuoteChg()) {
			this->setFairPrice((qt->bidPrice()*qt->askVolume() + qt->askPrice()*qt->bidVolume()) / (qt->bidVolume() + qt->askVolume()));
			this->setSpread(qt->askPrice() - qt->bidPrice());
			this->setMidPrice((qt->askPrice() + qt->bidPrice())/2);
			this->setBidAskDepth((qt->bidVolume() + qt->askVolume())/2);
		}

		return true;
	}

	void Bar::registerMetaType() {
		qRegisterMetaType<BarPtr>("BarPtr");
		qRegisterMetaType<Bar>("Bar");
	}

	bool Bar::fromCsvString(const QString & line, QChar sep) {
      //date,time,open,high,low,close,vol,turnover,openInt,fairPrice,midPrice,spread,bidAskDepth
		QStringList secs = line.split(sep);
		if (secs.size() < 13) {
			MTS_ERROR("Invalid bar field size '%d'\n", secs.size());
			return false;
		}
		bool ok = false;
		int date = secs[0].toInt(&ok);
		if (!ok) {
			MTS_ERROR("Invalid bar date '%s'\n", qPrintable(secs[0]));
			return false;
		}
		int time = secs[1].toInt()*1000;
		if (!ok) {
			MTS_ERROR("Invalid bar time '%s'\n", qPrintable(secs[1]));
			return false;
		}
		DateTime dt(time, date);
		if (dt.isNull()) {
			MTS_ERROR("Invalid bar date time '%s %s'\n", qPrintable(secs[0]), qPrintable(secs[1]));
			return false;
		}
		this->setEndTicksSinceEpoch(dt.toUTCMillisecsSinceEpoch());

		this->setOpenPrice(secs[2].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar open price '%s'\n", qPrintable(secs[2]));
			return false;
		}

		this->setHighPrice(secs[3].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar high price '%s'\n", qPrintable(secs[3]));
			return false;
		}

		this->setLowPrice(secs[4].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar low price '%s'\n", qPrintable(secs[4]));
			return false;
		}

		this->setClosePrice(secs[5].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar close price '%s'\n", qPrintable(secs[5]));
			return false;
		}

		this->setVolume(secs[6].toDouble(&ok)); //TODO check double
		if (!ok) {
			MTS_ERROR("Invalid bar volume '%s'\n", qPrintable(secs[6]));
			return false;
		}

		this->setTurnover(secs[7].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar turnover '%s'\n", qPrintable(secs[7]));
			return false;
		}

		this->setOpenInterest(secs[8].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar open interest '%s'\n", qPrintable(secs[8]));
			return false;
		}

		this->setFairPrice(secs[9].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar fair price '%s'\n", qPrintable(secs[9]));
			return false;
		}

		this->setMidPrice(secs[10].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar mid price '%s'\n", qPrintable(secs[10]));
			return false;
		}

		this->setSpread(secs[11].toDouble(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar spread '%s'\n", qPrintable(secs[11]));
			return false;
		}

		this->setBidAskDepth(secs[12].toInt(&ok));
		if (!ok) {
			MTS_ERROR("Invalid bar bid ask depth '%s'\n", qPrintable(secs[12]));
			return false;
		}

		return true;
	}

	QString Bar::toCsvString() const
	{
		//date, time, openPrice, highPrice, lowPrice, closePrice, volume, turnover, openInterest, fairPrice, midPrice, spread, bidAskDepth
		DateTime dt = DateTime(this->endTicksSinceEpoch()).toLocal();
		QString str;
		str.sprintf("%d,%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%d", dt.date(), dt.time(), openPrice(), highPrice(), lowPrice(), closePrice(),
			volume(), turnover(), openInterest(), fairPrice(), midPrice(), spread(), bidAskDepth()); //TODO check format
		return str;
	}

	Bar * Bar::generateNextEmptyBar(qint64 timestamp) const {
		Bar* barPtr = new Bar();
		barPtr->setEndTicksSinceEpoch(timestamp);
		barPtr->setOpenPrice(this->closePrice());
		barPtr->setHighPrice(this->closePrice());
		barPtr->setLowPrice(this->closePrice());
		barPtr->setClosePrice(this->closePrice());
		barPtr->setSpread(this->spread());
		barPtr->setFairPrice(this->fairPrice());
		barPtr->setMidPrice(this->midPrice());
		return barPtr;
	}

	bool Bar::isEmpty() const {
		return _currentTotalVolume == 0 && _openTotalVolume == 0;
	}

}
