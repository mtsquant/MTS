
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
#include "mts_core/mts_core_api.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <map>
#include "mts_core/InstrumentId.h"

namespace mts
{
	class MTS_CORE_API ContinuousSymbolMgr
	{
	private:
		ContinuousSymbolMgr();
	public:
		~ContinuousSymbolMgr();
		static ContinuousSymbolMgr* instance();
		bool init(const QString& configFilePath);
		std::string getSymbol(const std::string& mkt, int date) const;
		QStringList  allMainSymbols (int date) const;
		QStringList  allContinuousSymbols ( ) const; //return AG888...
		QString getSymbolFromContinuous ( const QString& symbol /* AG888*/ , int date );
	private:
		typedef std::map<int/*begin date*/, std::string/*sym*/> DateSymMap;
		QMap<QString/*mkt*/, DateSymMap> _mktDateSymMap;

		std::string getSymbol ( const DateSymMap& dateSymMap , int date ) const;
	};
}