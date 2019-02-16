
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
#include "mts_core_api.h"

#define OT_UNKNOWN 0
#define OT_DIRECT 1
#define OT_NET 2
#define OT_BOX 3



#define OT_LAST  OT_BOX


namespace mts
{
	bool MTS_CORE_API isInstanceIdValid(int instanceId);
	bool MTS_CORE_API isStrategyIdValid(int strategyId);
}