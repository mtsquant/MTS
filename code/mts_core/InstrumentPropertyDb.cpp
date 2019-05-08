
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
#include "InstrumentPropertyDb.h"
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include "base/MtsLog.h"

#include "TradingDateMgr.h"
#include "ContinuousSymbolMgr.h"

namespace mts
{

	InstrumentPropertyDb::InstrumentPropertyDb() 
		:_locker(QMutex::Recursive)
	{

	}


	InstrumentPropertyDb::~InstrumentPropertyDb() {
		QMutexLocker l(&_locker);
		qDeleteAll(_instrumentProperties);
	}

	InstrumentPropertyDb * InstrumentPropertyDb::instance() {
		static InstrumentPropertyDb db;
		return &db;
	}

	bool InstrumentPropertyDb::hasMtsSymbol(const QString & mtsSym)
	{
		QMutexLocker l(&_locker);
		return _mtsSymbolProperties.contains(mtsSym);
	}

	bool InstrumentPropertyDb::insert(InstrumentBaseProperty * prop) {
		QMutexLocker l(&_locker);
		if (_mtsSymbolProperties.contains(prop->mtsSymbol())) {
			return false;
		}
		doInsert(prop);
		return true;
	}

	InstrumentBaseProperty * InstrumentPropertyDb::find(const InstrumentId & id) const {
		QMutexLocker l(&_locker);
		InstrumentBaseProperty * ibp= _instrumentProperties[id];
		if (ibp == nullptr) {
			InstrumentId rawId = findInstrumentIdFromMtsSymbol(id.symbol,false);
			if (rawId.isNull()) {
				return nullptr;
			} else {
				return _instrumentProperties[rawId];
			}
		} else {
			return _instrumentProperties[id];
		}
	}

	int InstrumentPropertyDb::size() const {
		QMutexLocker l(&_locker);
		return _instrumentProperties.size();
	}

	bool InstrumentPropertyDb::dumpTo(const QString& filePath) const
	{
		QMutexLocker l(&_locker);
		QJsonArray jsonArr;
		for (auto it = _instrumentProperties.constBegin(), itEnd = _instrumentProperties.constEnd(); it != itEnd; ++it) {
			InstrumentBaseProperty* prop = it.value();
			QJsonObject obj;
			prop->dumpTo(&obj);
			jsonArr.append(obj);
		}
		QJsonDocument jsonDoc;
		jsonDoc.setArray(jsonArr);
		QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Indented);
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			return false;
		}
		file.write(byteArray);
		return true;
	}

	bool InstrumentPropertyDb::loadFrom(const QString& filePath)
	{
		MTS_FILE("Load file '%s'\n" , qPrintable(filePath));
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return false;
		}
		QByteArray jsonStr = file.readAll();
		if (jsonStr.isEmpty()) {
			return false;
		}

		QMutexLocker l(&_locker);
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr, &jsonError);
		if (jsonError.error == QJsonParseError::NoError && jsonDoc.isArray()){
			QJsonArray jsonArray = jsonDoc.array();
			for (int i = 0,size= jsonArray.size(); i < size;++i) {
				const QJsonValue& val = jsonArray.at(i);
				if (!val.isObject()) {
					return false;
				}
				QJsonObject obj = val.toObject();

				mts::InstrumentProperty* instrumentProperty = new mts::InstrumentProperty();
				if (!instrumentProperty->loadFrom(obj)) {
					return false;
				}
				this->insert(instrumentProperty);
			}
			return true;
		} else {
			MTS_ERROR("Failed to read '%s' : %s\n", qPrintable(filePath),qPrintable(jsonError.errorString()));
			return false;
		}
	}

	QList<InstrumentId> InstrumentPropertyDb::instrumentIds(int beginDate , int endDate) const{
		QMutexLocker l(&_locker);
		QList<InstrumentId> ids;
		for (auto it = _instrumentProperties.constBegin(), itEnd = _instrumentProperties.constEnd(); it != itEnd; ++it) {
			InstrumentBaseProperty* property = it.value();
			if (auto* prop = dynamic_cast<InstrumentProperty*>(property)) {
				if (prop->openDate() <= endDate && prop->expireDate() >= beginDate) {
					ids.append(property->instrumentId());
				}
			}
		}
		return ids;
	}


	InstrumentId InstrumentPropertyDb::findInstrumentId(const QString & symbol) const {
		QString realSymbol = symbol;
		bool isContinuous=InstrumentId::isContinuousSymbol(symbol);
		if (isContinuous) {
			assert(!ContinuousSymbolMgr::instance()->allContinuousSymbols().isEmpty());
			realSymbol=ContinuousSymbolMgr::instance()->getSymbolFromContinuous(symbol,DateTime::now().date());
		}

		QMutexLocker l(&_locker);
		auto it = _symbolProperties.constFind(realSymbol);
		if (it != _symbolProperties.constEnd()) {
			auto id= it.value()->instrumentId();
			if (isContinuous) {
				id.symbol = symbol;
			}
			return id;
		}
		auto id= findInstrumentIdFromMtsSymbol(realSymbol);
		if (isContinuous) {
			id.symbol = symbol;
		}
		return id;
	}

	InstrumentId InstrumentPropertyDb::findInstrumentIdFromMtsSymbol(const QString & mtsSymbol,bool continuousSymbolRpl) const
	{
        QString realSymbol = mtsSymbol;
		bool isContinuous = InstrumentId::isContinuousSymbol(mtsSymbol);
        if (isContinuous) {
            assert(!ContinuousSymbolMgr::instance()->allContinuousSymbols().isEmpty());
            realSymbol=ContinuousSymbolMgr::instance()->getSymbolFromContinuous(mtsSymbol,DateTime::now().date());
        }
        QMutexLocker l(&_locker);
        auto it = _mtsSymbolProperties.constFind(realSymbol.toUpper());
		if (it != _mtsSymbolProperties.constEnd()) {
			auto id=it.value()->instrumentId();
			if (continuousSymbolRpl&&isContinuous) {
				id.symbol = mtsSymbol;
			}
			return id;
		}
            MTS_ERROR("can not find instrument id for mts symbol: %s\n", qPrintable(mtsSymbol.toUpper()));
            return InstrumentId();
	}

	QList<mts::InstrumentId> InstrumentPropertyDb::instrumentIds() const
	{
		QMutexLocker l(&_locker);
		return _instrumentProperties.keys();
	}
	
	void InstrumentPropertyDb::doInsert(InstrumentBaseProperty * prop)
	{
		_instrumentProperties.insert(prop->instrumentId(), prop);
		_symbolProperties.insert(prop->instrumentId().symbol, prop);
		_mtsSymbolProperties.insert(prop->mtsSymbol(), prop);
	}

}
