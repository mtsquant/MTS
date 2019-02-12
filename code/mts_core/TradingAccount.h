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
#include<QtGlobal>
#include <QString>
#include "mts_core/MethodDefine.h"
#include "mts_core/mts_core_api.h"
#include "base/Currency.h"
#include <QtCore/QSharedPointer>

namespace mts 
{
	struct MTS_CORE_API TradingAccountId
	{
		QString brokerId;
		QString username;
		CurrencyId currency;
		QString key() const {
			return brokerId + "@" + username+"@"+ currencyIdName(currency);
		}
	};

	class MTS_CORE_API TradingAccount
	{
	public:
		TradingAccount();
		~TradingAccount();

		const TradingAccountId& accountId()const;
		const QString& password()const;
		double commission()const;//������
		double available()const; //�����ʽ�
		double positionProfit()const;//�ֲ�ӯ��
		double closeProfit()const;//ƽ��ӯ��
		double currMargin()const;//��ǰ��֤���ܶ�
		double preBalance()const;//���ս��
		double deposit() const;///�����
		double withdraw() const;///������

		double dynamicInterests()const;

		void setAccountId(const TradingAccountId&);
		void setPassword(const QString&);
		void setCommission(double);
		void setAvailable(double);
		void setPositionProfit(double);
		void setCloseProfit(double);
		void setCurrMargin(double);
		void setPreBalance(double);
		void setDeposit(double);
		void setWithdraw(double);

		QString toJsonString() const;
		QJsonObject toJson() const;
	private:
		TradingAccountId _id;
		QString _password;
		double _commission;//������
		double _available;//�����ʽ�
		double _positionProfit;//�ֲ�ӯ��
		double _closeProfit;//ƽ��ӯ��
		double _currMargin;//��֤��	
		double _preBalance;//���ս��
		double	_deposit;//�����
		double	_withdraw;//������
	};
	typedef QSharedPointer<TradingAccount> TradingAccountPtr;
}
Q_DECLARE_METATYPE(mts::TradingAccountPtr);
