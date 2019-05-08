
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
#include "ExchOrderSet.h"
#include "mts_core/Order.h"

namespace mts {
	ExchOrderSet::ExchOrderSet()
	{
		_fillId = 1000;
		_exchId = 1000;
	}
	ExchOrderSet::~ExchOrderSet()
	{

	}

	QString ExchOrderSet::genExchId(const InstrumentId& id) const
	{
		return QString("sim_%1").arg(_exchId++);
	}

	QString ExchOrderSet::genFillId(const InstrumentId& id) const
	{
		return QString("fill_%1").arg(_fillId++);
	}
}