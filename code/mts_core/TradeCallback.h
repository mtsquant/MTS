
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
#include "mts_core/OrderReport.h"
#include "mts_core/Order.h"
#include "mts_core/Position.h"
#include "DateCallback.h"

namespace mts
{
    class TradeCallback:virtual public DateCallback
	{
	public:
        virtual ~TradeCallback(){}
		virtual void onOrderNewDone(OrderReportNewDone*) = 0;
		virtual void onOrderNewReject(OrderReportNewReject*) = 0;
		virtual void onOrderFill(OrderReportFill*) = 0;

		virtual void onOrderCancelDone(OrderReportCancelDone*) = 0;
		virtual void onOrderCancelReject(OrderReportCancelReject*) = 0;


		virtual void onOrderOtherReport(OrderReport*) = 0;

		virtual void onOrderUpdate(Order*) = 0;

		virtual void onPositionUpdate(Position*) = 0;
		virtual void onTradingAccountUpdate(TradingAccount*) {};
	};
}
