
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
#include "Environment.h"
#include <QtCore/QDir>
#include "base/MtsLog.h"
#include <QtCore/qcoreapplication.h>
#include "mts_core/ConfigParams.h"
#include "base/Library.h"





namespace mts
{

	QString Environment::getLibraryFileName(const QString& componentName,const QString& mode) {
		QString dllFileName=
	#ifndef NDEBUG
		QString("%1_%2_d").arg(componentName).arg(mode);
	#else
		QString("%1_%2").arg(componentName).arg(mode);
	#endif

		return
	#ifndef _WIN32
		"lib"+
	#endif
		dllFileName;
	}

	class  EnvironmentFactory
	{
	public:
		EnvironmentFactory(const QString& libraryPath);
		~EnvironmentFactory(void);

		bool loadTo(Environment*, EnvironmentMode);

	private:
		const QString _libraryPath;
		bool initEnvironmentForSimu(Environment*);
		bool initEnvironmentForBeta(Environment*);
		bool initEnvironmentForReal(Environment*);

	private:
		Clock* createClockComponent(const QString& modeName);
		Feeds* createFeedsComponent(const QString& modeName);
		Trade* createTradeComponent(const QString& modeName);

		Library  _clockLib;
		Library _feedsLib;
		Library _tradeLib;
	};



	Environment * Environment::instance() {
		static Environment envir;
		return &envir;
	}

	Environment::Environment ()
		:_clock ( nullptr ) , _feeds ( nullptr ) , _trade ( nullptr ) , /*_eventLoopRunner(nullptr), */_accountCallback ( nullptr ) , _mode ( ENVIR_UNKNOWN )
		, _instanceId(0), _strategyPosCheck(1)
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

	bool Environment::initialize(FeedsCallback* feedsCb, TradeCallback* tradeCb, AccountCallback* accountCallback, const QVariantMap & params) {
		assert(ConfigParams::isInitialized());
		MTS_SET_FP_PATH ( qPrintable (ConfigParams::instance()->logFile()) );

		_instanceId = ConfigParams::instance()->instanceId ();
		_strategyPosCheck = ConfigParams::instance()->strategyPosCheck();
		_accountCallback = accountCallback;
		this->feeds()->addCallback(feedsCb);
		this->trade()->addCallback(tradeCb);

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

		if (!_trade->initialize(params)) {
			return false;
		}



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

	OrderId Environment::genOrderReferenceId(int strategyId,int orderType,const InstrumentId& instrumentId) const {
		assert(isStrategyIdValid(strategyId));
		if (!isStrategyIdValid(strategyId)) {
			MTS_ERROR("Invalid strategyId %d\n", strategyId);
			return OrderId();
		} else {
			return trade()->createOrderId(_instanceId, strategyId, orderType, instrumentId);
		}
	}


	bool Environment::isCurrentInstanceInstanceId (int instanceId) const
	{
		if (mode() == ENVIR_REAL) {
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




		

	EnvironmentFactory::EnvironmentFactory(const QString& libraryPath)
		:_libraryPath(libraryPath)
	{
	}
	
	EnvironmentFactory::~EnvironmentFactory(void) 
	{
	}

	bool EnvironmentFactory::loadTo(Environment*  envir,EnvironmentMode mode) {
		if (!QDir(_libraryPath).exists()) {
			MTS_ERROR("No such dir '%s'\n", qPrintable(_libraryPath));
			return false;
		}

		QCoreApplication::addLibraryPath(_libraryPath);
		MTS_DEBUG("Mts library path:'%s'\n", qPrintable(_libraryPath));

		if (mode == ENVIR_SIMU) {
			return initEnvironmentForSimu(envir);
		} else if (mode == ENVIR_BETA) {
			return initEnvironmentForBeta(envir);
		} else if (mode == ENVIR_REAL) {
			return initEnvironmentForReal(envir);
		} else {
			MTS_ERROR("Unknown environment\n");
			return false;
		}
	}

	bool EnvironmentFactory::initEnvironmentForSimu(Environment* envir) {
		envir->init(
			createClockComponent("simu"),
			createFeedsComponent("simu"),
			createTradeComponent("simu")
		);

		if (!QObject::connect(envir->feeds(), SIGNAL(feedFileOpened(const QString&, const QJsonObject&, FeedsType)),
										envir->clock(), SLOT(onFeedFileOpened(const QString&, const QJsonObject&, FeedsType))))
		{
			MTS_ERROR("Failed to connect FeedsSimu signal 'feedFileOpened()' to ClockSimu slot '' on Simu mode\n");
			return false;
		}

		return envir->isValid();
	}

	bool EnvironmentFactory::initEnvironmentForBeta(Environment* envir) {
		envir->init(
			createClockComponent("real"),
			createFeedsComponent("real"),
			createTradeComponent("simu")
			);
		return envir->isValid();
	}

	
	bool EnvironmentFactory::initEnvironmentForReal(Environment* envir) {
		envir->init(
			createClockComponent("real"),
			createFeedsComponent("real"),
			createTradeComponent("real")
		);
		return envir->isValid();
	}

	Clock * EnvironmentFactory::createClockComponent(const QString& modeName) {
        QString fileName=QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("clock", modeName));
        MTS_DEBUG("Load '%s'\n",qPrintable(fileName));
        _clockLib.setFileName(fileName);
		if (!_clockLib.load()) {
			MTS_ERROR("Failed to load '%s':'%s'\n", qPrintable(_clockLib.fileName()),_clockLib.errorString().toUtf8().constData());
			return nullptr;
		}

        MTS_DEBUG("Load '%s'\n",qPrintable(_clockLib.fileName()));
		typedef Clock* (*CreateClock) ();
		CreateClock createClock = (CreateClock) _clockLib.resolve("createClock");
		if (!createClock) {
			MTS_ERROR("No such function 'createClock'  in '%s':'%s'\n", qPrintable(_clockLib.fileName()), qPrintable(_clockLib.errorString()));
			return nullptr;
		}

		return createClock();
	}



	Feeds * EnvironmentFactory::createFeedsComponent(const QString& modeName) {
		QString filePath;
		if (modeName.compare("real", Qt::CaseInsensitive)==0) {
			assert(ConfigParams::instance()->mode() == mts::EnvironmentMode::ENVIR_REAL);
			if (ConfigParams::instance()->allExchSessions().size() > 1) { //use feed_proxy for multi-feeds dlls
				filePath= QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("feeds", "proxy"));
			}else {
				auto feedsProtocol = ConfigParams::instance()->feedsFrontProtocol();
				QString protocolName = frontProtocolName(feedsProtocol);
				filePath = QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("feeds", modeName + "_" + protocolName));
			}
		} else {
			filePath = QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("feeds", modeName));
		}

		_feedsLib.setFileName(filePath);
		if (!_feedsLib.load()) {
			MTS_ERROR ( "Failed to load '%s':'%s'\n" , qPrintable ( _feedsLib.fileName () ) , _feedsLib.errorString ().toUtf8 ().constData () );
			return nullptr;
		}

		typedef Feeds* (*CreateFeeds) ();
		CreateFeeds createFeeds = (CreateFeeds)_feedsLib.resolve("createFeeds");
		if (!createFeeds) {
			MTS_ERROR("No such function 'createFeeds'  in '%s':'%s'\n", qPrintable(_feedsLib.fileName()), qPrintable(_feedsLib.errorString()));
			return nullptr;
		}

		return createFeeds();
	}


	Trade * EnvironmentFactory::createTradeComponent(const QString& modeName) {
		QString filePath;
		if (modeName.compare("real", Qt::CaseInsensitive)==0) {
			auto tradeProtocol = ConfigParams::instance()->tradeFrontProtocol();
			QString protocolName = frontProtocolName(tradeProtocol);
			filePath = QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("trade", modeName + "_" + protocolName));
		} else {
			filePath = QDir(_libraryPath).absoluteFilePath(Environment::getLibraryFileName("trade", modeName));
		}
		_tradeLib.setFileName(filePath);
		if (!_tradeLib.load()) {
			MTS_ERROR ( "Failed to load '%s':'%s'\n" , qPrintable ( _tradeLib.fileName () ) , _tradeLib.errorString ().toUtf8 ().constData () );
			return nullptr;
		}

		typedef Trade* (*CreateTrade) ();
		CreateTrade createTrade = (CreateTrade)_tradeLib.resolve("createTrade");
		if (!createTrade) {
			MTS_ERROR("No such function 'createTrade'  in '%s':'%s'\n", qPrintable(_tradeLib.fileName()), qPrintable(_tradeLib.errorString()));
			return nullptr;
		}

		return createTrade();
	}

}
