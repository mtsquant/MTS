
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
#pragma once
#include "mts_core/mts_core_api.h"
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include "Position.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/InstrumentProperty.h"

namespace mts
{
	class MTS_CORE_API Account
	{
	public:
		Account();
		Account(const Account&);
		~Account();

		Account* clone() const;

		InstrumentBaseProperty* find ( const QString& symbol) const;

		static void registerMetaType();
	private:
		
	};

	typedef QSharedPointer<Account> AccountPtr;

}
Q_DECLARE_METATYPE(mts::AccountPtr);
