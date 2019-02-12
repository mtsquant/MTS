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

#ifdef _WIN32
#include "../../api/win64/CTP/ThostFtdcTraderApi.h"
#else
#include "../../api/linux64/CTP/ThostFtdcTraderApi.h"
#include <float.h>
#endif
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include "ctp_ext_api.h"

// ��װ CTP �� struct��
// ����Ҫ�õĵ��ֶΰ�װ�ɷ����������
// ���ԣ���Ҫ���κγ�Ա����
template<typename _CTPField>
class CTPCommonField:public _CTPField
{
public:
	CTPCommonField(const QString& brokerId,const QString& investorId, const QString& userId) {
		memset(dynamic_cast<_CTPField*>(this), 0, sizeof(_CTPField));
		setBrokerID(brokerId);
		setInvestorID(investorId);
		setUserID(userId);
	}

	CTPCommonField(const _CTPField& field)
		:_CTPField(field) {
	}

	~CTPCommonField() {
	}
	void setInstrumentID(const QString& id) {
		strncpy(this->InstrumentID, qPrintable(id), 31);
	}
	QString instrumentID() const {
		return this->InstrumentID;
	}

	void setOrderRef(const QString& id) {
		strncpy(this->OrderRef, qPrintable(id), 13);
	}
	QString orderRef() const {
		return this->OrderRef;
	}

	//QString toString() const {
	//	return QString("\"BrokerID\":\"%1\",\"InvestorID\":\"%2\",\"UserID\":\"%3\",\"InstrumentID\":\"%4\",\"OrderRef\":\"%5\"")
	//		.arg(this->brokerID())
	//		.arg(this->investorID())
	//		.arg(this->userID())
	//		.arg(this->instrumentID())
	//		.arg(this->orderRef())
	//		;
	//}

	virtual QJsonObject toJson() const {
		QJsonObject json;
		json.insert("BrokerId",QString(this->brokerID()));
		json.insert("InvestorID", QString(this->investorID()));
		json.insert("UserID", QString(this->userID()));
		json.insert("InstrumentID", QString(this->instrumentID()));
		json.insert("OrderRef", QString(this->orderRef()));
		return json;
	}

	QString toJsonString() const {
		return QJsonDocument(toJson()).toJson().simplified ();
	}

private:
	void setBrokerID(const QString& id) {
		strncpy(this->BrokerID, qPrintable(id), 11);
	}
	QString brokerID() const {
		return this->BrokerID;
	}

	void setInvestorID(const QString& id) {
		strncpy(this->InvestorID, qPrintable(id), 13);
	}
	QString investorID() const {
		return this->InvestorID;
	}

	void setUserID(const QString& id) {
		strncpy(this->UserID, qPrintable(id), 16);
	}
	QString userID() const {
		return this->UserID;
	}

};

