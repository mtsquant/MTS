
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
#include "./EnvironmentFactory.h"
#include <QtCore/QDir>
#include "base/MtsLog.h"
#include <QtCore/qcoreapplication.h>
#include "mts_core/Environment.h"
#include "mts_core/ConfigParams.h"


namespace mts {


	QString EnvironmentFactory::getLibraryFileName(const QString& componentName, const QString& mode) {
		QString dllFileName =
#ifndef NDEBUG
			QString("%1_%2_d").arg(componentName).arg(mode);
#else
			QString("%1_%2").arg(componentName).arg(mode);
#endif

		return
#ifndef _WIN32
			"lib" +
#endif
			dllFileName.toLower();
	}


	EnvironmentFactory::EnvironmentFactory(const QString& libraryPath)
		:_libraryPath(libraryPath)
	{
	}

	EnvironmentFactory::~EnvironmentFactory(void)
	{
	}

	bool EnvironmentFactory::loadTo(Environment*  envir, EnvironmentMode mode) {
		if (!QDir(_libraryPath).exists()) {
			MTS_ERROR("No such dir '%s'\n", qPrintable(_libraryPath));
			return false;
		}

		QCoreApplication::addLibraryPath(_libraryPath);
		MTS_DEBUG("Mts library path:'%s'\n", qPrintable(_libraryPath));

		envir->init(
			createClockComponent(ConfigParams::instance()->componentMode(MTS_COMP_CLOCK)),
			createFeedsComponent(ConfigParams::instance()->componentMode(MTS_COMP_FEEDS)),
			createTradeComponent(ConfigParams::instance()->componentMode(MTS_COMP_TRADE))
		);

		if (mode == ENVIR_SIMU) {
			if (!QObject::connect(envir->feeds(), SIGNAL(feedFileOpened(const QString&, const QJsonObject&, FeedsType)),
				envir->clock(), SLOT(onFeedFileOpened(const QString&, const QJsonObject&, FeedsType))))
			{
				MTS_ERROR("Failed to connect FeedsSimu signal 'feedFileOpened()' to ClockSimu slot '' on Simu mode\n");
				return false;
			}
		}
		return envir->isValid();
	}







	Clock * EnvironmentFactory::createClockComponent(EnvironmentMode mode) {
		const QString modeName = environmentModeName(mode);
		const QString fileName = getLibraryFileName("clock", modeName);
		QString filePath = QDir(_libraryPath).absoluteFilePath(fileName);
		MTS_DEBUG("Load '%s'\n", qPrintable(filePath));
		_clockLib.setFileName(filePath);
		if (!_clockLib.load()) {
			MTS_ERROR("Failed to load '%s':'%s'\n", qPrintable(fileName), _clockLib.errorString().toUtf8().constData());
			return nullptr;
		}

		typedef Clock* (*CreateClock) (EnvironmentMode);
		CreateClock createClock = (CreateClock)_clockLib.resolve("createClock");
		if (!createClock) {
			MTS_ERROR("No such function 'createClock'  in '%s':'%s'\n", qPrintable(fileName),_clockLib.errorString().toUtf8().constData());
			return nullptr;
		}

		return createClock(mode);
	}



	Feeds * EnvironmentFactory::createFeedsComponent(EnvironmentMode mode) {
		const QString modeName = environmentModeName(mode);
		QString fileName;
		if (mode== mts::EnvironmentMode::ENVIR_REAL) {
			if (ConfigParams::instance()->allExchSessions().size() > 1) { //use feed_proxy for multi-feeds dlls
				fileName = getLibraryFileName("feeds", "proxy");
			} else {
				auto feedsProtocol = ConfigParams::instance()->feedsFrontProtocol(ConfigParams::instance()->allExchSessions().first());
				QString protocolName = frontProtocolName(feedsProtocol);
				fileName = getLibraryFileName("feeds", modeName + "_" + protocolName);
			}
		}else {
			fileName = getLibraryFileName("feeds", modeName);
		}
		QString filePath = QDir(_libraryPath).absoluteFilePath(fileName);
		MTS_DEBUG("Load '%s'\n", qPrintable(filePath));
		_feedsLib.setFileName(filePath);
		if (!_feedsLib.load()) {
			MTS_ERROR("Failed to load '%s':'%s'\n", qPrintable(fileName), _feedsLib.errorString().toUtf8().constData());
			return nullptr;
		}

		typedef Feeds* (*CreateFeeds) (EnvironmentMode);
		CreateFeeds createFeeds = (CreateFeeds)_feedsLib.resolve("createFeeds");
		if (!createFeeds) {
			MTS_ERROR("No such function 'createFeeds'  in '%s':'%s'\n", qPrintable(fileName), _feedsLib.errorString().toUtf8().constData());
			return nullptr;
		}

		return createFeeds(mode);
	}


	Trade * EnvironmentFactory::createTradeComponent(EnvironmentMode mode) {
		const QString modeName = environmentModeName(mode);
		QString fileName;
		if (mode == mts::EnvironmentMode::ENVIR_REAL) {
			if (ConfigParams::instance()->allExchSessions().size() > 1) { //use trade_proxy for multi-trade dlls
				fileName = getLibraryFileName("trade", "proxy");
			} else {
				auto tradeProtocol = ConfigParams::instance()->tradeFrontProtocol(ConfigParams::instance()->allExchSessions().first());
				QString protocolName = frontProtocolName(tradeProtocol);
				fileName = getLibraryFileName("trade", modeName + "_" + protocolName);
			}
		}else {
			fileName = getLibraryFileName("trade", modeName);
		}

		QString filePath = QDir(_libraryPath).absoluteFilePath(fileName);
		MTS_DEBUG("Load '%s'\n", qPrintable(filePath));
		_tradeLib.setFileName(filePath);
		if (!_tradeLib.load()) {
			MTS_ERROR("Failed to load '%s':'%s'\n", qPrintable(fileName), _tradeLib.errorString().toUtf8().constData());
			return nullptr;
		}

		typedef Trade* (*CreateTrade) (EnvironmentMode);
		CreateTrade createTrade = (CreateTrade)_tradeLib.resolve("createTrade");
		if (!createTrade) {
			MTS_ERROR("No such function 'createTrade'  in '%s':'%s'\n", qPrintable(fileName), _tradeLib.errorString().toUtf8().constData());
			return nullptr;
		}

		return createTrade(mode);
	}

}