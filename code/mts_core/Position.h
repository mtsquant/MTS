
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
#include <QtCore/QSharedPointer>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

#include "Order.h"
#include "mts_core/Quote.h"
#include "mts_core/InstrumentProperty.h"

namespace mts
{

	struct VolumeInfo {
		double open; //TODO check double
		double current; //TODO check double
		double active; //TODO check double
		double close;//平仓数量 //TODO check double
		double activeClose; //TODO check double
		double closeYesterdayVolume;//上期:平昨仓数量;其它为优先记作平昨仓 //TODO check double
		double activeCloseYesterdayVolume; //TODO check double
		double fillVolume; //TODO check double
		double fillAmount;
		double openPnl;
		double fillPnl;
		void clear() {
			memset(this, 0, sizeof(VolumeInfo));
		}
		void businessDateChanged(int businessDate) {
			double tmpCurrent = current; //TODO check double
			clear();
			open = tmpCurrent;
			current = tmpCurrent;
		}
		QString toString() const {
			QString str;
			str.sprintf("open=%lf,current=%lf,active=%lf,close=%lf,activeClose=%lf,closeYesterdayVolume=%lf,activeCloseYesterdayVolume=%lf,fillVolume=%lf,fillAmount=%lf,openPnl=%lf,fillPnl=%lf\n",
				open,
				current,
				active,
				close,
				activeClose,
				closeYesterdayVolume,
				activeCloseYesterdayVolume,
				fillVolume,
				fillAmount,
				openPnl,
				fillPnl); //TODO check format
			return str;
		}
		void toJson(QJsonObject& obj)const {
			obj.insert("open", open);
			obj.insert("current", current);
			obj.insert("active", active);
			obj.insert("close", close);
			obj.insert("activeClose", activeClose);
			obj.insert("closeYesterdayVolume", closeYesterdayVolume);
			obj.insert("activeCloseYesterdayVolume", activeCloseYesterdayVolume);
			obj.insert("fillVolume", fillVolume);
			obj.insert("fillAmount", fillAmount);
			obj.insert("openPnl", openPnl);
			obj.insert("fillPnl", fillPnl);
		}
		void fromJson(const QJsonObject& obj) {
			open = obj.value("open").toDouble(); //TODO check double
			current = obj.value("current").toDouble(); //TODO check double
			active = obj.value("active").toDouble(); //TODO check double
			close = obj.value("close").toDouble(); //TODO check double
			activeClose = obj.value("activeClose").toDouble(); //TODO check double
			closeYesterdayVolume = obj.value("closeYesterdayVolume").toDouble(); //TODO check double
			activeCloseYesterdayVolume = obj.value("activeCloseYesterdayVolume").toDouble(); //TODO check double
			fillVolume = obj.value("fillVolume").toDouble(); //TODO check double
			fillAmount = obj.value("fillAmount").toDouble();
			openPnl = obj.value("openPnl").toDouble();
			fillPnl = obj.value("fillPnl").toDouble();
		}
	};

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
		VolumeInfo _longInfo;
		VolumeInfo _shortInfo;
		mutable InstrumentBaseProperty* _cachedInstrumentProperty;
	};
	typedef QSharedPointer<Position> PositionPtr;
}
Q_DECLARE_METATYPE(mts::PositionPtr);
