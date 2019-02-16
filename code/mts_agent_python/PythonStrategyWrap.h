
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
#include "PythonStrategy.h"
#include <boost/python.hpp>
#include "base/MtsLog.h"

using namespace boost::python;

///=====================================================================================
///Boost.Python封装 for callback functions
///=====================================================================================
template <class StrategyClass>
struct PythonStrategyWrapTemplate : StrategyClass, wrapper < StrategyClass >
{
	PythonStrategyWrapTemplate(const std::string& name, int strategyId, const std::string& strategyPath = std::string())
		:StrategyClass(name, strategyId, strategyPath) {
	}

	virtual void onEnvirInitialized() override {
		try {
			auto ov = this->get_override("onEnvirInitialized");
			PyObject* cb = ov.ptr();
			if (!ov.is_none()) {
				call<void>(cb);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onEnvirInitialized() error\n");
			PyErr_Print();
		}

	}

	virtual void onInitialized ( const PyAccount& account ) override {
		try {
			auto ov = this->get_override("onInitialized");
			PyObject* cb = ov.ptr();
			if (!ov.is_none()) {
				call<void>(cb, account);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onInitialized() error\n");
			PyErr_Print();
		}
	}

	virtual void onTimer ( int timerId ) override {
		try {
			auto ov = this->get_override("onTimer");
			PyObject* cb = ov.ptr();
			if (!ov.is_none()) {
				call<void>(cb, timerId);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onTimer() error\n");
			PyErr_Print();
		}
	}

	virtual void onQuoteSnapshotUpdate (const PyQuote& quote) override {
		try {
			auto ov = this->get_override("onQuoteSnapshotUpdate");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), quote);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onQuoteSnapshotUpdate() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderNewDone ( const PyOrderNewDone&orderNewDone) override {
		try {
			auto ov = this->get_override("onOrderNewDone");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderNewDone);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderNewDone() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderNewReject ( const PyOrderNewReject& orderNewReject ) override {
		try {
			auto ov = this->get_override("onOrderNewReject");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderNewReject);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderNewReject() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderFill ( const PyOrderFill& orderFill) override {
		try {
			auto ov = this->get_override("onOrderFill");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderFill);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderFill() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderCancelDone ( const PyOrderCancelDone& orderCancelDone) override {
		try {
			auto ov = this->get_override("onOrderCancelDone");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderCancelDone);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderCancelDone() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderCancelReject ( const PyOrderCancelReject& orderCancelReject) override {
		try {
			auto ov = this->get_override("onOrderCancelReject");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderCancelReject);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderCancelReject() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderOtherReport ( const PyOrderReport& orderReport) override {
		try {
			auto ov = this->get_override("onOrderOtherReport");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), orderReport);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderOtherReport() error\n");
			PyErr_Print();
		}
	}

	virtual void onPositionUpdate ( const PyPosition& pos) override {
		try {
			auto ov = this->get_override("onPositionUpdate");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), pos);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onPositionUpdate() error\n");
			PyErr_Print();
		}
	}

	virtual void onOrderUpdate (const PyOrder& order) override {
		try {
			auto ov = this->get_override("onOrderUpdate");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), order);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onOrderUpdate() error\n");
			PyErr_Print();
		}
	}

	virtual void onBusinessDateChanged ( int date ) override {
		try {
			auto ov = this->get_override("onBusinessDateChanged");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), date);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onBusinessDateChanged() error\n");
			PyErr_Print();
		}
	}

	virtual void onBarUpdate ( const PyBar& bar) override {
		try {
			auto ov = this->get_override("onBarUpdate");
			if (!ov.is_none()) {
				call<void>(ov.ptr(), bar);
			}
		} catch (error_already_set const &) {
			MTS_ERROR("call python onBarUpdate() error\n");
			PyErr_Print();
		}
	}

};