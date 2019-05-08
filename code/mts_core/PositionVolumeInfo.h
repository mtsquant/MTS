
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
#include <QtCore/QJsonObject>
#include "mts_core_api.h"

namespace mts {

	class MTS_CORE_API PositionVolumeInfo
	{
	public:
		PositionVolumeInfo();
		~PositionVolumeInfo();

		void clear();
		void businessDateChanged(int businessDate);
		QString toString() const;
		void toJson(QJsonObject& obj)const;
		void fromJson(const QJsonObject& obj);

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

	};

}