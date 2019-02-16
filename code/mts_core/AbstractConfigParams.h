
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
#include <QtCore/QVariantMap>
#include "mts_core/mts_core_api.h"
#include "mts_core/enums.h"
#include "base/MtsLog.h"
#include "ParamsFields.h"

namespace mts
{
	class MTS_CORE_API AbstractConfigParams 
	{
	protected:
		AbstractConfigParams();
		virtual ~AbstractConfigParams();
	public:
		bool load(const QVariantMap & params) ;
		EnvironmentMode mode() const;
		int instanceId() const;
		int strategyPosCheck() const;

		QVariant  value(const QString& key, const QVariant& defaultValue=QVariant()) const;
		QStringList keys() const;
	protected:
		virtual bool doLoad(const QVariantMap & params);
		bool isSimu() const;
	private:
		EnvironmentMode _mode;
		int _instanceId;
		int _strategyPosCheck;

		QVariantMap _allValues;
	};


}