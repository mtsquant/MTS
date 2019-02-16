
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
#include "OrderAction.h"
#include <QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <assert.h>
#include "mts_core/InstrumentPropertyDb.h"
#include "base/DateTime.h"

namespace mts {
	OrderAction::OrderAction(char type)
		:InstrumentObject(),_type(type), _book(0), _tradingDay(0), _createTicksSinceEpoch(DateTime::now().toUTCMillisecsSinceEpoch ())
	{

	}

	OrderAction::OrderAction(const OrderAction& action)
		: InstrumentObject(action.instrumentId()), _type(action._type)
	{
		_accountId=action._accountId;
		//_instumentId = action._instumentId;
		_book = action._book;
		_tradingDay = action._tradingDay;
		_createTicksSinceEpoch = action._createTicksSinceEpoch;
		_perfNote = action._perfNote;
	}

	OrderAction::~OrderAction()
	{

	}


	const TradingAccountId& OrderAction::tradingAccountId() const
	{
		return _accountId;
	}

	//const InstrumentId& OrderAction::instrumentId() const
	//{
	//	return _instumentId;
	//}

	char OrderAction::type() const
	{
		return _type;
	}

	int OrderAction::book() const
	{
		return _book;
	}

	int OrderAction::tradingDay() const
	{
		return _tradingDay;
	}

	qint64 OrderAction::createTicksSinceEpoch() const
	{
		return _createTicksSinceEpoch;
	}

	void OrderAction::setTradingAccountId(const TradingAccountId& id)
	{
		_accountId = id;
	}

	//void OrderAction::setInstrumentId(const InstrumentId& id)
	//{
	//	_instumentId = id;
	//}

	void OrderAction::setBook(int bk)
	{
		_book = bk;
	}

	void OrderAction::setTradingDay(int day)
	{
		_tradingDay = day;
	}

	void OrderAction::setCreateTicksSinceEpoch(qint64 msecs)
	{
		_createTicksSinceEpoch = msecs;
	}

	QJsonObject& OrderAction::toJson(QJsonObject& jsonObj) const
	{
		jsonObj.insert("symbol", this->instrumentId().symbol);
		jsonObj.insert("instrumentType", instrumentTypeName(this->instrumentId().typeId));
		jsonObj.insert("exchId", exchIdName(this->instrumentId().exchId));
		jsonObj.insert("type", this->type());
		jsonObj.insert("book", this->book());
		jsonObj.insert("tradingDay", this->tradingDay());
		jsonObj.insert("createTicksSinceEpoch", this->createTicksSinceEpoch());
		jsonObj.insert("perfNote", this->perfNote());
		return jsonObj;
	}

	QString OrderAction::toJsonString() const
	{
        QJsonObject tmp;
        QJsonDocument jsonDoc(this->toJson(tmp));
		return jsonDoc.toJson().simplified();
	}

	void OrderAction::setPerfNote(const QString& note)
	{
		_perfNote = note;
	}

	QString OrderAction::perfNote() const
	{
		return _perfNote;
	}
	//====================================
	OrderActionNew::OrderActionNew(char type)
		:OrderAction(type), _orderType(int(OT_DIRECT)), _priceType(PriceType::PRICE_LIMIT),
		_condition(TimeCondition::TC_UNKNOWN), _offset(CombOffsetFlag::OF_UNKNOWN), _side(DirectionSide::D_UNKNOWN),
		_volume(0), _fillVolume(0), _price(0), _fillAmount(0), _strategyId(0),_instanceId(0)
	{

	}

	OrderActionNew::OrderActionNew(const OrderActionNew& action)
		: OrderAction(action)
	{
		_volume= action._volume;
		_fillVolume= action._fillVolume;
		_price=action._price;
		_fillAmount=action._fillAmount;
		_priceType=action._priceType;
		_side=action._side;
		_offset=action._offset;
		_condition=action._condition;
		_createSrc=action._createSrc;
		_refId=action._refId;
		_strategyId = action._strategyId;
		_instanceId = action._instanceId;
		_orderType = action._orderType;
	}

	OrderActionNew::~OrderActionNew()
	{

	}

	double OrderActionNew::volume() const //TODO check double
	{
		return _volume;
	}

	double OrderActionNew::price() const
	{
		return _price;
	}

	double OrderActionNew::fillVolume() const //TODO check double
	{
		return _fillVolume;
	}

	double OrderActionNew::leftVolume() const //TODO check double
	{
		return volume() - fillVolume();
	}

	double OrderActionNew::fillAmount() const
	{
		return _fillAmount;
	}

	double OrderActionNew::fillPrice() const
	{
		if (fillVolume() == 0) {
			return 0;
		}
		double multiple = 1.0;
		if (auto ptr = mts::InstrumentPropertyDb::instance()->find(instrumentId()))
		{
			multiple = ptr->volumeMultiple();
		}

		return fillAmount() / fillVolume() / multiple;
	}

	mts::PriceType OrderActionNew::priceType() const
	{
		return _priceType;
	}

	mts::DirectionSide OrderActionNew::directionSide() const
	{
		return _side;
	}

	mts::CombOffsetFlag OrderActionNew::offsetFlag() const
	{
		return _offset;
	}

	mts::TimeCondition OrderActionNew::timeCondition() const
	{
		return _condition;
	}
	
	int OrderActionNew::orderType() const
	{
		return _orderType;
	}

	QString OrderActionNew::createSrc() const
	{
		return _createSrc;
	}

	QVariantMap OrderActionNew::params() const
	{
		return _params;
	}

	const OrderId& OrderActionNew::referenceId() const
	{
		return _refId;
	}

	int OrderActionNew::strategyId () const{
		return _strategyId;
	}

	int OrderActionNew::instanceId() const {
		return _instanceId;
	}

	void OrderActionNew::setVolume(double v) //TODO check double
	{
		_volume = v;
	}

	void OrderActionNew::setPrice(double price)
	{
		_price = price;
	}
	void OrderActionNew::setFillVolume(double volume) //TODO check double
	{
		_fillVolume = volume;
	}

	void OrderActionNew::setFillAmount(double amt)
	{
		_fillAmount = amt;
	}

	void OrderActionNew::setPriceType(PriceType type)
	{
		_priceType = type;
	}

	void OrderActionNew::setDirectonSide(DirectionSide side)
	{
		_side = side;
	}

	void OrderActionNew::setOffsetFlag(CombOffsetFlag flag)
	{
		_offset = flag;
	}

	void OrderActionNew::setTimeCondition(TimeCondition tc)
	{
		_condition = tc;
	}

	void OrderActionNew::setOrderType(int type)
	{
		_orderType = type;
	}

	void OrderActionNew::setCreateSrc(const QString& src)
	{
		_createSrc = src;
	}

	void OrderActionNew::setParams(const QVariantMap& params)
	{
		_params = params;
	}

	void OrderActionNew::setReferenceId(const OrderId& id)
	{
		_refId = id;
	}

	void OrderActionNew::setInstanceId(int id)
	{
		_instanceId = id;
	}

	bool OrderActionNew::setStrategyId ( int id){
		assert ( id );
		if (!isStrategyIdValid(id)){
			return false;
		} 
		_strategyId = id;
		return true;
	}
	
	QJsonObject& OrderActionNew::toJson(QJsonObject& jsonObj) const
	{
		OrderAction::toJson(jsonObj);
		jsonObj.insert("volume", this->volume());
		jsonObj.insert("fillVolume", this->fillVolume());
		jsonObj.insert("leftVolume", this->leftVolume());
		jsonObj.insert("price", this->price());
		jsonObj.insert ("fillPrice" , this->fillPrice () );
		jsonObj.insert("fillAmount", this->fillAmount());
		jsonObj.insert("priceType", priceTypeName(this->priceType()));
		jsonObj.insert("directionSide", directionSideName(this->directionSide()));
		jsonObj.insert("offsetFlag", combOffsetFlagName(this->offsetFlag()));
		jsonObj.insert("timeCondition", timeConditionName(this->timeCondition()));
		//jsonObj.insert("orderType", orderTypeName(this->orderType()));
		jsonObj.insert("createSrc", this->createSrc());
		jsonObj.insert("referenceId", this->referenceId().toString());
		jsonObj.insert ( "strategyId" , this->strategyId () );
		return jsonObj;
	}

	QString OrderActionNew::description() const {
		return QString(LS("方向：%1，价格：%2，数量：%3")
							.arg(LS(directionSideDescription(this->directionSide())))
							.arg(this->price()).arg(this->volume())
		);
	}
	//=======================================
	OrderActionCancel::OrderActionCancel()
		:OrderActionNew(OrderStatus::OS_CANCEL_REQUEST)
	{

	}

	OrderActionCancel::~OrderActionCancel()
	{

	}

	////========================================
	//OrderActionReplace::OrderActionReplace()
	//	:OrderActionNew(OrderStatus::OS_REPLACE_REQUEST)
	//{

	//}

	//OrderActionReplace::~OrderActionReplace()
	//{

	//}

}
