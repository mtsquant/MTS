
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
#include "InstrumentId.h"
#include<QtCore/QStringList>
#include "InstrumentPropertyDb.h"

namespace mts
{

	InstrumentId::InstrumentId()
		:typeId(InstrumentType::TYPE_UNKNOWN)
		,exchId(ExchId::EXCH_UNKNOWN)
	{
	}

	InstrumentId::InstrumentId(const QString & sym, InstrumentType type/*, ExchId exch*/)
		:typeId(type),symbol(sym)
		,exchId(InstrumentPropertyDb::instance()->findInstrumentId(sym).exchId)
	{
	}


	InstrumentId::InstrumentId(const QString& symbol, InstrumentType type, ExchId exchId)
		: symbol ( symbol )
		, typeId ( type )
		, exchId ( exchId )
	{

	}

	InstrumentId::InstrumentId(const InstrumentId & id, const QString & newSymbol)
		: symbol(newSymbol)
		, typeId(id.typeId)
		, exchId(id.exchId) 
	{
	}


	InstrumentId::~InstrumentId() {
	}

	bool InstrumentId::isNull() const {
		return this->typeId == InstrumentType::TYPE_UNKNOWN 
			&& this->exchId == ExchId::EXCH_UNKNOWN 
			&& this->symbol.isNull();
	}


	QString InstrumentId::toString() const {
		return QString("%1.%2.%3")
			.arg(symbol)
			.arg(instrumentTypeName(typeId))
			.arg(exchIdName(exchId))
			;
	}

	bool InstrumentId::fromString(const QString & idStr) {
		auto sections = idStr.split(QChar('.'));
		if (sections.size() < 2) {
			return false;
		}
		symbol = sections[0];
		typeId = instrumentType(qPrintable(sections[1]));
		exchId =  (sections.size() >= 3)? mts::exchId(qPrintable(sections[2])):InstrumentPropertyDb::instance()->findInstrumentId(symbol).exchId;
		return true;
	}


	QString InstrumentId::toProduct(const QString & symbol) {
		if (symbol.size() > 0) {
			int i = 0;
			for (i = 0; i < symbol.size(); i++) {
				if (symbol.at(i).isNumber()) {
					break;
				}
			}
			return symbol.left(i);
		}
		return symbol;
	}

	QString InstrumentId::toContinuousSymbol(const QString & symbol) {
		return toProduct(symbol) + CONTINUOUS_CONTRACT_SUFFIX;
	}

	bool InstrumentId::isContinuousSymbol(const QString & symbol) {
		static const QString ccs(CONTINUOUS_CONTRACT_SUFFIX);
		return symbol.endsWith(ccs);
    }

    bool operator==(const InstrumentId &e1, const InstrumentId &e2)
    {
        return e1.symbol == e2.symbol
                && e1.exchId == e2.exchId
                && e1.typeId == e2.typeId;
    }

    uint qHash(const InstrumentId &key, uint seed)
    {
        return qHash ( key.symbol ) ^ (key.exchId*1000+key.typeId); //for performance
    }

}
