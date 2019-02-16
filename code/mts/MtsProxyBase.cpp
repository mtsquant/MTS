
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
#include "MtsProxyBase.h"
#include "mts_core/Environment.h"
#include <QtCore/QMetaObject>
#include <QtCore/QCoreApplication>
#include "MtsMainThread.h"

#include "ClockMgr.h"
#include "QuoteMgr.h"
#include "PositionMgr.h"
#include "OrderMgr.h"
#include "AccountMgr.h"
#include "TradingAccountMgr.h"
#include "OrderFillMgr.h"
#include "mts_core/InstrumentPropertyDb.h"


#include "mts_core/ConfigParams.h"
#include "base/MtsLog.h"

#include <boost/bind.hpp>


namespace mts
{
	MtsProxyBase::MtsProxyBase()
		:_callback(nullptr), _mode(ENVIR_UNKNOWN)
	{
		Account::registerMetaType();
		Quote::registerMetaType();
		QuoteSnapshot::registerMetaType();
		Bar::registerMetaType();
		CalcBar::registerMetaType();
		Position::registerMetaType();
		OrderReportNewDone::registerMetaType();
		OrderReportNewReject::registerMetaType();
		OrderReportFill::registerMetaType();
		OrderReportCancelDone::registerMetaType();
		OrderReportCancelReject::registerMetaType();
		OrderReport::registerMetaType();
		Order::registerMetaType();

		qRegisterMetaType<TradingAccountPtr>("TradingAccountPtr");
	}

	MtsProxyBase::~MtsProxyBase()
	{
	}

	Qt::ConnectionType MtsProxyBase::connectionType() const {
		if (_mode == ENVIR_SIMU) {
			return Qt::DirectConnection;
		}
		else {
			if (Environment::instance()->isCurrentMtsThread()) {
				return Qt::DirectConnection;
			}
			else {
				return Qt::BlockingQueuedConnection;
			}
		}
	}


	//main thread event loop
	int MtsProxyBase::exec() {
		return QCoreApplication::instance()->exec();  //environment has not be initialized yet
	}

	void MtsProxyBase::exit(int code) {
			QCoreApplication::instance()->exit(code);
	}

	bool MtsProxyBase::createMtsMgrs(const QVariantMap & params) {
		return MtsMainThread::instance(_mode)->init(params,
			[]() {
			//The mgrs should be initialized in the mts thread 
			ClockMgr::instance();
			QuoteMgr::instance();
			OrderMgr::instance();
			OrderFillMgr::instance();
			TradingAccountMgr::instance();
			InstrumentPropertyDb::instance();
			PositionMgr::instance();
			////OrderAssistant::instance(); //TODO:it should be load dynamicly
		});
	}


	bool MtsProxyBase::loadMtsComponents(const QString& libraryPath) {
		bool ret = false;
		QMetaObject::invokeMethod(Environment::instance(), "load", this->connectionType(),
			Q_RETURN_ARG(bool, ret),
			Q_ARG(EnvironmentMode, _mode),
			Q_ARG(QString, libraryPath)
		);
		return ret;
	}


	bool MtsProxyBase::initMtsMgrs() {
		QuoteMgr::instance()->setStrategy(_callback);
		OrderMgr::instance()->setStrategy(_callback);
		AccountMgr::instance()->setStrategy(_callback);
		////OrderAssistant::instance()->init(
		////	boost::bind(&OrderMgr::sendOrder, OrderMgr::instance(), _1, _2),
		////	boost::bind(&PositionMgr::getPosition, PositionMgr::instance(), _1, GM_GET_OR_CREATE)
		////);
		return true;
	}



	bool MtsProxyBase::startRunMtsEnvir(const QVariantMap & params) {
		bool ret = false;
		return 		QMetaObject::invokeMethod(Environment::instance(), "initialize", this->connectionType(),
			Q_RETURN_ARG(bool, ret),
			Q_ARG(FeedsCallback*, QuoteMgr::instance()),
			Q_ARG(TradeCallback*, OrderMgr::instance()),
			Q_ARG(AccountCallback*, AccountMgr::instance()),
			Q_ARG(QVariantMap, params)
		);
	}

	bool MtsProxyBase::initialize(StrategyInterface* callbacks, const QVariantMap & params) {
		if (_callback) {//avoid to call the function multi times
			return true;
		}

		assert(!ConfigParams::isInitialized());
		if (!ConfigParams::instance()->load(params)) {
			MTS_ERROR("Failed to initialize MTS, check the params\n");
			return false;
		}

		_mode = ConfigParams::instance()->mode();

		if (!createMtsMgrs(params)) { // start the mts thread on real mode 
			return false;
		}


		QString libraryPath = ConfigParams::instance()->mtsDir();
		assert(!libraryPath.isEmpty());
		if (!loadMtsComponents(libraryPath)) {
			MtsMainThread::instance(_mode)->exitEventLoop();
			return false;
		}

		////if (!loadAlgoSugs(libraryPath)) {//load algo dll
		////	MtsMainThread::instance(_mode)->exitEventLoop();
		////	return false;
		////}

		_callback = callbacks;// the following initialize functions will be use the _callback
		if (!initMtsMgrs()) {
			MtsMainThread::instance(_mode)->exitEventLoop();
			return false;
		}

		////if (!loadIndicators()) {
		////	MtsMainThread::instance(_mode)->exitEventLoop();
		////	return false;
		////}

		if (!startRunMtsEnvir(params)) {
			MtsMainThread::instance(_mode)->exitEventLoop();
			return false;
		}

		_params = params;
		return true;
	}

	QVariantMap MtsProxyBase::params() const {
		return _params;
	}

	int MtsProxyBase::newTimer(int beginTime, int interval) {
		int timerId = 0;
		QMetaObject::invokeMethod(ClockMgr::instance(), "createTimer", this->connectionType(),
			Q_RETURN_ARG(int, timerId),
			Q_ARG(int, beginTime),
			Q_ARG(int, interval),
			Q_ARG(TimerCallback*, _callback)
		);
		return timerId;
	}

	bool MtsProxyBase::removeTimer(int id) {
		bool ret = false;
		QMetaObject::invokeMethod(ClockMgr::instance(), "destoryTimer", this->connectionType(),
			Q_RETURN_ARG(bool, ret),
			Q_ARG(int, id)
		);
		return ret;
	}

	mts::OrderId MtsProxyBase::newOrder(OrderActionNew* orderActionNew, int orderType) {
		mts::OrderId ret;
		QMetaObject::invokeMethod(OrderMgr::instance(), "sendOrder", this->connectionType(),
		////QMetaObject::invokeMethod(OrderAssistant::instance(), "sendOrder", this->connectionType(),
			Q_RETURN_ARG(mts::OrderId, ret),
			Q_ARG(OrderActionNew*, orderActionNew),
			Q_ARG(int, orderType)
		);

		if (!ret.isValid()) {
			MTS_FILE("NewOrderFail:%s\n", qPrintable(orderActionNew->toJsonString()));
		}
		return ret;
	}

	bool MtsProxyBase::cancelOrder(OrderActionCancel* orderActionCancel) {
		bool ret = false;
		QMetaObject::invokeMethod(OrderMgr::instance(), "cancelOrder", this->connectionType(),
			Q_RETURN_ARG(bool, ret),
			Q_ARG(OrderActionCancel*, orderActionCancel)
		);
		if (!ret) {
			MTS_FILE("CancelOrderFail:%s\n", qPrintable(orderActionCancel->toJsonString()));
		}
		return ret;
	}

	bool MtsProxyBase::subscribeQuotes(const QList<InstrumentId>& instruments) {
		Position* pos = nullptr;
		for (int i = 0, size = instruments.size(); i < size; ++i) {
			QMetaObject::invokeMethod(PositionMgr::instance(), "getPosition", this->connectionType(),
				Q_RETURN_ARG(Position*, pos),
				Q_ARG(InstrumentId, instruments[i]),
				Q_ARG(GetMode, GM_GET_OR_CREATE)
			);
		}

		bool isEmpty = (QuoteMgr::instance()->subscribedInstrumentCount() == 0);
		int ret = 0;
		QMetaObject::invokeMethod(QuoteMgr::instance(), "subscribe", this->connectionType(),
			Q_RETURN_ARG(int, ret),
			Q_ARG(QList<InstrumentId>, instruments)
		);
		if (isEmpty) { // enter the event loop when subscribe symbol first
			MtsMainThread::instance(_mode)->enterEventLoop(); //for simu only
		}
		return ret;
	}

	bool MtsProxyBase::unsubscribeQuotes(const QList<InstrumentId>& instruments) {
		int ret = 0;
		QMetaObject::invokeMethod(QuoteMgr::instance(), "unsubscribe", this->connectionType(),
			Q_RETURN_ARG(int, ret),
			Q_ARG(QList<InstrumentId>, instruments)
		);
		if (QuoteMgr::instance()->subscribedInstrumentCount() == 0) {
			//TODO: exit MtsMainThread::instance()->exitEventLoop(); and enter exec() .即，进入不产生genTicks 的 事件循环
		}
		return ret;
	}

	Position* MtsProxyBase::getPosition(const InstrumentId & id)  const {
		GetMode mode = GM_GET;
		Position* pos = nullptr;
		QMetaObject::invokeMethod(PositionMgr::instance(), "getPosition", this->connectionType(),
			Q_RETURN_ARG(Position*, pos),
			Q_ARG(InstrumentId, id),
			Q_ARG(GetMode, mode)
		);
		return pos;
	}

	mts::Position* MtsProxyBase::getPosition(const InstrumentId& id, int instanceId, int strategyId) const
	{
		GetMode mode = GM_GET;
		Position* pos = nullptr;
		QMetaObject::invokeMethod(PositionMgr::instance(), "getPosition", this->connectionType(),
			Q_RETURN_ARG(Position*, pos),
			Q_ARG(InstrumentId, id),
			Q_ARG(int, instanceId),
			Q_ARG(int, strategyId),
			Q_ARG(GetMode, mode)
		);
		return pos;
	}

	QList<TradingAccount*> MtsProxyBase::getAllTradingAccounts() const
	{
		QList<TradingAccount*>  accounts;
		QMetaObject::invokeMethod(TradingAccountMgr::instance(), "allTradingAccount", this->connectionType(),
			Q_RETURN_ARG(QList<TradingAccount*>, accounts)
		);

		return accounts;
	}

	QList<Order*> MtsProxyBase::getAllOrders() const
	{
		QList<Order*> orders;
		QMetaObject::invokeMethod(OrderMgr::instance(), "allOrders", this->connectionType(),
			Q_RETURN_ARG(QList<Order*>, orders)
		);
		return orders;
	}

	QList<Order*> MtsProxyBase::getActiveOrders(const InstrumentId& id) const {
		QList<Order*>  orders;
		QMetaObject::invokeMethod(OrderMgr::instance(), "allActiveOrders", this->connectionType(),
			Q_RETURN_ARG(QList<Order*>, orders),
			Q_ARG(InstrumentId, id)
		);

		return orders;
	}

	QList<OrderReportFill*> MtsProxyBase::getAllOrderFills() const
	{
		QList<OrderReportFill*>  fills;
		QMetaObject::invokeMethod(OrderFillMgr::instance(), "allFills", this->connectionType(),
			Q_RETURN_ARG(QList<OrderReportFill*>, fills)
		);

		return fills;
	}

	Quote* MtsProxyBase::getQuote(const InstrumentId& id) const {
		Quote* quote = nullptr;
		QMetaObject::invokeMethod(QuoteMgr::instance(), "getQuote", this->connectionType(),
			Q_RETURN_ARG(Quote*, quote),
			Q_ARG(InstrumentId, id)
		);

		return quote;
	}

	QList<Position*> MtsProxyBase::getAllPositions(int strategyId) const {
		//GetMode mode = GM_GET;
		QList<Position*> positions;
		QMetaObject::invokeMethod(PositionMgr::instance(), "allPositions", this->connectionType(),
			Q_RETURN_ARG(QList<Position*>, positions),
			Q_ARG(int, strategyId)
		);
		return positions;
	}

	DateTime MtsProxyBase::getNow() const {
		DateTime now;
		QMetaObject::invokeMethod(ClockMgr::instance(), "now", this->connectionType(),
			Q_RETURN_ARG(DateTime, now)
		);
		return now;
	}

	int MtsProxyBase::currentTradingDay() const {
		int date = 0;
		QMetaObject::invokeMethod(ClockMgr::instance(), "currentTradingDay", this->connectionType(),
			Q_RETURN_ARG(int, date)
		);
		return date;
	}

	QList<InstrumentId> MtsProxyBase::instrumentIds()const {
		DateTime now = getNow();
		QList<InstrumentId> ids;
		QMetaObject::invokeMethod(mts::InstrumentPropertyDb::instance(), "instrumentIds", this->connectionType(),
			Q_RETURN_ARG(QList<InstrumentId>, ids),
			Q_ARG(int, now.date()),
			Q_ARG(int, now.date())
		);
		return ids;
	}

	TradingAccount * MtsProxyBase::getTradingAccount(CurrencyId currency) const {
		TradingAccount * account = nullptr;
		QMetaObject::invokeMethod(mts::TradingAccountMgr::instance(), "getTradingAccount", this->connectionType(),
			Q_RETURN_ARG(TradingAccount *, account),
			Q_ARG(CurrencyId, currency)
		);

		return account;
	}

	void MtsProxyBase::initStrategyPos(int strategyId, const QString& dir)
	{
		QMetaObject::invokeMethod(OrderMgr::instance(), "initStrategyPos", this->connectionType(),
			Q_ARG(int, strategyId),
			Q_ARG(const QString&, dir)
		);
	}
}
