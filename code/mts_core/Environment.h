
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
#include <QtCore/QLibrary>
#include <QtCore/QThread>
#include "enums.h"

#include "Clock.h"
#include "Feeds.h"
#include "Trade.h"
#include "StrategyInterface.h"

namespace mts
{

	class MTS_CORE_API Environment:public QObject
	{
		Q_OBJECT

	public:
		static Environment* instance(); //The first call should be in MtsThread
	public Q_SLOTS:
		bool load(EnvironmentMode,  const QString& libraryPath); //根据mode来load不同的component dll
		bool initialize(FeedsCallback*, TradeCallback*, AccountCallback* ,const QVariantMap & params);
	public:
		QString libraryPath()const;
		EnvironmentMode mode() const;
		bool isCurrentMainThread() const;
		bool isCurrentMtsThread() const;
		QString genOrderReferenceId(int strategyId/*1-9*/,int orderType, int directionSide, int priceType, const InstrumentId& instrumentId) const;
		bool isCurrentInstanceInstanceId(int instanceId) const;
		int instanceId()const;
		int strategyPosCheck() const;
		bool positionPersistenceEnable() const;

		bool isValid() const;
		Clock*	clock() const;
		Feeds*	feeds() const;
		Trade*	trade() const;

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
		int _instanceId;
		int _strategyPosCheck;
		bool _positionPersistenceEnable;
	};


}
