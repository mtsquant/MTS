
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
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QSet>
#include "InstrumentProperty.h"


namespace mts
{
	class MTS_CORE_API InstrumentPropertyDb :public QObject
	{
		Q_OBJECT
	public:
		InstrumentPropertyDb();
		~InstrumentPropertyDb();
		static InstrumentPropertyDb*  instance();

		InstrumentId findInstrumentId(const QString& symbol) const; //from exch symbol
		bool hasMtsSymbol(const QString& mtsSym);

		bool insert(InstrumentBaseProperty*);   //The class will own the pointer,you do not delete it
		InstrumentBaseProperty* find(const InstrumentId& id) const;
		int size() const;
		bool dumpTo(const QString& filePath) const;
		bool loadFrom(const QString& filePath);


	public Q_SLOTS:
		QList<InstrumentId> instrumentIds(int beginDate , int endDate) const; //return all valid contract ids between beginDate and endDate
		QList<InstrumentId> instrumentIds()const;

	private:
		InstrumentId findInstrumentIdFromMtsSymbol(const QString& mtsSymbol, bool continuousSymbolRpl = true) const;
		void doInsert(InstrumentBaseProperty * prop);
		typedef QHash<InstrumentId, InstrumentBaseProperty*> InstrumentPropertySet;
		typedef QHash<QString, InstrumentBaseProperty*> SymPropertySet;

		mutable QMutex _locker;
		InstrumentPropertySet  _instrumentProperties;
		SymPropertySet   _symbolProperties;
		SymPropertySet   _mtsSymbolProperties; //mts format symbol (AByyyy) , 品种大小，年份月份采用四位表示
	};
}
