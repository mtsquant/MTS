
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
#include "CTPCancelOrderField.h"



CTPCancelOrderField::CTPCancelOrderField(const QString& brokerId, const QString& investorId, const QString& userId)
	:CTPCommonField<CThostFtdcInputOrderActionField>(brokerId, investorId, userId)
{
	this->ActionFlag = THOST_FTDC_AF_Delete;

}


CTPCancelOrderField::~CTPCancelOrderField() {
}

void CTPCancelOrderField::initFromOrder(const CThostFtdcOrderField & order) {
	this->setInstrumentID(order.InstrumentID); //必填
	this->setThostLevelId(order.FrontID, order.SessionID, order.OrderRef);
	if (strlen(order.OrderSysID) > 0) {
		this->setExchangeLevelId(order.ExchangeID, order.OrderSysID);
	}
}




void CTPCancelOrderField::setExchangeLevelId(const QString & exchangeId, const QString & orderSysId) {
	strcpy(this->ExchangeID, qPrintable(exchangeId));
	strcpy(this->OrderSysID, qPrintable(orderSysId));

}

void CTPCancelOrderField::setThostLevelId(int frontId, int sessionId, const QString & orderRef) {
	this->FrontID = frontId; // 指向委托单的FrontID;
	this->SessionID = sessionId; // 指向委托单的SessionID;
	this->setOrderRef(orderRef); //必填  
}
