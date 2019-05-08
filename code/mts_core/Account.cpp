
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
#include "Account.h"
#include "mts_core/InstrumentPropertyDb.h"

namespace mts
{

	Account::Account() {
	}

	Account::Account(const Account & a) 
	{
	}


	Account::~Account() {
	}

	Account * Account::clone() const{
		return new Account(*this);
	}

	InstrumentBaseProperty * Account::find ( const QString& symbol ) const{
		auto id = InstrumentPropertyDb::instance ()->findInstrumentId ( symbol );
		if ( id.isNull () ){
			return nullptr;
		}
		return InstrumentPropertyDb::instance ()->find ( id );
	}

	void Account::registerMetaType() {
		qRegisterMetaType<Account>("Account");
		qRegisterMetaType<AccountPtr>("AccountPtr");
	}

}
