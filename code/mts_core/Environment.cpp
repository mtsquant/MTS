
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
#include "Environment.h"
#include "base/MtsLog.h"
#include <QtCore/qcoreapplication.h>
#include "./EnvironmentFactory.h"


#include "mts_core/ConfigParams.h"
#include "mts_core/OrderType.h"



namespace mts
{



	Environment * Environment::instance() {
		static Environment envir;
		return &envir;
	}

	Environment::Environment ()
		:_clock ( nullptr ) , _feeds ( nullptr ) , _trade ( nullptr ) , /*_eventLoopRunner(nullptr), */_accountCallback ( nullptr ) , _mode ( ENVIR_UNKNOWN )
		, _instanceId(0), _strategyPosCheck(1), _positionPersistenceEnable(false)
	{
		assert ( isCurrentMtsThread () );
	}

	void Environment::init ( Clock* clock , Feeds* feeds , Trade* trade/*, EventLoopRunner* evtLoopRunner*/ ){
		_clock = clock;
		_feeds = feeds;
		_trade = trade;
	}

	bool Environment::load(EnvironmentMode mode,const QString& libraryPath) {
		static EnvironmentFactory factory(libraryPath);
		if (!factory.loadTo(this, mode)) {
			return false;
		}

		if (!isValid()) {
			return false;
		}

		_mode = mode;
		return true;

	}

	bool Environment::initialize(FeedsCallback* feedsCb/*quoteMgr*/, TradeCallback* tradeCb/*orderMgr*/, AccountCallback* accountCallback/*accountMgr*/, const QVariantMap & params) {
		assert(ConfigParams::isInitialized());
		MTS_SET_FP_PATH ( qPrintable (ConfigParams::instance()->logFile()) );

		_instanceId = ConfigParams::instance()->instanceId ();
		_strategyPosCheck = ConfigParams::instance()->strategyPosCheck();
		_positionPersistenceEnable = ConfigParams::instance()->positionPersistenceEnable();
		_accountCallback = accountCallback;

		connect(this->feeds(), SIGNAL(initializeDone()),
			this, SLOT(onInitialized()));
		connect(this->trade(), SIGNAL(initializeDone()),
			this, SLOT(onInitialized()));

		this->_clock->addDateCallback(this->feeds());
		this->_clock->addDateCallback(tradeCb);

		if (!_clock->initialize(params)) {
			return false;
		}

		QMetaObject::invokeMethod(this, "onEnvirInitialized");

		if (!_feeds->initialize(params)) {
			return false;
		}
		this->feeds()->addCallback(feedsCb);

		if (!_trade->initialize(params)) {
			return false;
		}
		this->trade()->addCallback(tradeCb);



		return true;
	}

	void Environment::onEnvirInitialized() {
		_accountCallback->onEnvirInitialized();
	}

	void Environment::onInitialized() {
		if (this->feeds()->isInitialized() && this->trade()->isInitialized()) {
			disconnect(this->feeds(), SIGNAL(initializeDone()));
			disconnect(this->trade(), SIGNAL(initializeDone()));
			_accountCallback->onInitialized(nullptr);//&account);
		}
	}

	EnvironmentMode Environment::mode() const {
		return _mode;
	}

	bool Environment::isCurrentMainThread() const{
		return QThread::currentThread() == QCoreApplication::instance()->thread();
	}

	bool Environment::isCurrentMtsThread() const{
		return this->thread()==QThread::currentThread();
	}

	QString Environment::genOrderReferenceId(int strategyId,int orderType, int directionSide, int priceType,const InstrumentId& instrumentId) const {
		assert(isStrategyIdValid(strategyId));
		if (!isStrategyIdValid(strategyId)) {
			MTS_ERROR("Invalid strategyId %d\n", strategyId);
			return "";
		} else {
			return trade()->createOrderId(_instanceId, strategyId, orderType, directionSide, priceType, instrumentId);
		}
	}


	bool Environment::isCurrentInstanceInstanceId (int instanceId) const
	{
		if (mode() == ENVIR_REAL || mode() == ENVIR_BETA) {
			return instanceId == _instanceId;
		} else {
			return true;
		}
	}

	int Environment::instanceId() const
	{
		return _instanceId;
	}

	int Environment::strategyPosCheck() const
	{
		return _strategyPosCheck;
	}

	bool Environment::positionPersistenceEnable() const
	{
		return _positionPersistenceEnable;
	}

	bool Environment::isValid() const {
		return _clock
			&& _feeds
			&& _trade
			;
	}

	Clock * Environment::clock() const {
		return _clock;
	}

	Feeds * Environment::feeds() const {
		return _feeds;
	}

	Trade * Environment::trade() const {
		return _trade;
	}


}
