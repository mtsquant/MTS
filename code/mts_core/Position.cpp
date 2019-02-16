
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
#include "Position.h"
#include "base/MtsLog.h"
#include "mts_core/InstrumentPropertyDb.h"

namespace mts
{

	Position::Position(const InstrumentId& id)
		:InstrumentObject(id), _lastPrice(0), _prePrice(0),_preSettlementPrice(0),
		_cachedInstrumentProperty(nullptr) 
	{
		memset(&_longInfo, 0, sizeof(_longInfo));
		memset(&_shortInfo, 0, sizeof(_shortInfo));
	}

	Position::~Position() {

	}

	bool Position::isNull() const {
		static VolumeInfo nullVolumeInfo;
		if (memcmp(&_longInfo, &nullVolumeInfo, sizeof(VolumeInfo)) != 0) {
			return false;
		}
		if (memcmp(&_shortInfo, &nullVolumeInfo, sizeof(VolumeInfo)) != 0) {
			return false;
		}

		return true;
	}




	double Position::longOpenVolume() const { //TODO check double
		return _longInfo.open;
	}

	void Position::setLongOpenVolume(double vlm) { //TODO check double
		_longInfo.open = vlm;
	}

	double Position::shortOpenVolume() const { //TODO check double
		return _shortInfo.open;
	}

	void Position::setShortOpenVolume(double vlm) { //TODO check double
		_shortInfo.open = vlm;
	}

	double Position::longCurrentVolume() const { //TODO check double
		return _longInfo.current;
	}

	void Position::setLongCurrentVolume(double vlm) { //TODO check double
		_longInfo.current = vlm;
	}

	double Position::shortCurrentVolume() const { //TODO check double
		return _shortInfo.current;
	}

	void Position::setShortCurrentVolume(double vlm) { //TODO check double
		_shortInfo.current = vlm;
	}

	double Position::longActiveVolume() const { //TODO check double
		return _longInfo.active;
	}

	void Position::setLongActiveVolume(double vlm) { //TODO check double
		_longInfo.active = vlm;
	}

	double Position::shortActiveVolume() const { //TODO check double
		return _shortInfo.active;
	}

	void Position::setShortActiveVolume(double vlm) { //TODO check double
		_shortInfo.active = vlm;
	}

	double Position::longCloseVolume() const { //TODO check double
		return _longInfo.close;
	}

	void Position::setLongCloseVolume(double vlm) { //TODO check double
		_longInfo.close = vlm;
	}

	double Position::shortCloseVolume() const { //TODO check double
		return _shortInfo.close;
	}

	void Position::setShortCloseVolume(double vlm) { //TODO check double
		_shortInfo.close = vlm;
	}

	double Position::longActiveCloseVolume() const { //TODO check double
		return _longInfo.activeClose;
	}

	void Position::setLongActiveCloseVolume(double vlm) { //TODO check double
		_longInfo.activeClose = vlm;
	}

	double Position::shortActiveCloseVolume() const { //TODO check double
		return _shortInfo.activeClose;
	}

	void Position::setShortActiveCloseVolume(double vlm) { //TODO check double
		_shortInfo.activeClose = vlm;
	}

	double Position::longCloseYesterdayVolume() const { //TODO check double
		return _longInfo.closeYesterdayVolume;
	}

	void Position::setLongCloseYesterdayVolume(double vlm) { //TODO check double
		_longInfo.closeYesterdayVolume = vlm;
	}

	double Position::shortCloseYesterdayVolume() const { //TODO check double
		return _shortInfo.closeYesterdayVolume;
	}

	void Position::setShortCloseYesterdayVolume(double vlm) { //TODO check double
		_shortInfo.closeYesterdayVolume = vlm;
	}

	double Position::longActiveCloseYesterdayVolume() const { //TODO check double
		return _longInfo.activeCloseYesterdayVolume;
	}

	void Position::setLongActiveCloseYesterdayVolume(double vlm) { //TODO check double
		_longInfo.activeCloseYesterdayVolume = vlm;
	}

	double Position::shortActiveCloseYesterdayVolume() const { //TODO check double
		return _shortInfo.activeCloseYesterdayVolume;
	}

	double Position::longLeftVolume() const { //TODO check double
		return longCurrentVolume() + longActiveCloseVolume();
	}

	double Position::longLeftYesterdayVolume() const { //TODO check double
		return longOpenVolume() + longCloseYesterdayVolume() + longActiveCloseYesterdayVolume();
	}

	double Position::shortLeftVolume() const { //TODO check double
		return shortCurrentVolume() + shortActiveCloseVolume();
	}

	double Position::shortLeftYesterdayVolume() const { //TODO check double
		return shortOpenVolume() + shortCloseYesterdayVolume() + shortActiveCloseYesterdayVolume();
	}

	void Position::setShortActiveCloseYesterdayVolume(double vlm) { //TODO check double
		_shortInfo.activeCloseYesterdayVolume = vlm;
	}

	double Position::longFillVolume() const { //TODO check double
		return _longInfo.fillVolume;
	}

	void Position::setLongFillVolume(double vlm) { //TODO check double
		_longInfo.fillVolume = vlm;
	}

	double Position::shortFillVolume() const { //TODO check double
		return _shortInfo.fillVolume;
	}

	void Position::setShortFillVolume(double vlm) { //TODO check double
		_shortInfo.fillVolume = vlm;
	}

	double Position::longFillAmount() const {
		return _longInfo.fillAmount;
	}

	void Position::setLongFillAmount(double amount) {
		_longInfo.fillAmount = amount;
	}

	double Position::shortFillAmount() const {
		return _shortInfo.fillAmount;
	}

	void Position::setShortFillAmount(double amount) {
		_shortInfo.fillAmount = amount;
	}

	double Position::longOpenPnl() const {
		return _longInfo.openPnl;
	}

	void Position::setLongOpenPnl(double pnl) {
		_longInfo.openPnl = pnl;
	}

	double Position::shortOpenPnl() const {
		return _shortInfo.openPnl;
	}

	void Position::setShortOpenPnl(double pnl) {
		_shortInfo.openPnl = pnl;
	}

	double Position::longFillPnl() const {
		return _longInfo.fillPnl;
	}

	void Position::setLongFillPnl(double pnl) {
		_longInfo.fillPnl = pnl;
	}

	double Position::shortFillPnl() const {
		return _shortInfo.fillPnl;
	}

	void Position::setShortFillPnl(double pnl) {
		_shortInfo.fillPnl = pnl;
	}

	double Position::openPnl() const {
		return longOpenPnl() + shortOpenPnl();
	}

	double Position::fillPnl() const {
		return longFillPnl() + shortFillPnl();
	}

	double Position::totoalPnl() const {
		return openPnl() + fillPnl();
	}

	double Position::preClosePrice() const
	{
		return _prePrice;
	}

	double Position::preSettlementPrice() const
	{
		return _preSettlementPrice;
	}

	double Position::lastPrice() const
	{
		return _lastPrice;
	}

	bool isCombOffsetYesterdy(mts::CombOffsetFlag flag) {
		return flag == mts::CombOffsetFlag::OF_CLOSEYESTERDAY || flag == mts::CombOffsetFlag::OF_CLOSE;
	}
	void Position::processPositionSnapshot(bool isLong, double yesterdayVolume) {//只记录昨仓，其它仓位信息，通过order，计算出来。所以要求，query的pos，order必须有顺序。 //TODO check double
		if (isLong) {
			setLongOpenVolume(yesterdayVolume);
			setLongCurrentVolume(yesterdayVolume);
		} else {
			setShortOpenVolume(-yesterdayVolume);
			setShortCurrentVolume(-yesterdayVolume);
		}
		MTS_FILE("Position:%s\n" , qUtf8Printable(this->toJsonString()));
	}

	void Position::processOrder(Order * order, Order* lastOrder) {
		double fillVolume = order->fillVolume(); //TODO check double
		double leftVolume = order->leftVolume(); //TODO check double
		double lastFillVolume = 0; //TODO check double
		double lastLeftVolume = 0; //TODO check double
		bool isFinished = order->isFinished();
		bool isLastFinished = false;
		double fillAmountDelta = order->fillAmount();
		if (lastOrder) {
			lastFillVolume = lastOrder->fillVolume();
			lastLeftVolume = lastOrder->leftVolume();
			isLastFinished = lastOrder->isFinished();
			fillAmountDelta -= lastOrder->fillAmount();
		}

		switch (order->directionSide()) {
		case DirectionSide::D_BUY:
		{
			setLongCurrentVolume(longCurrentVolume() + fillVolume - lastFillVolume);
			setLongFillVolume(longFillVolume() + fillVolume - lastFillVolume);
			setLongFillAmount(longFillAmount() + fillAmountDelta);
			if (isFinished) {
				if (!isLastFinished) {
					setLongActiveVolume(longActiveVolume() - lastLeftVolume);
				}
			} else {
				setLongActiveVolume(longActiveVolume() + leftVolume - lastLeftVolume);
			}
		}
		break;
		case DirectionSide::D_COVER:
		{
			setShortCurrentVolume(shortCurrentVolume() + (fillVolume - lastFillVolume));
			setShortCloseVolume(shortCloseVolume() + (fillVolume - lastFillVolume));
			setShortFillVolume(shortFillVolume() + (fillVolume - lastFillVolume));
			setShortFillAmount(shortFillAmount() + fillAmountDelta);
			if (exchHasBothPos(order->instrumentId().exchId)) {
				if (isCombOffsetYesterdy(order->offsetFlag())) {
					setShortCloseYesterdayVolume(shortCloseYesterdayVolume() + std::min(abs(shortOpenVolume() + shortCloseYesterdayVolume()), (fillVolume - lastFillVolume)));
				}
			} else if (shortOpenVolume() + shortCloseYesterdayVolume() < 0) {
				setShortCloseYesterdayVolume(shortCloseYesterdayVolume() + std::min(abs(shortOpenVolume() + shortCloseYesterdayVolume()), (fillVolume - lastFillVolume)));
			}
			if (isFinished&&!isLastFinished) {
				setShortActiveVolume(shortActiveVolume() - lastLeftVolume);
				setShortActiveCloseVolume(shortActiveCloseVolume() - lastLeftVolume);
				if (exchHasBothPos(order->instrumentId().exchId)) {
					if (isCombOffsetYesterdy(order->offsetFlag())) {
						setShortActiveCloseYesterdayVolume(shortActiveCloseYesterdayVolume() - lastLeftVolume);
					}
				}
			} else {
				setShortActiveVolume(shortActiveVolume() + (leftVolume - lastLeftVolume));
				setShortActiveCloseVolume(shortActiveCloseVolume() + (leftVolume - lastLeftVolume));
				if (exchHasBothPos(order->instrumentId().exchId)) {
					if (isCombOffsetYesterdy(order->offsetFlag())) {
						setShortActiveCloseYesterdayVolume(shortActiveCloseYesterdayVolume() + (leftVolume - lastLeftVolume));
					}
				}
			}
		}
		break;
		case DirectionSide::D_SHORT:
		{
			setShortCurrentVolume(shortCurrentVolume() + (fillVolume - lastFillVolume));
			setShortFillVolume(shortFillVolume() + (fillVolume - lastFillVolume));
			setShortFillAmount(shortFillAmount() + fillAmountDelta);
			if (isFinished) {
				if (!isLastFinished) {
					setShortActiveVolume(shortActiveVolume() - lastLeftVolume);
				}
			} else {
				setShortActiveVolume(shortActiveVolume() +(leftVolume - lastLeftVolume));
			}
		}
		break;
		case DirectionSide::D_SELL:
		{
			setLongCurrentVolume(longCurrentVolume()+ (fillVolume - lastFillVolume));
			setLongCloseVolume(longCloseVolume() + (fillVolume - lastFillVolume));
			setLongFillVolume(longFillVolume() + (fillVolume - lastFillVolume));
			setLongFillAmount(longFillAmount() + fillAmountDelta);
			if (exchHasBothPos(order->instrumentId().exchId)) {
				if (isCombOffsetYesterdy(order->offsetFlag())) {
					setLongCloseYesterdayVolume(longCloseYesterdayVolume() + std::max(-(longOpenVolume() + longCloseYesterdayVolume()), (fillVolume - lastFillVolume)));
				}
			} else if (longOpenVolume() + longCloseYesterdayVolume() > 0) {
				setLongCloseYesterdayVolume(longCloseYesterdayVolume() + std::max(-(longOpenVolume() + longCloseYesterdayVolume()), (fillVolume - lastFillVolume)));
			}
			if (isFinished&&!isLastFinished) {
				setLongActiveVolume(longActiveVolume() - lastLeftVolume);
				setLongActiveCloseVolume(longActiveCloseVolume() - lastLeftVolume);
				if (exchHasBothPos(order->instrumentId().exchId)) {
					if (isCombOffsetYesterdy(order->offsetFlag())) {
						setLongActiveCloseYesterdayVolume(longActiveCloseYesterdayVolume() - lastLeftVolume);
					}
				}
			} else {
				setLongActiveVolume(longActiveVolume() + (leftVolume - lastLeftVolume));
				setLongActiveCloseVolume(longActiveCloseVolume() + (leftVolume - lastLeftVolume));
				if (exchHasBothPos(order->instrumentId().exchId)) {
					if (isCombOffsetYesterdy(order->offsetFlag())) {
						setLongActiveCloseYesterdayVolume(longActiveCloseYesterdayVolume() + (leftVolume - lastLeftVolume));
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}

	void Position::processQuote(QuoteSnapshotPtr qt) {
		double volumeMultiple = 1.0;
		if (const mts::InstrumentBaseProperty* instrumentProperty = attribute()) {
			volumeMultiple = instrumentProperty->volumeMultiple();
		}
		_lastPrice = qt->lastPrice();
		_preSettlementPrice = qt->preSettlementPrice();

		_prePrice = qt->preClosePrice();
		setLongOpenPnl(volumeMultiple*longOpenVolume()*(_lastPrice - _prePrice));
		setShortOpenPnl(volumeMultiple*shortOpenVolume()*(_lastPrice - _prePrice));
		setLongFillPnl(volumeMultiple*longFillVolume()*_lastPrice - longFillAmount());
		setShortFillPnl(volumeMultiple*shortFillVolume()*_lastPrice - shortFillAmount());
	}

	
	const InstrumentBaseProperty* Position::attribute() const {
		if (_cachedInstrumentProperty==nullptr) {
			_cachedInstrumentProperty = InstrumentPropertyDb::instance()->find(this->instrumentId());
		}
		return _cachedInstrumentProperty;
	}


	Position * Position::clone() const {
		return new Position(*this);
	}

	void Position::businessDateChanged(int businessDate)
	{
		_longInfo.businessDateChanged(businessDate);
		_shortInfo.businessDateChanged(businessDate);
	}

	QString Position::toJsonString() const
	{
		return QJsonDocument(toJson()).toJson().simplified();
	}

	QJsonObject Position::toJson() const
	{
		QJsonObject obj;
		obj.insert("instumentId", this->instrumentId().toString());
		{
			QJsonObject lObj;
			_longInfo.toJson(lObj);
			obj.insert("long", lObj);
		}
		{
			QJsonObject sObj;
			_shortInfo.toJson(sObj);
			obj.insert("short", sObj);
		}
		obj.insert("preClosePrice", preClosePrice());
		obj.insert("preSettlementPrice", preSettlementPrice());
		obj.insert("lastPrice", lastPrice());
		return obj;
	}

	void Position::clear()
	{
		_longInfo.clear();
		_shortInfo.clear();
	}

	mts::Position* Position::fromJsonString(const QJsonObject& obj)
	{
		mts::Position* pos = new mts::Position();
		mts::InstrumentId id;
		id.fromString(obj.value("instumentId").toString());
		pos->setInstrumentId(id);
		pos->_longInfo.fromJson(obj.value("long").toObject());
		pos->_shortInfo.fromJson(obj.value("short").toObject());
		return pos;
	}

	void Position::registerMetaType() {
		qRegisterMetaType<Position>("Position");
		qRegisterMetaType<PositionPtr>("PositionPtr");
	}

}