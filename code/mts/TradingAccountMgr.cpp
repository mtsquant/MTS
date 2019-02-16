
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
#include "TradingAccountMgr.h"

namespace mts
{
	TradingAccountMgr* TradingAccountMgr::_instance = nullptr;
	TradingAccountMgr::TradingAccountMgr()
	{
	}


	TradingAccountMgr::~TradingAccountMgr()
	{
		qDeleteAll(_accounts);
	}

	TradingAccountMgr* TradingAccountMgr::instance()
	{
		if (_instance == nullptr) {
			_instance = new TradingAccountMgr;
		}
		return _instance;
	}

	QList<TradingAccount*> TradingAccountMgr::allTradingAccount() const
	{
		return _accounts.values();
	}

	TradingAccount* TradingAccountMgr::getTradingAccount(const TradingAccountId& id) const
	{
		auto it = _accounts.constFind(id.key());
		if (it != _accounts.cend()) {
			return it.value();
		}
		return nullptr;
	}

	void TradingAccountMgr::updateTradingAccount(TradingAccount* ta)
	{
		TradingAccount* last = _accounts[ta->accountId().key()];
		if (last != nullptr) {
			*last=*ta;
		}
		else {
			_accounts[ta->accountId().key()] = new TradingAccount(*ta);
		}
	}

	TradingAccount * TradingAccountMgr::getTradingAccount(CurrencyId currency) const {
		for (auto it = _accounts.constBegin(), itEnd = _accounts.constEnd(); it != itEnd; ++it) {
			TradingAccount* account = it.value();
			if (account->accountId().currency == currency) {
				return account;
			}
		}

		return nullptr;
	}

}
