
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
#include <QtGlobal>
#include <QVariantMap>
#include "mts_core/InstrumentObject.h"
#include "mts_core/TradingAccount.h"
#include "OrderId.h"
#include "base/PerfTest.h"

class QJsonObject;
namespace mts {
#define  REFERENCE_INVALID_ID -99999
	class MTS_CORE_API OrderAction :public InstrumentObject
	{
	public:
		OrderAction(char type);
		OrderAction(const OrderAction&);
		virtual ~OrderAction();

		const TradingAccountId& tradingAccountId()const;
		char type()const;
		int book() const;
		int tradingDay()const;
		qint64 createTicksSinceEpoch()const;

		void setTradingAccountId(const TradingAccountId&);
		void setBook(int);
		void setTradingDay(int);
		void setCreateTicksSinceEpoch(qint64);
		virtual QJsonObject& toJson(QJsonObject& jsonObj) const;
		QString toJsonString() const;
		void setPerfNote(const QString&);
		QString perfNote()const;
	protected:
		const char _type;//状态类型
		TradingAccountId _accountId;
		int _book;
		int _tradingDay;//交易日
		qint64 _createTicksSinceEpoch;//创建时间
		QString _perfNote;
	};

	class MTS_CORE_API OrderActionNew:public OrderAction
	{
	public:
		OrderActionNew(char type=OrderStatus::OS_NEW_REQUEST);
		OrderActionNew(const OrderActionNew&);
		virtual ~OrderActionNew();
	
		double volume()const; //TODO check double
		double fillVolume() const; //TODO check double
		virtual double leftVolume() const; //TODO check double
		double price()const;
		double fillAmount() const;
		double fillPrice()const;
		PriceType priceType()const;
		DirectionSide directionSide()const;
		CombOffsetFlag offsetFlag()const;
		TimeCondition timeCondition()const;
		int orderType() const;
		QString createSrc()const;
		QVariantMap params()const;
		const OrderId& referenceId()const;
		int strategyId() const;
		int instanceId() const;

		void setVolume(double); //TODO check double
		void setPrice(double);
		void setFillVolume(double); //TODO check double
		void setFillAmount(double);
		void setPriceType(PriceType);
		void setDirectonSide(DirectionSide);
		void setOffsetFlag(CombOffsetFlag);
		void setTimeCondition(TimeCondition);
		void setOrderType(int);
		void setCreateSrc(const QString&);
		void setParams(const QVariantMap&);
		void setReferenceId(const OrderId&);
		void setInstanceId(int);
		bool setStrategyId ( int ); /*1~9*/

		virtual QJsonObject& toJson(QJsonObject& jsonObj) const override;

		QString description() const;
	protected:
		double _volume; //TODO check double
		double _fillVolume; //TODO check double
		double _price;
		double _fillAmount;
		PriceType _priceType;
		DirectionSide _side;
		CombOffsetFlag _offset;
		TimeCondition _condition;
		int _orderType;
		QString _createSrc;
		QVariantMap _params;
		OrderId _refId;

		int _strategyId; /*1~9*/
		int _instanceId; /*1~9*/
	};

	class MTS_CORE_API OrderActionCancel:public OrderActionNew
	{
	public:
		OrderActionCancel();
		virtual ~OrderActionCancel();

	};

	/*class OrderActionReplace:public OrderActionNew
	{
	public:
		OrderActionReplace();
		virtual ~OrderActionReplace();

	};*/
}
