
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
#include "CrossThreadNotifier.h"
#include "mts_core/Environment.h"

namespace mts
{


	CrossThreadNotifier::CrossThreadNotifier(  const std::string& name, int strategyId , const std::string& strategyPath)
		:_name(name) ,_strategyId(strategyId),_strategyPath(strategyPath)
	{
	}


	CrossThreadNotifier::~CrossThreadNotifier() {
	}


	void CrossThreadNotifier::onEnvirInitialized() {
		assert(Environment::instance()->isCurrentMtsThread());

		Q_EMIT envirInitialized();
	}

	void CrossThreadNotifier::onInitialized(Account * account) {
		assert(Environment::instance()->isCurrentMtsThread());

		Q_EMIT initialized(AccountPtr(account->clone()));
	}

	void CrossThreadNotifier::onTimeout(int id) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT timeout(id);
	}

	void CrossThreadNotifier::onQuoteSnapshotUpdate(QuoteSnapshotPtr qt) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT quoteSnapshotUpdate(QuoteSnapshotPtr(qt->clone()));
	}

	void CrossThreadNotifier::onOrderNewDone(OrderReportNewDone * done) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderNewDone(OrderReportNewDonePtr(done->clone()));
	}

	void CrossThreadNotifier::onOrderNewReject(OrderReportNewReject * reject) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderNewReject(OrderReportNewRejectPtr(reject->clone()));
	}

	void CrossThreadNotifier::onOrderFill(OrderReportFill * fill) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderFill(OrderReportFillPtr(fill->clone()));
	}

	void CrossThreadNotifier::onOrderCancelDone(OrderReportCancelDone * done) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderCancelDone(OrderReportCancelDonePtr(done->clone()));
	}

	void CrossThreadNotifier::onOrderCancelReject(OrderReportCancelReject * reject) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderCancelReject(OrderReportCancelRejectPtr(reject->clone()));
	}

	void CrossThreadNotifier::onOrderOtherReport(OrderReport * report) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderOtherReport(OrderReportPtr(report->clone()));
	}

	void CrossThreadNotifier::onPositionUpdate(Position * pos) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT positionUpdate(PositionPtr(pos->clone()));
	}

	void CrossThreadNotifier::onOrderUpdate(Order * order) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT orderUpdate(OrderPtr(order->clone()));
	}

	void CrossThreadNotifier::onBusinessDateChanged(int dt) {
		//assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT businessDateChanged(dt);
	}

	void CrossThreadNotifier::onBarUpdate(CalcBarPtr bar) {
		assert(Environment::instance()->isCurrentMtsThread());
		Q_EMIT barUpdate(bar);
	}

	Qt::ConnectionType CrossThreadNotifier::connectionType() const {
		//return Environment::instance()->mode()->threadConnectionType();
		//in real mode , all callback will be call by queued
		//in simu mode , all callback will be call by block queued for reduce 2 event loops to one event loop (mts thread event loop)
		// THINKING: does real mode use block queued mode also? if use , the nodejs plugin can not support
		//		for c++, callback can be called all of mode (directly, block queued, queued)
		//      for python , callback can be called both of mode (block queued , queued)
		//      for nodejs, callback ONLY can be called of mode (queued)
		//		NOTE: the queued mode does not support simu because we can not monitor if the 2 event loops empty
		return (mts::Environment::instance()->mode() == ENVIR_SIMU ?
			Qt::ConnectionType(Qt::DirectConnection | Qt::UniqueConnection) :
			Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection)
			);
		//return Qt::QueuedConnection;
	}

}