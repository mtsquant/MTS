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
#include "CTPInputOrderField.h"
//#include "../mts/PositionMgr.h"
#include "mts_core/Position.h"
#include "mts_core/const.h"
#include "mts_core/InstrumentId.h"

using namespace mts;

CTPInputOrderField::CTPInputOrderField(const QString& brokerId, const QString& investorId, const QString& userId)
	:CTPCommonField<CThostFtdcInputOrderField>(brokerId, investorId,userId)
{
	//�ױ���־
	this->CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; //����
	this->VolumeCondition = THOST_FTDC_VC_AV; //�ɽ�������:�κ�����
	this->MinVolume = 1;	//��С�ɽ���:1

	this->ContingentCondition = THOST_FTDC_CC_Immediately;  //��������:����
	this->StopPrice = 0;  //ֹ���
	this->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;	//ǿƽԭ��:��ǿƽ
	this->IsAutoSuspend = 0;  //�Զ������־:��
	//req.BusinessUnit;
	//this->RequestID = this->nextRequestId();
	this->UserForceClose = 0;   //�û�ǿ����־:��
   //req.IsSwapOrder;

}

CTPInputOrderField::~CTPInputOrderField() {
}

void CTPInputOrderField::setLimitPrice(double price) {
	if (price == 0) {//market order
		this->OrderPriceType = THOST_FTDC_OPT_AnyPrice;//����
		this->LimitPrice = 0; //����
		this->TimeCondition = THOST_FTDC_TC_IOC;  //��Ч������:������ɣ�������
	} else {//limit order
		this->OrderPriceType = THOST_FTDC_OPT_LimitPrice;//����
		this->LimitPrice = price; //����
		this->TimeCondition = THOST_FTDC_TC_GFD;  //��Ч������:������Ч
	}
}

void CTPInputOrderField::setDirection(mts::DirectionSide direction) {
	//��ƽ����
	//������������ֺͽ�֡�
	//ƽ���ʱ����ƽ��־��������Ϊƽ�� THOST_FTDC_OF_Close
	//ƽ���ʱ����ƽ��־��������Ϊƽ��� THOST_FTDC_OF_CloseToday
	//������������������ֺͽ�֡�
	//��ƽ��־����ͳһ����Ϊƽ�� THOST_FTDC_OF_Close
	//InstrumentId iid(this->InstrumentID, TYPE_FUTR);
	//InstrumentId iid;
	//iid.symbol = this->InstrumentID;
	//Position* pos = PositionMgr::instance()->getPosition(iid, GM_GET_OR_CREATE);
	//bool isSHFE = (pos&&pos->attribute()->id().exchId== EXCH_SHFE);
	/*if (pos) {
		///this->ExchangeID[0] = pos->attribute().ExchangeID[0];
	}*/
	if (direction == D_BUY || direction == D_SHORT) {
		if (direction == D_BUY) {
			this->Direction = THOST_FTDC_D_Buy; //����
		} else {
			this->Direction = THOST_FTDC_D_Sell; //����
		}
		this->CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	} else if (direction == D_SELL || direction == D_COVER) {
		if (direction == D_SELL) {
			this->Direction = THOST_FTDC_D_Sell; //����
		} else {
			this->Direction = THOST_FTDC_D_Buy; //����
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
		//this->CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
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

//QString CTPInputOrderField::toString() const {
//	return QString("%1,\"Direction\":\"%2 %3\",\"LimitPrice\":%4,\"VolumeTotalOriginal\":%5")
//		.arg(CTPCommonField::toString())
//		.arg(this->Direction == THOST_FTDC_D_Buy?"BUY":"SELL")
//		.arg(this->CombOffsetFlag[0]== THOST_FTDC_OF_Open?"OPEN":"CLOSE")
//		.arg(this->LimitPrice)
//		.arg(this->VolumeTotalOriginal)
//		;
//}

