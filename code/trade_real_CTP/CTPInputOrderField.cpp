
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
#include "CTPInputOrderField.h"
#include "mts_core/Position.h"
#include "mts_core/const.h"
#include "mts_core/InstrumentId.h"

using namespace mts;

CTPInputOrderField::CTPInputOrderField(const QString& brokerId, const QString& investorId, const QString& userId)
	:CTPCommonField<CThostFtdcInputOrderField>(brokerId, investorId,userId)
{
	this->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; //必填
	this->VolumeCondition = THOST_FTDC_VC_AV; //成交量类型:任何数量
	this->MinVolume = 1;	//最小成交量:1

	this->ContingentCondition = THOST_FTDC_CC_Immediately;  //触发条件:立即
	this->StopPrice = 0;  //止损价
	this->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//强平原因:非强平
	this->IsAutoSuspend = 0;  //自动挂起标志:否
	this->UserForceClose = 0;   //用户强评标志:否

}

CTPInputOrderField::~CTPInputOrderField() {
}

void CTPInputOrderField::setLimitPrice(double price) {
	if (price == 0) {//market order
		this->OrderPriceType = THOST_FTDC_OPT_AnyPrice;//必填
		this->LimitPrice = 0; //必填
		this->TimeCondition = THOST_FTDC_TC_IOC;  //有效期类型:立即完成，否则撤销
	} else {//limit order
		this->OrderPriceType = THOST_FTDC_OPT_LimitPrice;//必填
		this->LimitPrice = price; //必填
		this->TimeCondition = THOST_FTDC_TC_GFD;  //有效期类型:当日有效
	}
}

void CTPInputOrderField::setDirection(mts::DirectionSide direction) {
	/*if (pos) {
	}*/
	if (direction == D_BUY || direction == D_SHORT) {
		if (direction == D_BUY) {
			this->Direction = THOST_FTDC_D_Buy; //必填
		} else {
			this->Direction = THOST_FTDC_D_Sell; //必填
		}
		this->CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	} else if (direction == D_SELL || direction == D_COVER) {
		if (direction == D_SELL) {
			this->Direction = THOST_FTDC_D_Sell; //必填
		} else {
			this->Direction = THOST_FTDC_D_Buy; //必填
		}
		this->CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	};

}

void CTPInputOrderField::setCombOffsetFlag(int offsetFlag)
{
	switch (offsetFlag)
	{
	case mts::OF_OPEN:
		this->CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		break;
	case mts::OF_CLOSE:
		this->CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		break;
	case mts::OF_FORCECLOSE:
		break;
	case mts::OF_CLOSETODAY:
		this->CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		break;
	case mts::OF_CLOSEYESTERDAY:
		this->CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		break;
	default:
		break;
	}
}

inline
QString directionDefineName ( TThostFtdcDirectionType  d ){
	if ( d == THOST_FTDC_D_Buy ){
		return "THOST_FTDC_D_Buy";
	}
	if ( d == THOST_FTDC_D_Sell ){
		return "THOST_FTDC_D_Sell";
	}
	return QString("UNKNOWN:%1").arg(d);
}

inline 
QString combOffsetDefineName ( char f ){
	if ( f == THOST_FTDC_OF_Open ){
		return "THOST_FTDC_OF_Open";
	}
	if ( f == THOST_FTDC_OF_Close ){
		return "THOST_FTDC_OF_Close";
	}
	if ( f == THOST_FTDC_OF_ForceClose ){
		return "THOST_FTDC_OF_ForceClose";
	}
	if ( f == THOST_FTDC_OF_CloseToday ){
		return "THOST_FTDC_OF_CloseToday";
	}
	if ( f == THOST_FTDC_OF_CloseYesterday ){
		return "THOST_FTDC_OF_CloseYesterday";
	}
	if ( f == THOST_FTDC_OF_ForceOff ){
		return "THOST_FTDC_OF_ForceOff";
	}
	if ( f == THOST_FTDC_OF_LocalForceClose ){
		return "THOST_FTDC_OF_LocalForceClose";
	}

	return QString ( "UNKNOWN:%1" ).arg ( f );
}

QJsonObject CTPInputOrderField::toJson() const {
	auto json = CTPCommonField::toJson();
	json.insert("Direction", directionDefineName(this->Direction));
	json.insert("CombOffsetFlag", combOffsetDefineName(this->CombOffsetFlag[0]));
	json.insert("LimitPrice", this->LimitPrice);
	json.insert("VolumeTotalOriginal", this->VolumeTotalOriginal);
	return json;
}


