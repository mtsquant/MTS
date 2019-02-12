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
#include "CTPCancelOrderField.h"



CTPCancelOrderField::CTPCancelOrderField(const QString& brokerId, const QString& investorId, const QString& userId)
	:CTPCommonField<CThostFtdcInputOrderActionField>(brokerId, investorId, userId)
{
	this->ActionFlag = THOST_FTDC_AF_Delete;
	//req.OrderActionRef = this->nextRequestId();

}


CTPCancelOrderField::~CTPCancelOrderField() {
}

void CTPCancelOrderField::initFromOrder(const CThostFtdcOrderField & order) {
	this->setInstrumentID(order.InstrumentID); //����
	this->setThostLevelId(order.FrontID, order.SessionID, order.OrderRef);
	if (strlen(order.OrderSysID) > 0) {
		this->setExchangeLevelId(order.ExchangeID, order.OrderSysID);
	}
}


/// �������к�
//FrontID ��SessionID�� OrderRef��
//ExchangID�� OrderSysID��
/// ��������
//BrokerID��
//UserID,
//InvestorID��
//InstrumentID��
//���� 5 �����׹ؼ���, �ڳ���ʱ��Ҫʹ�á�
//���������ͣ���� Thost��Thost ������ Front ��SessionID��OrderRef ����λ
//���������û���������� Thost ExchangeID + TraderID + OrderLocalID ����λ
//�������ͣ���ڽ�������Thost ������ ExchangID��OrderSysID ����λ��Ȼ��������ת������ָ�


void CTPCancelOrderField::setExchangeLevelId(const QString & exchangeId, const QString & orderSysId) {
	strcpy(this->ExchangeID, qPrintable(exchangeId));
	strcpy(this->OrderSysID, qPrintable(orderSysId));

}

void CTPCancelOrderField::setThostLevelId(int frontId, int sessionId, const QString & orderRef) {
	this->FrontID = frontId; // ָ��ί�е���FrontID;
	this->SessionID = sessionId; // ָ��ί�е���SessionID;
	this->setOrderRef(orderRef); //����  
}
