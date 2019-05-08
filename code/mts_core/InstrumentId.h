
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
#include "enums.h"
#include <QtCore/QString>
#include <QtCore/QHash>
#include "mts_core_api.h"

namespace mts
{//暂时还只要symbol与type，因为行情不一定有Exch（郑州和大连商品交易所）
	class MTS_CORE_API InstrumentId
	{
	public:
		InstrumentId();
		InstrumentId(const QString& symbol, InstrumentType/*, ExchId*/);
		InstrumentId(const QString& symbol, InstrumentType type,ExchId exchId);
		InstrumentId(const InstrumentId&, const QString& newSymbol); //use new symbol to gen an new InstrumentId
		~InstrumentId();
		QString symbol;//商品代码
		InstrumentType typeId;//商品类别
		ExchId exchId;//交易市场

		QString toString() const;
		bool fromString(const QString& idStr);
		bool isNull() const;

		static QString toProduct(const QString& symbol);
		static QString toContinuousSymbol(const QString& symbol);
		static bool isContinuousSymbol(const QString& symbol);

	};
	MTS_CORE_API bool operator==(const InstrumentId& e1, const InstrumentId& e2);
	MTS_CORE_API uint qHash(const InstrumentId& key, uint seed=0);
}
