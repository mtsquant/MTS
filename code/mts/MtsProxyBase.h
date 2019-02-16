
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
#include <QtCore/QList>
#include <QtCore/QSet>
#include "base/DateTime.h"
#include "mts_core/StrategyInterface.h"
#include "mts_core/OrderAction.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/Position.h"
#include "mts_core/Order.h"
#include "mts_core/Quote.h"

class QCoreApplication;
namespace mts
{
	//此类作为MTS的代理，所有agent library 使用它和mts通信以及回调。
	class  MtsProxyBase
	{
	public:
		virtual int exec();
		virtual void exit(int code);

		//request - 此类方法调用后，异步发送到MTS 线程。其结果会由MTS线程直接通知回调（通过StrategyInterface）
		//并由各自语言的agent 用其语言相关的线程间通知机制通知主线程（比如：C++ 中用Qt emit signal，nodejs 中用libuv t_async ...）
		virtual bool initialize(StrategyInterface* callbacks, const QVariantMap& params);
		QVariantMap params() const;

		int newTimer(int beginTime, int interval);
		bool removeTimer(int id);

		virtual mts::OrderId newOrder(OrderActionNew* orderActionNew, int orderType);
		bool cancelOrder(OrderActionCancel* orderActionCancel);

		bool subscribeQuotes(const QList<InstrumentId>& instruments);
		bool unsubscribeQuotes(const QList<InstrumentId>& instruments);

		//query - 此类方法，异步发送到MTS 线程， 然后等待结果后，直接返回结果
		Position* getPosition(const InstrumentId& id) const;
		Position* getPosition(const InstrumentId& id, int instanceId, int strategyId) const;
		QList<TradingAccount*> getAllTradingAccounts() const;
		QList<Order*> getAllOrders() const;
		QList<Order*> getActiveOrders(const InstrumentId& id) const;
		QList<OrderReportFill*> getAllOrderFills() const;
		Quote* getQuote(const InstrumentId& id) const;

		QList<Position*> getAllPositions(int strategyId = DEFAULT_STRATEGY_ID) const;

		DateTime getNow() const;
		int currentTradingDay() const;
		QList<InstrumentId> instrumentIds()const;

		TradingAccount* getTradingAccount(CurrencyId) const;

		void initStrategyPos(int strategyId, const QString& dir);
	protected:
		MtsProxyBase();
		virtual ~MtsProxyBase();
		EnvironmentMode _mode;
		virtual bool createMtsMgrs(const QVariantMap & params);
		virtual bool initMtsMgrs();
		Qt::ConnectionType  connectionType() const;
	private:
		QVariantMap _params;
		StrategyInterface* _callback;

		bool loadMtsComponents(const QString& libraryPath);
		bool startRunMtsEnvir(const QVariantMap & params);
	};

}