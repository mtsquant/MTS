
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
//#include "TickSrc.h"
#include <QtCore/QLibrary>
#include <QtCore/QThread>
#include "enums.h"

#include "Clock.h"
#include "Feeds.h"
#include "Trade.h"
//#include "mts_core/EventLoopRunner.h"
#include "StrategyInterface.h"
#include "mts_core/OrderId.h"
//#include "MtsMode.h"

namespace mts
{

	class MTS_CORE_API Environment:public QObject
	{
		Q_OBJECT

	public:
		static Environment* instance(); //The first call should be in MtsThread
		static QString getLibraryFileName(const QString& componentName, const QString& mode);
	public Q_SLOTS:
		bool load(EnvironmentMode,  const QString& libraryPath); //根据mode来load不同的component dll
		bool initialize(FeedsCallback*, TradeCallback*, AccountCallback* ,const QVariantMap & params);
	public:
		//QString name() const;
		//MtsMode* mode() const;
		QString libraryPath()const;
		EnvironmentMode mode() const;
		bool isCurrentMainThread() const;
		bool isCurrentMtsThread() const;
		mts::OrderId genOrderReferenceId(int strategyId/*1-9*/,int orderType, const InstrumentId& instrumentId) const;
		bool isCurrentInstanceInstanceId(int instanceId) const;
		//static bool isStrategyReferenceId ( qint64 referenceId , int strategyId );
		//static int parseStrategyId ( qint64 referenceId );
		//static int parseOrderType(qint64 referenceId);
		int instanceId()const;
		int strategyPosCheck() const;

		bool isValid() const;
		Clock*	clock() const;
		Feeds*	feeds() const;
		Trade*	trade() const;
		//EventLoopRunner* eventLoopRunner() const;

		//utility 
		DateTime now() const {
			assert(isValid());
			return clock()->now();
		}
	private Q_SLOTS:
		void onEnvirInitialized();
		void onInitialized();
	private:
		Environment();

		void init(Clock*, Feeds*, Trade* 
				/*, EventLoopRunner* */
		);
	private:
		EnvironmentMode _mode;
		friend class EnvironmentFactory;
		AccountCallback* _accountCallback;
		Clock* 	_clock;
		Feeds* 	_feeds;
		Trade* 	_trade;
		//EventLoopRunner* _eventLoopRunner;
		int _instanceId;
		int _strategyPosCheck;
	};


}
