
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
#include <QtCore/QSharedPointer>

#include <QtCore/QJsonDocument>

#include "Order.h"
#include "mts_core/Quote.h"
#include "mts_core/InstrumentProperty.h"
#include "mts_core/PositionVolumeInfo.h"

namespace mts
{



	class MTS_CORE_API Position :public InstrumentObject {
	public:
		Position(const InstrumentId& id = InstrumentId());
		~Position();

		bool isNull() const;

		double longOpenVolume() const;							//多头开盘仓位 //TODO check double
		double shortOpenVolume() const;						//空头开盘仓位 //TODO check double
		double longCurrentVolume() const;						//多头持仓 //TODO check double
		double shortCurrentVolume() const;						//空头持仓 //TODO check double
		double longActiveVolume() const;						//多头正在报单仓量 //TODO check double
		double shortActiveVolume() const;						//空头正在报单仓量 //TODO check double
		double longCloseVolume() const;							//多头平仓量 //TODO check double
		double shortCloseVolume() const;						//空头平仓量 //TODO check double
		double longActiveCloseVolume() const;					//多头正在报单平仓量 //TODO check double
		double shortActiveCloseVolume() const;				//空头正在报单平仓量 //TODO check double
		double longCloseYesterdayVolume() const;			//多头平昨仓量 //TODO check double
		double shortCloseYesterdayVolume() const;			//空头平昨仓量 //TODO check double
		double longActiveCloseYesterdayVolume() const;	//多头正在报单平昨仓量 //TODO check double
		double shortActiveCloseYesterdayVolume() const;	//空头正在报单平昨仓量 //TODO check double
		double longLeftVolume()const;							//多头可平仓量 //TODO check double
		double longLeftYesterdayVolume()const;				//多头剩余昨仓 //TODO check double
		double shortLeftVolume()const;							//空头可平仓量 //TODO check double
		double shortLeftYesterdayVolume()const;				//空头剩余昨仓 //TODO check double
		double longFillVolume() const;								//多头成交仓量 //TODO check double
		double shortFillVolume() const;							//空头成交仓量 //TODO check double
		double longFillAmount() const;						//多头成交金额
		double shortFillAmount() const;						//空头成交金额
		double longOpenPnl() const;							//多头开盘盈亏
		double shortOpenPnl() const;						//空头开盘盈亏
		double longFillPnl() const;								//多头成交盈亏
		double shortFillPnl() const;							//空头成交盈亏
		double openPnl() const;								//开盘持仓盈亏
		double fillPnl() const;									//当日成交盈亏
		double totoalPnl() const;								//总和盈亏

		double preClosePrice()const;
		double preSettlementPrice()const;
		double lastPrice()const;

		void setLongOpenVolume(double); //TODO check double
		void setShortOpenVolume(double); //TODO check double
		void setLongCurrentVolume(double); //TODO check double
		void setShortCurrentVolume(double); //TODO check double
		void setLongActiveVolume(double); //TODO check double
		void setShortActiveVolume(double); //TODO check double
		void setLongCloseVolume(double); //TODO check double
		void setShortCloseVolume(double); //TODO check double
		void setLongActiveCloseVolume(double); //TODO check double
		void setShortActiveCloseVolume(double); //TODO check double
		void setLongCloseYesterdayVolume(double); //TODO check double
		void setShortCloseYesterdayVolume(double); //TODO check double
		void setLongActiveCloseYesterdayVolume(double); //TODO check double
		void setShortActiveCloseYesterdayVolume(double); //TODO check double
		void setLongFillVolume(double); //TODO check double
		void setShortFillVolume(double); //TODO check double
		void setLongFillAmount(double);
		void setShortFillAmount(double);
		void setLongOpenPnl(double);
		void setShortOpenPnl(double);
		void setLongFillPnl(double);
		void setShortFillPnl(double);

		void processPositionSnapshot(bool isLong, double yesterdayVolume); //TODO check double
		void processOrder(Order * order, Order* lastOrder);
		void processQuote(QuoteSnapshotPtr);
		const InstrumentBaseProperty* attribute()const;

		Position* clone() const;
		void businessDateChanged(int businessDate);

		QString toJsonString() const;
		QJsonObject toJson() const;

		void clear();
		static Position* fromJsonString(const QJsonObject&);
		static void registerMetaType();

	private:
		double _lastPrice;
		double _prePrice;
		double _preSettlementPrice;
		PositionVolumeInfo _longInfo;
		PositionVolumeInfo _shortInfo;
		mutable InstrumentBaseProperty* _cachedInstrumentProperty;

	};
	typedef QSharedPointer<Position> PositionPtr;
}
Q_DECLARE_METATYPE(mts::PositionPtr);
