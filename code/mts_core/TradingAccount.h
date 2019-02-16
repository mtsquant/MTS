
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
		double commission()const;//手续费
		double available()const; //可用资金
		double positionProfit()const;//持仓盈亏
		double closeProfit()const;//平仓盈亏
		double currMargin()const;//当前保证金总额
		double preBalance()const;//上日结存
		double deposit() const;///入金金额
		double withdraw() const;///出金金额

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
		double _commission;//手续费
		double _available;//可用资金
		double _positionProfit;//持仓盈亏
		double _closeProfit;//平仓盈亏
		double _currMargin;//保证金	
		double _preBalance;//上日结存
		double	_deposit;//入金金额
		double	_withdraw;//出金金额
	};
	typedef QSharedPointer<TradingAccount> TradingAccountPtr;
}
Q_DECLARE_METATYPE(mts::TradingAccountPtr);
