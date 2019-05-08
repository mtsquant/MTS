
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
#include "ContinuousSymbolMgr.h"
#include "base/MtsLog.h"
#include <QtCore/QFile>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

namespace mts
{
	ContinuousSymbolMgr::ContinuousSymbolMgr()
	{
	}

	ContinuousSymbolMgr::~ContinuousSymbolMgr()
	{

	}

	mts::ContinuousSymbolMgr* ContinuousSymbolMgr::instance()
	{
		static ContinuousSymbolMgr mgr;
		return &mgr;
	}

	bool ContinuousSymbolMgr::init(const QString& configPath)
	{
		QFile file(configPath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			MTS_ERROR("can not open file: '%s'\n", qPrintable(configPath));
			return false;
		}
		QByteArray jsonStr = file.readAll();
		file.close();
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr, &jsonError);
		if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
			return false;
		}
		QJsonObject jsonObj = jsonDoc.object();
		QStringList mktList = jsonObj.keys();
		QStringList dateList;
		for (int i = 0, mktSize = mktList.size(); i < mktSize; ++i) {
			QJsonValueRef dateSymVal = jsonObj[mktList[i]];
			if (!dateSymVal.isObject()) {
				MTS_ERROR("invalid config in: '%s'\n", qPrintable(configPath));
				return false;
			}
			QJsonObject dateSymObj = dateSymVal.toObject();
			dateList = dateSymObj.keys();
			for (int j = 0, dateSize = dateList.size(); j < dateSize; ++j) {
				QJsonValueRef symVal = dateSymObj[dateList[j]];
				if (!symVal.isString()) {
					MTS_ERROR("invalid config in: '%s'\n", qPrintable(configPath));
					return false;
				}
				_mktDateSymMap[mktList[i]][dateList[j].toInt()] = qPrintable(symVal.toString());
			}
		}
		return true;
	}

	std::string ContinuousSymbolMgr::getSymbol(const std::string& mkt, int date) const
	{
		auto mktIt = _mktDateSymMap.find(QString::fromStdString(mkt).toUpper());
		if (mktIt == _mktDateSymMap.end()) {
			return "";
		}
		const DateSymMap& dateSymMap = mktIt.value();
		return getSymbol ( dateSymMap , date );
	}


	std::string ContinuousSymbolMgr::getSymbol ( const DateSymMap & dateSymMap , int date ) const{
		auto itDate = dateSymMap.lower_bound ( date );
		if ( itDate == dateSymMap.begin () ){
			if ( date != itDate->first ){
				return "";
			}
		} else if ( itDate == dateSymMap.end () ){
			if ( dateSymMap.size () > 0 ){
				--itDate;
			} else{
				return "";
			}
		} else{
			if ( date != itDate->first ){
				--itDate;
			}
		}
		return itDate->second;
	}

	QStringList ContinuousSymbolMgr::allMainSymbols ( int date ) const{
		QStringList  ids;
		for ( auto i = _mktDateSymMap.begin () , iend = _mktDateSymMap.end (); i != iend; ++i ){
			QString mkt = i.key ();
			ids << getSymbol ( i.value () , date ).c_str ();
		}
		return ids;
	}


	QStringList ContinuousSymbolMgr::allContinuousSymbols ( ) const{
		QStringList  ids;
		for ( auto i = _mktDateSymMap.begin () , iend = _mktDateSymMap.end (); i != iend; ++i ){
			QString mkt = i.key ();
			ids << mkt + CONTINUOUS_CONTRACT_SUFFIX;
		}
		return ids;
	}

	QString ContinuousSymbolMgr::getSymbolFromContinuous ( const QString & symbol /* AG888*/ , int date ){
		if (!InstrumentId::isContinuousSymbol ( symbol ) ){
			return "";
		}

		QString mkt = InstrumentId::toProduct ( symbol );
		return getSymbol ( qPrintable ( mkt ) ,date).c_str();
	}
	
}