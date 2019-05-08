
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
#include "base/Event.h"
#include "base/EventType.h"
#include "mts_core/Trade.h"



typedef DataEvent<mts::OrderReportNewDone*, EVT_RPT_ORDER_NEW_DONE> OrderNewDoneEvent;

typedef DataEvent<mts::OrderReportNewReject*, EVT_RPT_ORDER_NEW_REJECT> OrderNewRejectEvent;
typedef DataEvent<mts::OrderReportFill*, EVT_RPT_ORDER_FILL> OrderFillEvent;
typedef DataEvent<mts::OrderReportCancelDone*, EVT_RPT_ORDER_CANCEL_DONE> OrderCancelDoneEvent;
typedef DataEvent<mts::OrderReportCancelReject*, EVT_RPT_ORDER_CANCEL_REJECT> OrderCancelRejectEvent;
typedef DataEvent<mts::Order*, EVT_ORDER> OrderEvent;
typedef DataEvent<mts::Position*, EVT_POSITION> PositionEvent;

