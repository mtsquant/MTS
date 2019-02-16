
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

#include "ctp_ext/CTPCommonField.h"
#include "mts_core/const.h"
#include "mts_core/enums.h"

// 包装 CTP 的 struct，把下单需要用的的字段包装成方法调用
class CTPInputOrderField :public CTPCommonField<CThostFtdcInputOrderField>
{
public:
	CTPInputOrderField(const QString& brokerId, const QString& investorId, const QString& userId);
	~CTPInputOrderField();

	void setLimitPrice(double price); //price=0 means market order
	void setDirection(mts::DirectionSide);
	void setCombOffsetFlag(int offsetFlag);
	virtual QJsonObject toJson() const;
};

