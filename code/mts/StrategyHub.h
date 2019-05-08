
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
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QJsonArray>
#include <string>
#include "base/DateTime.h"
#include "mts_core/StrategyInterface.h"
#include "mts_core/OrderAction.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/Position.h"
#include "mts_core/Order.h"
#include "mts_core/Quote.h"
#include "mts_api.h"

#ifdef _WIN32
inline int qHash(const std::string& s) {
    return qHash(QLatin1String(s.c_str()));
}
#else
#endif


namespace mts
{
	class MTS_API StrategyHub :public StrategyInterface
	{
	public:
		int exec();
		void exit(int);

		bool initialize( const QVariantMap& params);

		bool registerStrategy(StrategyInterface* strategy);
		void unregisterStrategy(StrategyInterface* strategy);

		int newTimer(StrategyInterface* strategy, int beginTime, int interval);
		int newOnceTimer(StrategyInterface* strategy, int beginTime);
		bool removeTimer(StrategyInterface* strategy,int id);

		virtual QString newOrder(StrategyInterface* strategy, OrderActionNew* orderActionNew,int orderType);
		bool cancelOrder(StrategyInterface* strategy, OrderActionCancel* orderActionCancel);

		bool subscribeQuotes(StrategyInterface* strategy, const QList<InstrumentId>& instruments);
		bool unsubscribeQuotes(StrategyInterface* strategy, const QList<InstrumentId>& instruments);
		void unsubscribeAllQuotes ( StrategyInterface* strategy );

		QList<TradingAccount*> getAllTradingAccounts() const;
		Position* getPosition(const InstrumentId& id) const;
		Position* getPosition(const InstrumentId& id, int instanceId, int strategyId) const;
		QList<Order*> getAllOrders(StrategyInterface* strategy) const;
		QList<Order*> getActiveOrders(StrategyInterface* strategy, const InstrumentId& id) const;
		QList<OrderReportFill*> getAllOrderFills() const;

		Quote* getQuote(const InstrumentId& id) const;

		QList<Position*> getAllPositions(int strategyId = DEFAULT_STRATEGY_ID) const;

		DateTime getNow() const;
		QList<InstrumentId> instrumentIds()const;
		int currentTradingDay() const;

		int subscribedInstrumentCount() const;

		TradingAccount* getTradingAccount(CurrencyId) const;

		QJsonArray allStrategiesToJson() const;
		std::string  allStrategiesToJsonString() const;
	protected:
		StrategyHub();
		~StrategyHub();

	private:
		QHash<int/*strategy id*/, StrategyInterface* >  _idStrategies;				//all strategies

		QHash<int/*timer id*/, StrategyInterface* >  _timerStrategies;			//all strategies that created timers
		QHash<InstrumentId, QSet<StrategyInterface*> >  _quoteStrategies;	//all strategies that subscribe quotes

		QSet<int/*once  timer id*/> _onceTimerIds;

	protected:
		virtual std::string name () const override{
			return "StrategyMgr";
		}
		virtual int strategyId () const override{
			return 0;
		}

		virtual void onTimeout(int timerId) override;
		virtual void onQuoteSnapshotUpdate(QuoteSnapshotPtr) override;
		virtual void onOrderNewDone(OrderReportNewDone *) override;
		virtual void onOrderNewReject(OrderReportNewReject *) override;
		virtual void onOrderFill(OrderReportFill *) override;
		virtual void onOrderCancelDone(OrderReportCancelDone *) override;
		virtual void onOrderCancelReject(OrderReportCancelReject *) override;
		virtual void onOrderOtherReport(OrderReport *) override;
		virtual void onOrderUpdate(Order *) override;
		virtual void onPositionUpdate(Position *) override;
		virtual void onEnvirInitialized() override;
		virtual void onInitialized(Account *) override;
	public:
		virtual void onBusinessDateChanged(int businessDate) override;
	private:
		virtual void onBarUpdate(CalcBarPtr) override;

		QList<StrategyInterface*> getCallbacks(int strategyId) const;

		void removeAllOnceTimerIds();

	};

}

