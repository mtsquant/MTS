
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
#include "InstrumentProperty.h"
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include "base/MtsLog.h"

namespace mts
{
	InstrumentBaseProperty::InstrumentBaseProperty() {
		setKind(InstrumentKind::KIND_NORMAL);
	}

	InstrumentBaseProperty::~InstrumentBaseProperty() {

	}

	void InstrumentBaseProperty::dumpTo(QJsonObject * obj) const {
		obj->insert("InstrumentID", instrumentId().symbol);
		obj->insert("InstrumentName", name());
		obj->insert("InstrumentType", mts::instrumentTypeName(instrumentId().typeId));
		obj->insert("ExchangeID", mts::exchIdName(instrumentId().exchId));
		obj->insert("MtsSymbol", mtsSymbol());
		obj->insert("VolumeMultiple", volumeMultiple());
		obj->insert("PriceTick", priceTick());
		obj->insert("MinOrderSize", minOrderSize());
		obj->insert("OrderSizeIncrement", orderSizeIncrement());
		obj->insert("PricePrecision", pricePrecision());
		obj->insert("SizePrecision", sizePrecision());
		obj->insert("CreateDate", createDate());
	}

	bool InstrumentBaseProperty::loadFrom(const QJsonObject& obj) {

		QJsonValue id = obj.value("InstrumentID");
		QJsonValue instrumentType = obj.value("InstrumentType");
		QJsonValue exchangeID = obj.value("ExchangeID");
		QJsonValue name = obj.value("InstrumentName");
		QJsonValue mtsSymbol = obj.value("MtsSymbol");
		QJsonValue volumeMultiple = obj.value("VolumeMultiple");
		QJsonValue priceTick = obj.value("PriceTick");
		QJsonValue minOrderSize = obj.value("MinOrderSize");
		QJsonValue orderSizeIncrement = obj.value("OrderSizeIncrement");
		QJsonValue pricePrecision = obj.value("PricePrecision");
		QJsonValue sizePrecision = obj.value("SizePrecision");
		QJsonValue createDate = obj.value("CreateDate");
		if (id.isString() && instrumentType.isString() && exchangeID.isString() && volumeMultiple.isDouble() 
			&& priceTick.isDouble() && minOrderSize.isDouble() && orderSizeIncrement.isDouble() && createDate.isDouble() 
			&& pricePrecision.isDouble() && sizePrecision.isDouble())
		{
			auto instrumentId = mts::InstrumentId(id.toString(), mts::instrumentType(qPrintable(instrumentType.toString())), mts::exchId(qPrintable(exchangeID.toString())));
			this->setInstrumentId(instrumentId);
			this->setName(name.toString());
			this->setMtsSymbol(mtsSymbol.toString());
			this->setVolumeMultiple(volumeMultiple.toDouble());
			this->setPriceTick(priceTick.toDouble());
			this->setMinOrderSize(minOrderSize.toDouble());
			this->setOrderSizeIncrement(orderSizeIncrement.toDouble());
			this->setPricePrecision(pricePrecision.toInt());
			this->setSizePrecision(sizePrecision.toInt());
			this->setCreateDate(createDate.toInt());
			auto it= obj.constFind("ProductId");
			if (it != obj.constEnd()) {
				this->setProuduct(it.value().toString());
			}
			if (this->prouduct().isEmpty()) {
				this->setProuduct(InstrumentId::toProduct(this->instrumentId().symbol));

			}
			return true;
		} else {
			MTS_ERROR("invalid instrument config:%s", qPrintable(QJsonDocument(obj).toJson().simplified()));
			return false;
		}
	}

	bool InstrumentBaseProperty::isNull () const{
		return _instrumentId.isNull();
	}

	void InstrumentBaseProperty::setInstrumentId(const InstrumentId & id) {
		_instrumentId = id;
		auto exchId = this->_instrumentId.exchId;
		TradingSections secs;
        switch (exchId) {
		case EXCH_CZCE:
		case EXCH_DCE:
			secs << TradingTimeRange(900, 1015)
				<< TradingTimeRange(1030, 1130)
				<< TradingTimeRange(1330, 1500)
				<< TradingTimeRange(2100, 2330)
				;
			break;
		case EXCH_SHFE:
		{
			secs << TradingTimeRange(900, 1015)
				<< TradingTimeRange(1030, 1130)
				<< TradingTimeRange(1330, 1500)
				;
			auto symbol = this->_instrumentId.symbol.toLower();
			if (symbol.startsWith("cu")) { //天然橡胶
				secs << TradingTimeRange ( 2100 , 2300 );
				return ;
			} 
			secs << TradingTimeRange ( 2100 , 2359 , true );

			if (symbol.startsWith("ag") || symbol.startsWith("au")) {//贵金属
				secs << TradingTimeRange(0, 230);
			} else{
				secs<< TradingTimeRange ( 0 , 100 );
			}

			break;
		}
		case EXCH_CFFEX:
		case EXCH_INE:
			secs<<TradingTimeRange(900,1130) 
				<< TradingTimeRange (1300,1515)
				;
			break;
		case EXCH_SHSE:
		case EXCH_SZSE:
			secs << TradingTimeRange(930, 1130)
				<< TradingTimeRange(1300, 1500);
			break;
		case EXCH_HB:
		case EXCH_BMEX:
		case EXCH_OK:
			secs << TradingTimeRange(0, 2359, true);
			break;
		default:
			MTS_ERROR("Unknown exchId:%d\n", this->_instrumentId.exchId);
			return;
		}
		setTradingSection(secs);
	}

	bool InstrumentBaseProperty::isTradingTimeSection(const DateTime & dt) const {
		int time = dt.hour()*100+ dt.minute();
		for (int i = 0, size = _tradingSection.size(); i < size; ++i) {
			const TradingTimeRange& range = _tradingSection[i];
			if (range.isInSection(time)) {
				return true;
			}
		}
		return false;
	}

	QString mts::InstrumentBaseProperty::productId() const {
		return mtsSymbol().left(mtsSymbol().length()-4);
	}

	QString InstrumentBaseProperty::toJsonString () const{
		QJsonObject json;
		this->dumpTo ( &json );
		QJsonDocument doc ( json );
		return doc.toJson ().simplified ();
	}


	InstrumentProperty::InstrumentProperty() {

	}

	InstrumentProperty::~InstrumentProperty() {

	}

	void InstrumentProperty::dumpTo(QJsonObject * object) const {
		InstrumentBaseProperty::dumpTo(object);
		object->insert("OpenDate", this->openDate());
		object->insert("ExpireDate", this->expireDate());
		object->insert("StartDelivDate", this->startDelivDate());
		object->insert("EndDelivDate", this->endDelivDate());
	}

	bool InstrumentProperty::loadFrom(const QJsonObject& obj) {
		if (!InstrumentBaseProperty::loadFrom(obj)) {
			return false;
		}
		QJsonValue openDate = obj.value("OpenDate");
		QJsonValue expireDate = obj.value("ExpireDate");
		QJsonValue startDelivDate = obj.value("StartDelivDate");
		QJsonValue endDelivDate = obj.value("EndDelivDate");
		if (openDate.isDouble() && expireDate.isDouble() && startDelivDate.isDouble() && endDelivDate.isDouble()) {
			this->setOpenDate(openDate.toInt());
			this->setExpireDate(expireDate.toInt());
			this->setStartDelivDate(startDelivDate.toInt());
			this->setEndDelivDate(endDelivDate.toInt());
			return true;
		} else {
			return false;
		}
	}


	mts::InstrumentId InstrumentProperty::convertMainInstument(const mts::InstrumentId& from)
	{
		mts::InstrumentId id = from;
		id.symbol = InstrumentId::toContinuousSymbol(from.symbol);
		return id;
	}

}
