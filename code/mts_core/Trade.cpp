
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
#include "Trade.h"

namespace mts
{

	TradeBase::TradeBase()
		:Component()
	{
	}

	TradeBase::~TradeBase()
	{
	}


	Trade::Trade()
		:TradeBase()
	{
	}


	Trade::~Trade() {
	}

	int Trade::addCallback(TradeCallback * cb) {
		_callbacks.append(cb);
		return _callbacks.size();
	}


	void Trade::notifyAll(mts::OrderReportNewDone * rpt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderNewDone(rpt);
		}
	}

	void Trade::notifyAll(mts::OrderReportNewReject * rpt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderNewReject(rpt);
		}
	}

	void Trade::notifyAll(mts::OrderReportFill * rpt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderFill(rpt);
		}
	}

	void Trade::notifyAll(mts::OrderReportCancelDone * rpt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderCancelDone(rpt);
		}
	}

	void Trade::notifyAll(mts::OrderReportCancelReject * rpt) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderCancelReject(rpt);
		}
	}

	void Trade::notifyAll(mts::Order * order) {
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onOrderUpdate(order);
		}
	}

	void Trade::notifyAll(mts::Position* pos)
	{
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onPositionUpdate(pos);
		}
	}

	void Trade::notifyAll(mts::TradingAccount* ta)
	{
		for (int i = 0, size = _callbacks.size(); i < size; ++i) {
			TradeCallback* callback = _callbacks[i];
			callback->onTradingAccountUpdate(ta);
		}
	}


}
