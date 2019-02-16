
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
#include <QtCore/QVariant>
#include <QtCore/QMap>
#include "TimerCallback.h"
#include "TradeCallback.h"
#include "AccountCallback.h"
#include "mts_core/Position.h"
#include "FeedsCallback.h"


namespace mts
{
	//各个脚本包装库实现此接口,此接口将被在mts 线程中调用，
	//由包装库负责通知到主线程，然后在主线程中回调脚本
	//由于不同脚本环境通知机制不同，所以把线程通知机制放入到各个脚本包装库中
	class StrategyInterface:public TimerCallback, public AccountCallback, public FeedsSnapshotCallback, public BarFeedsCallback, public TradeCallback
	{
	public:
		virtual std::string name() const = 0;
		virtual std::string strategyPath() const { return std::string(); };
		virtual int strategyId () const = 0; /*一个进程内多个Strategy时用来区分不同Strategy （1-9），最多9个Strategies 在一个进程中*/
	};

}