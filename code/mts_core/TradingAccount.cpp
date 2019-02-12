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
#include "TradingAccount.h"
#include <QJsonDocument>

namespace mts
{

	TradingAccount::TradingAccount()
	{
		_commission = 0;
		_available = 0;
		_positionProfit = 0;
		_closeProfit = 0;
		_currMargin = 0;
		_preBalance = 0;
		_deposit = 0;
		_withdraw = 0;
	}

	TradingAccount::~TradingAccount()
	{

	}

	const TradingAccountId& TradingAccount::accountId() const
	{
		return _id;
	}

	const QString& TradingAccount::password() const
	{
		return _password;
	}

	double TradingAccount::commission() const
	{
		return _commission;
	}

	double TradingAccount::available() const
	{
		return _available;
	}

	double TradingAccount::positionProfit() const
	{
		return _positionProfit;
	}

	double TradingAccount::closeProfit() const
	{
		return _closeProfit;
	}

	double TradingAccount::currMargin() const
	{
		return _currMargin;
	}

	double TradingAccount::preBalance() const
	{
		return _preBalance;
	}

	double TradingAccount::deposit() const
	{
		return _deposit;
	}

	double TradingAccount::withdraw() const
	{
		return _withdraw;
	}

	double TradingAccount::dynamicInterests() const
	{
		return preBalance() + deposit() - withdraw() - commission() + positionProfit();
	}

	void TradingAccount::setAccountId(const TradingAccountId& id)
	{
		_id = id;
	}

	void TradingAccount::setPassword(const QString& pw)
	{
		_password = pw;
	}

	void TradingAccount::setCommission(double commission)
	{
		_commission = commission;
	}

	void TradingAccount::setAvailable(double margin)
	{
		_available = margin;
	}

	void TradingAccount::setPositionProfit(double profit)
	{
		_positionProfit = profit;
	}

	void TradingAccount::setCloseProfit(double profit)
	{
		_closeProfit = profit;
	}

	void TradingAccount::setCurrMargin(double v)
	{
		_currMargin = v;
	}

	void TradingAccount::setPreBalance(double v)
	{
		_preBalance = v;
	}

	void TradingAccount::setDeposit(double v)
	{
		_deposit = v;
	}

	void TradingAccount::setWithdraw(double v)
	{
		_withdraw = v;
	}

	QString TradingAccount::toJsonString() const
	{
		return QJsonDocument(toJson()).toJson().simplified();
	}

	QJsonObject TradingAccount::toJson() const
	{
		QJsonObject obj;
		obj.insert("id", _id.key());
		obj.insert("positionProfit", _positionProfit);
		obj.insert("available", _available);
		obj.insert("closeProfit", _closeProfit);
		obj.insert("currMargin", _currMargin);
		obj.insert("commission", _commission);		
		obj.insert("dynamicInterests", dynamicInterests());
		return obj;
	}

}
