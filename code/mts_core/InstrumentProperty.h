
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
#include<QtGlobal>
#include <QList>
#include "base/Currency.h"
#include "base/DateTime.h"
#include "InstrumentId.h"
#include "MethodDefine.h"

#define TradingSectionMaxCount 5

class QJsonObject;
namespace mts
{
	struct MTS_CORE_API TradingTimeRange
	{
		TradingTimeRange(int beginTm=0,int endTm=0,bool includeEndTime=false)
            :_includeEndTime(includeEndTime),beginTime(beginTm), endTime(endTm)
		{
		}
		bool isInSection(int tm) const {
			return tm >= beginTime && (_includeEndTime?(tm <= endTime):(tm<endTime));
		}
		bool operator==(const TradingTimeRange& ttr)const {
			return _includeEndTime == ttr._includeEndTime && beginTime == ttr.beginTime && endTime == ttr.endTime;
		}
	private:
        bool _includeEndTime;
        int beginTime; //hhmm
        int endTime;  //hhmm
	};

	typedef QList<TradingTimeRange> TradingSections;

	class MTS_CORE_API InstrumentBaseProperty
	{
	public:
		InstrumentBaseProperty();
		virtual ~InstrumentBaseProperty();

		virtual void dumpTo(QJsonObject*) const;
		virtual bool loadFrom(const QJsonObject&);

		bool isNull () const;

		const InstrumentId& instrumentId() const {
			return _instrumentId;
		}
		void setInstrumentId(const InstrumentId& id);
		MemberCopyMethodDefine (QString, name, setName);
		MemberCopyMethodDefine (double, volumeMultiple, setVolumeMultiple);//数量乘数
		MemberCopyMethodDefine (double, priceTick, setPriceTick);//最小变动价位
		MemberCopyMethodDefine(double, minOrderSize, setMinOrderSize);//最小 Order Size
		MemberCopyMethodDefine(double, orderSizeIncrement, setOrderSizeIncrement);//Order Size增长单位
		MemberCopyMethodDefine(double, pricePrecision, setPricePrecision);//价格精度
		MemberCopyMethodDefine(double, sizePrecision, setSizePrecision);//数量精度

		MemberMethodDefine(CurrencyId, currencyId, setCurrencyId);//货币代码
		MemberMethodDefine(TradingSections, tradingSection, setTradingSection);//交易小结
		MemberCopyMethodDefine (int, createDate, setCreateDate);//创建日

		MemberCopyMethodDefine(QString, prouduct, setProuduct);//品种
		MemberCopyMethodDefine(InstrumentKind, kind, setKind);//合约类别

		MemberCopyMethodDefine (QString, mtsSymbol, setMtsSymbol); //mts format symbol (AByyyy) , 品种大写，年份月份采用四位表示

	public:
		bool isTradingTimeSection(const DateTime&) const;
		QString productId() const;
		QString toJsonString () const;
	private:
		InstrumentId _instrumentId;
	};

	class MTS_CORE_API InstrumentProperty :public InstrumentBaseProperty
	{
	public:
		InstrumentProperty();
		virtual ~InstrumentProperty();

		virtual void dumpTo(QJsonObject* object) const override;
		virtual bool loadFrom(const QJsonObject&) override;

		static mts::InstrumentId convertMainInstument(const mts::InstrumentId&);

		MemberCopyMethodDefine (int, openDate, setOpenDate);//上市日
		MemberCopyMethodDefine (int, expireDate, setExpireDate);//到期日
		MemberCopyMethodDefine (int, startDelivDate, setStartDelivDate);//开始交割日
		MemberCopyMethodDefine (int, endDelivDate, setEndDelivDate); ;//结束交割日
	};
}
