
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
#include "PythonStrategyWrap.h"
#include "PythonStrategy.h"

typedef  PythonStrategyWrapTemplate<PythonStrategy> PythonStrategyWrap;


void initializePythonModule();

#ifndef NDEBUG
BOOST_PYTHON_MODULE(mts_agent_python_d)
#else
BOOST_PYTHON_MODULE(mts_agent_python)
#endif
{
	initializePythonModule();

	class_<PythonStrategyWrap, boost::noncopyable>("StrategyBase", init<std::string, int>())
		//.def ( "initialize" , &PythonStrategyWrap::initialize )
		//.def ( "execute", &PythonStrategyWrap::exec)
		//.def ( "exit", &PythonStrategyWrap::exit)
		.def("newTimer", &PythonStrategyWrap::newTimer)
		.def("newOnceTimer", &PythonStrategyWrap::newOnceTimer)
		.def("removeTimer", &PythonStrategyWrap::removeTimer)
		.def("newOrder", &PythonStrategyWrap::newOrder)
		.def("cancelOrder", &PythonStrategyWrap::cancelOrder)
		.def("subscribeQuotes", &PythonStrategyWrap::subscribeQuotes)
		.def("subscribe", &PythonStrategyWrap::subscribeQuotes)
		.def("unsubscribeQuotes", &PythonStrategyWrap::unsubscribeQuotes)
		.def("unsubscribe", &PythonStrategyWrap::unsubscribeQuotes)
		.def("unsubscribeAllQuotes", &PythonStrategyWrap::unsubscribeAllQuotes)
		.def("unsubscribeAll", &PythonStrategyWrap::unsubscribeAllQuotes)

		.def("getPosition", &PythonStrategyWrap::getPosition)
		.def("getActiveOrders", &PythonStrategyWrap::getActiveOrders)
		.def("getQuote", &PythonStrategyWrap::getQuote)
		.def("getAllTradingCounts", &PythonStrategyWrap::getAllTradingCounts)

		//.def ( "loadBars" , &PythonStrategyWrap::loadBars )
		.add_property("allOrders", &PythonStrategyWrap::getAllOrders)
		.def("getAllOrders", &PythonStrategyWrap::getAllOrders)
		.add_property("allPositions", &PythonStrategyWrap::getAllPositions)
		.def("getAllPositions", &PythonStrategyWrap::getAllPositions)
		.add_property("now", &PythonStrategyWrap::getNow)
		.add_property("nowMicrosecs", &PythonStrategyWrap::getNowMicrosecs)
		.def("getNow", &PythonStrategyWrap::getNow)

		.def("onEnvirInitialized", &PythonStrategyWrap::onEnvirInitialized)
		.def("onInitialized", &PythonStrategyWrap::onInitialized)
		.def("onTimer", &PythonStrategyWrap::onTimer)
		.def("onQuoteSnapshotUpdate", &PythonStrategyWrap::onQuoteSnapshotUpdate)
		.def("onOrderNewDone", &PythonStrategyWrap::onOrderNewDone)
		.def("onOrderNewReject", &PythonStrategyWrap::onOrderNewReject)
		.def("onOrderFill", &PythonStrategyWrap::onOrderFill)
		.def("onOrderCancelDone", &PythonStrategyWrap::onOrderCancelDone)
		.def("onOrderCancelReject", &PythonStrategyWrap::onOrderCancelReject)
		.def("onOrderOtherReport", &PythonStrategyWrap::onOrderOtherReport)
		.def("onPositionUpdate", &PythonStrategyWrap::onPositionUpdate)
		.def("onOrderUpdate", &PythonStrategyWrap::onOrderUpdate)
		.def("onBarUpdate", &PythonStrategyWrap::onBarUpdate)
		;
}
