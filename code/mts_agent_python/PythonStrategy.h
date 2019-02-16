
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
#include <QtCore/QObject>
#include "base/DateTime.h"
#include "mts_core/StrategyInterface.h"
#include "mts_core/OrderAction.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/Position.h"
#include "mts_core/Order.h"
#include "mts_core/Quote.h"
#include "mts/CrossThreadNotifier.h"

#include <list>
#include <set>
#include <map>
#include <string>

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/object.hpp>
#include <boost/python.hpp>

#include "PyDateTime.h"
#include "PyAccount.h"
#include "PyQuote.h"
#include "PyBar.h"
#include "PyOrderNewDone.h"
#include "PyOrderNewReject.h"
#include "PyOrderCancelDone.h"
#include "PyOrderCancelReject.h"
#include "PyOrderFill.h"
#include "PyOrder.h"
#include "PyPosition.h"
#ifdef _WIN32
#include <boost/python/numpy.hpp>
#endif

using namespace boost::python;
using namespace boost;
#ifdef _WIN32
namespace np=boost::python::numpy;
#endif


using namespace mts;

class QCoreApplication;
class PythonStrategy :public QObject
{
	Q_OBJECT
public:
	PythonStrategy(const std::string& name,int strategyId, const std::string& strategyPath = std::string());
	virtual ~PythonStrategy();


	static bool initialize(dict params);
	static bool initialize(const std::string& paramsJsonFile);
	static bool initialize ( const std::string& paramsJsonFile, dict params );
	static int exec();
	static void exit(int code);

	int newTimer(int beginTime,int interval);
	int newOnceTimer(int beginTime);
	bool removeTimer(int id);

	std::string newOrder(dict params);
	bool cancelOrder(dict params);

	bool subscribeQuotes(list symbols);
	bool unsubscribeQuotes(list symbols);
	void unsubscribeAllQuotes ();

	PyQuote getQuote(const std::string& symbol) const;

	list getAllTradingCounts() const;
	PyPosition getPosition(const std::string& symbol) const;
	list getAllPositions() const;

	list getAllOrders() const;
	list getActiveOrders(const std::string& symbol) const;
	list getAllOrderFills() const;

	list getAllInstrumentSymbols() const;

	PyDateTime getNow() const;
	qint64 getNowMicrosecs() const;

private Q_SLOTS:
	void doEnvirInitialized();
	void doInitialized(AccountPtr);
	void doTimeout(int);
	void doOrderNewDone(OrderReportNewDonePtr);
	void doOrderNewReject(OrderReportNewRejectPtr);
	void doOrderFill(OrderReportFillPtr);
	void doOrderCancelDone(OrderReportCancelDonePtr);
	void doOrderCancelReject(OrderReportCancelRejectPtr);
	void doOrderOtherReport(OrderReportPtr);
	void doQuoteSnapshotUpdate(QuoteSnapshotPtr);
	void doPositionUpdate(PositionPtr);
	void doOrderUpdate(OrderPtr);
	void doBusinessDateChanged(int);
	void doBarUpdate(CalcBarPtr);
public:
	virtual void onEnvirInitialized() {};
	virtual void onInitialized(const PyAccount& data) {};
	virtual void onTimer ( int ){};
	virtual void onQuoteSnapshotUpdate(const PyQuote& quote) {};
	virtual void onOrderNewDone(const PyOrderNewDone& orderNewDone) {};
	virtual void onOrderNewReject(const PyOrderNewReject& orderNewReject) {};
	virtual void onOrderFill(const PyOrderFill&) {};
	virtual void onOrderCancelDone(const PyOrderCancelDone& ) {};
	virtual void onOrderCancelReject(const PyOrderCancelReject& ) {};
	virtual void onOrderOtherReport(const PyOrderReport& ) {};
	virtual void onPositionUpdate(const PyPosition&) {};
	virtual void onOrderUpdate(const PyOrder& ) {};
	virtual void onBusinessDateChanged(int) {}
	virtual void onBarUpdate(const PyBar& data) {};
private:
    friend class mts::CrossThreadNotifier;
	CrossThreadNotifier _notifier;
	QCoreApplication* _app;

	bool checkParamsNewOrder ( const dict& params );
};

