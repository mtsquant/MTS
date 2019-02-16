
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
#include "AccountMgr.h"

namespace mts {

	AccountMgr * mts::AccountMgr::instance()
	{
		static AccountMgr accountMgr;
		return &accountMgr;
	}

	void AccountMgr::setStrategy(StrategyInterface * notify)
	{
		_notify = notify;
	}

	AccountMgr::AccountMgr()
		:_notify(nullptr)
	{
	}


	AccountMgr::~AccountMgr()
	{
	}

	void AccountMgr::onEnvirInitialized() {
		_notify->onEnvirInitialized();
	}

	void AccountMgr::onInitialized(Account *)
	{
		Account account;
		_notify->onInitialized(&account);
	}


}