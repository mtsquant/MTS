
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
#include "mts/AbsStrategy.h"

using namespace boost::python;
using namespace boost;
#ifdef _WIN32
namespace np=boost::python::numpy;
#endif


using namespace mts;


class PythonStrategyInterface 
{
public:
	virtual void onEnvirInitialized() =0;
	virtual void onInitialized(const PyAccount& data) =0;
	virtual void onTimer(int) =0;
	virtual void onQuoteSnapshotUpdate(const PyQuote& quote) =0;
	virtual void onOrderNewDone(const PyOrderNewDone& orderNewDone) =0;
	virtual void onOrderNewReject(const PyOrderNewReject& orderNewReject) =0;
	virtual void onOrderFill(const PyOrderFill&) =0;
	virtual void onOrderCancelDone(const PyOrderCancelDone&) =0;
	virtual void onOrderCancelReject(const PyOrderCancelReject&) =0;
	virtual void onOrderOtherReport(const PyOrderReport&) =0;
	virtual void onPositionUpdate(const PyPosition&) =0;
	virtual void onOrderUpdate(const PyOrder&) =0;
	virtual void onBusinessDateChanged(int) = 0;
	virtual void onBarUpdate(const PyBar& data) =0;
};

class QCoreApplication;
class PythonStrategy :public AbsStrategy,public PythonStrategyInterface
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

	std::string newOrder(dict params);
	bool cancelOrder(dict params);

	bool subscribeQuotes(list symbols);
	bool unsubscribeQuotes(list symbols);
	void unsubscribeAllQuotes ();

	PyQuote getQuote(const std::string& symbol) const;

	list getAllTradingAccounts() const;
	PyPosition getPosition(const std::string& symbol) const;
	list getAllPositions() const;

	list getAllOrders() const;
	list getActiveOrders(const std::string& symbol) const;
	list getAllOrderFills() const;

	list getAllInstrumentSymbols() const;

	PyDateTime getNow() const;
	qint64 getNowMicrosecs() const;

private Q_SLOTS:
	virtual void doEnvirInitialized() override;
	virtual void doInitialized(AccountPtr) override;
	virtual void doTimeout(int) override;
	virtual void doOrderNewDone(OrderReportNewDonePtr) override;
	virtual void doOrderNewReject(OrderReportNewRejectPtr) override;
	virtual void doOrderFill(OrderReportFillPtr) override;
	virtual void doOrderCancelDone(OrderReportCancelDonePtr) override;
	virtual void doOrderCancelReject(OrderReportCancelRejectPtr) override;
	virtual void doOrderOtherReport(OrderReportPtr) override;
	virtual void doQuoteSnapshotUpdate(QuoteSnapshotPtr) override;
	virtual void doPositionUpdate(PositionPtr) override;
	virtual void doOrderUpdate(OrderPtr) override;
	virtual void doBusinessDateChanged(int) override;
	virtual void doBarUpdate(CalcBarPtr) override;

public: //the following callback will be overrided by python script
	virtual void onEnvirInitialized() override {};
	virtual void onInitialized(const PyAccount& data) override {};
	virtual void onTimer ( int ) override {};
	virtual void onQuoteSnapshotUpdate(const PyQuote& quote) override {};
	virtual void onOrderNewDone(const PyOrderNewDone& orderNewDone) override {};
	virtual void onOrderNewReject(const PyOrderNewReject& orderNewReject) override {};
	virtual void onOrderFill(const PyOrderFill&)  override {};
	virtual void onOrderCancelDone(const PyOrderCancelDone& )  override {};
	virtual void onOrderCancelReject(const PyOrderCancelReject& ) override {};
	virtual void onOrderOtherReport(const PyOrderReport& ) override {};
	virtual void onPositionUpdate(const PyPosition&)  override {};
	virtual void onOrderUpdate(const PyOrder& )  override {};
	virtual void onBusinessDateChanged(int) override {}
	virtual void onBarUpdate(const PyBar& data)  override {};
private:
    friend class mts::CrossThreadNotifier;

	bool checkParamsNewOrder ( const dict& params );
};

