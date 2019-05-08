
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
#include "PositionVolumeInfo.h"
#include "mts_core/TradingDateMgr.h"

namespace mts {

	PositionVolumeInfo::PositionVolumeInfo()
	{
		clear();
	}


	PositionVolumeInfo::~PositionVolumeInfo()
	{
	}

	void PositionVolumeInfo::clear() {
		memset(this, 0, sizeof(PositionVolumeInfo));
	}

	void PositionVolumeInfo::businessDateChanged(int businessDate) {
		if (!TradingDateMgr::instance()->is24Hour()) {
			double tmpCurrent = current; //TODO check double
			clear();
			open = tmpCurrent;
			current = tmpCurrent;
		}
	}

	QString PositionVolumeInfo::toString() const {
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

	void PositionVolumeInfo::toJson(QJsonObject & obj) const {
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

	void PositionVolumeInfo::fromJson(const QJsonObject & obj) {
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

}