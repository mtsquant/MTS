
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
	class  MtsProxyBase
	{
	public:
		virtual int exec();
		virtual void exit(int code);

		virtual bool initialize(StrategyInterface* strategyHub, const QVariantMap& params);
		QVariantMap params() const;

		int newTimer(int beginTime, int interval);
		bool removeTimer(int id);

		virtual QString newOrder(OrderActionNew* orderActionNew, int orderType);
		bool cancelOrder(OrderActionCancel* orderActionCancel);

		bool subscribeQuotes(const QList<InstrumentId>& instruments);
		bool unsubscribeQuotes(const QList<InstrumentId>& instruments);

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
		StrategyInterface* _callback;//strategyHub

		bool loadMtsComponents(const QString& libraryPath);
		bool startRunMtsEnvir(const QVariantMap & params);
	};

}