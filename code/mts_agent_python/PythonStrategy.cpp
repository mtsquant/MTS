
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
#include "PythonStrategy.h"
#include <iostream>
#include "mts/StrategyMgr.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include "base/MtsLog.h"
#include "base/MtsUtils.h"
#include "mts_core/InstrumentPropertyDb.h"

#include "mts_core/OrderType.h"
#include "base/MtsPath.h"
#include "mts_core/ConfigParams.h"
#include "PyTradingAccount.h"
#include "PythonUtil.h"
#include "mts/CrossThreadNotifier.h"

namespace p = boost::python;



bool PythonStrategy::initialize(dict params) {
	return PythonStrategy::initialize ( "" , params );
}

bool PythonStrategy::initialize(const std::string & paramsJsonFile) {
	return PythonStrategy::initialize ( paramsJsonFile,dict() );
}

bool readJsonFile(const std::string & paramsJsonFile, QVariantMap* vmap){
	if (paramsJsonFile.empty ()){
		return true;
	}
	MTS_LOG ( "Open config file '%s'\n" , paramsJsonFile.c_str () );
	QFile jsonFile ( paramsJsonFile.c_str () );
	if ( !jsonFile.exists () ){
		MTS_ERROR ( "No such config file '%s'\n" , paramsJsonFile.c_str () );
		return false;
	}

	if ( !jsonFile.open ( QFile::ReadOnly | QFile::Text ) ){
		MTS_ERROR ( "Failed to open config file '%s'\n" , paramsJsonFile.c_str () );
		return false;
	}

	QJsonDocument doc = QJsonDocument::fromJson ( jsonFile.readAll () );
	if ( doc.isNull () ){
		MTS_ERROR ( "Bad JSON format file '%s'\n" , paramsJsonFile.c_str () );
		return false;
	}

	(*vmap)=doc.toVariant ().toMap ();
	return true;
}

bool PythonStrategy::initialize ( const std::string & paramsJsonFile , dict params ){
	if ( QCoreApplication::instance () == nullptr ){
		MTS_ERROR ( "You should be new a strategy before calling Mts initialize\n");
		return false;
	}

	QVariantMap paramMap;
	if (!readJsonFile(paramsJsonFile, &paramMap)) {
		return false;
	}
	if ( len ( params ) > 0 ){
		 PythonUtil::dict2vmap ( params , &paramMap);
	}

	if ( paramMap.isEmpty () ){
		return false;
	}
	auto logLevelVar = paramMap.value("log_level");
	if (!logLevelVar.isNull()) {
		bool ok = false;
		int logLevel = logLevelVar.toInt(&ok);
		if (ok) {
			SET_MTS_LOG_LEVEL(logLevel);
		}
	}

	if ( !StrategyMgr::instance ()->initialize ( paramMap )){
		MTS_ERROR ( "Failed to initialize MTS\n" );
		return false;
	}
	return true;
}

int PythonStrategy::exec() {
	return StrategyMgr::instance()->exec();
}

void PythonStrategy::exit(int code) {
	StrategyMgr::instance()->exit(code);
}

static
QCoreApplication* getApplicationInstance() {
	auto app = QCoreApplication::instance();
	if (app == nullptr) {
		int argc = 1;
		char** argv = new char*[argc];
		char * arg = new char[255];
		strcpy(arg, "mts");
		argv[0] = arg;
		app = new QCoreApplication(argc, argv);
	}
	return app;
}


PythonStrategy::PythonStrategy(const std::string& name , int strategyId, const std::string& strategyPath)
	:AbsStrategy(name, strategyId, strategyPath) 
{
	getApplicationInstance();
	MTS_DEBUG("appPath() %s\n", qPrintable(MtsPath::appDirPath()));
}


PythonStrategy::~PythonStrategy() 
{
}

bool PythonStrategy::checkParamsNewOrder ( const dict& params ){
	

	return true;
}

std::string PythonStrategy::newOrder ( dict params ){
	if ( !checkParamsNewOrder ( params ) ){
		return "";
	}

	if (!params.contains("priceType")) {
		MTS_ERROR("Missing param 'priceType'\n");
		return "";
	}
	PriceType priceType = extract<PriceType>(params["priceType"]);
	if (priceType <= PRICE_UNKNOWN || priceType > PRICE_IOC) {
		MTS_ERROR("Unknown price type %d\n", priceType);
		return "";
	}

	double ordPrice = extract<double> ( params["price"] );
	if ( ordPrice <= 0 || ordPrice >= DBL_MAX ){
		return "";
	}
	QString symbol = QString::fromStdString(extract<std::string>(params["symbol"]));
	auto id= InstrumentPropertyDb::instance()->findInstrumentId(symbol);
	if (id.isNull()) {
		MTS_ERROR("No such symbol '%s'\n", qPrintable(symbol));
		return "";
	}
	OrderActionNew* actNewOrder = new OrderActionNew;
	int ot = OT_DIRECT;
	if (params.contains ( "type" )){
		ot = extract<int> ( params["type"] );
		if ( ot <= OT_UNKNOWN  ||  ot> OT_LAST ){
			MTS_ERROR ( "Unknown order type %d\n",ot );
			return "";
		}
	}
	actNewOrder->setInstrumentId(id);
	actNewOrder->setVolume(extract<double>(params["volume"])); //TODO check double
	actNewOrder->setPrice(ordPrice);
	actNewOrder->setPriceType(priceType);
	if ( ot == OT_DIRECT ){
		if ( !params.contains ( "direction" ) ){
			MTS_ERROR ( "Missing param 'direction'\n");
			return "";
		}
		DirectionSide direction = extract<DirectionSide> ( params["direction"] );
		if ( direction == D_UNKNOWN ){
			MTS_ERROR ( "Unknown order direction \n" );
			return "";
		}
		actNewOrder->setDirectonSide ( direction );
		if ( params.contains ("offsetFlag") ){
			CombOffsetFlag flag= extract<CombOffsetFlag> ( params["offsetFlag"] );
			actNewOrder->setOffsetFlag ( flag );
		}
	}
	if (params.contains("qt_rev_us")) {
		qint64 us = extract<qint64>(params["qt_rev_us"]);
		actNewOrder->setPerfNote(QString("qt=%1").arg(us));
	}
	if (params.contains("cpp_qt_rev_us")) {
		qint64 us = extract<qint64>(params["cpp_qt_rev_us"]);
		actNewOrder->setPerfNote(QString("%1|cpp_qt=%2").arg(actNewOrder->perfNote()).arg(us));
	}
	if (params.contains("neword")) {
		qint64 us = extract<qint64>(params["neword"]);
		actNewOrder->setPerfNote(QString("%1|neword=%2").arg(actNewOrder->perfNote()).arg(us));
	}
	return StrategyMgr::instance()->newOrder(_notifier, actNewOrder,ot).toStdString();
}

bool PythonStrategy::cancelOrder(dict params)
{
	OrderActionCancel* actCancelOrder = new OrderActionCancel;
	std::string symbol = extract<std::string>(params["symbol"]);
	std::string referenceId = extract<std::string>(params["referenceId"]);
	actCancelOrder->setReferenceId(referenceId.c_str());
	actCancelOrder->setInstrumentId(mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str()));
	actCancelOrder->setPerfNote(QString("clxord=%1").arg(DateTime::nowToUTCMicrosecsSinceEpoch()));
	return StrategyMgr::instance()->cancelOrder(_notifier, actCancelOrder);
}

bool PythonStrategy::subscribeQuotes(list symbols)
{
	QList<InstrumentId> ids;
	auto  symbolsSize = len(symbols);
	for (int i = 0; i <symbolsSize; ++i) {
		std::string sym= extract<std::string>(symbols[i]);
		auto id=InstrumentPropertyDb::instance()->findInstrumentId(sym.c_str());
		ids << id;
	}
	return StrategyMgr::instance()->subscribeQuotes(_notifier, ids);
}


bool PythonStrategy::unsubscribeQuotes(list symbols)
{
	QList<InstrumentId> ids;
	for (int i = 0; i < len(symbols); ++i) {
		std::string sym = call_method<std::string>(symbols.ptr(), "__getitem__", i);
		ids << mts::InstrumentPropertyDb::instance()->findInstrumentId(sym.c_str());
	}
	return StrategyMgr::instance()->unsubscribeQuotes(_notifier, ids);
}

void PythonStrategy::unsubscribeAllQuotes (){
	StrategyMgr::instance ()->unsubscribeAllQuotes (_notifier );
}



list PythonStrategy::getAllTradingAccounts() const {
	QList<TradingAccount*> accounts = StrategyMgr::instance()->getAllTradingAccounts();
	list l;
	for (int i = 0, size = accounts.size(); i < size; ++i) {
		mts::TradingAccount* account = accounts[i];
		l.append(PyTradingAccount(*account));
	}
	return l;
}

PyPosition PythonStrategy::getPosition(const std::string& symbol) const
{
	mts::InstrumentId id = mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str());
	Position* pos =StrategyMgr::instance()->getPosition(id);
	if (pos) {
		return PyPosition(*pos);
	} else {
		return PyPosition();
	}
}

list PythonStrategy::getAllOrders() const
{
	QList<Order*> orders = StrategyMgr::instance()->getAllOrders(const_cast<mts::CrossThreadNotifier*>(_notifier));
	list l;
	for (int i = 0, size = orders.size(); i < size; ++i) {
		l.append(PyOrder(*orders[i]));
	}
	return l;
}

list PythonStrategy::getActiveOrders(const std::string& symbol) const
{
	mts::InstrumentId id = mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str());
	QList<Order*> orders = StrategyMgr::instance()->getActiveOrders(const_cast<mts::CrossThreadNotifier*>(_notifier), id);
	list l;
	for (auto it = orders.begin(), itEnd = orders.end(); it != itEnd; ++it) {
		l.append(PyOrder(**it));
	}
	return l;
}

list PythonStrategy::getAllOrderFills() const {
	QList<OrderReportFill*> fills = StrategyMgr::instance()->getAllOrderFills();
	list l;
	for (int i = 0, size = fills.size(); i < size; ++i) {
		mts::OrderReportFill* fill = fills[i];
		l.append(PyOrderFill(*fill));
	}
	return l;
}

PyQuote PythonStrategy::getQuote(const std::string& symbol) const
{
	mts::InstrumentId id = mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str());
	Quote* quote = StrategyMgr::instance()->getQuote(id);
	if (quote) {
		return PyQuote(*quote);
	} else {
		return PyQuote();
	}
}

list PythonStrategy::getAllPositions() const
{
	QList<Position*> posList = StrategyMgr::instance()->getAllPositions();
	list l;
	for (int i = 0, size = posList.size(); i < size; ++i) {
		l.append(PyPosition(*posList[i]));
	}
	return l;
}

list PythonStrategy::getAllInstrumentSymbols() const {
	QList<InstrumentId> ids=StrategyMgr::instance()->instrumentIds();
	list l;
	for (int i = 0, size = ids.size(); i < size; ++i) {
		const mts::InstrumentId& id = ids[i];
		l.append(id.symbol.toStdString());
	}
	return l;
}


PyDateTime PythonStrategy::getNow() const {
	return PyDateTime(StrategyMgr::instance()->getNow());
}

qint64 PythonStrategy::getNowMicrosecs() const
{
	return DateTime::nowToUTCMicrosecsSinceEpoch();
}


void PythonStrategy::doEnvirInitialized() {
	this->onEnvirInitialized();
}

void PythonStrategy::doInitialized(AccountPtr account) {
	MTS_DEBUG ( "PythonStrategy::doInitialized\n" );
	_notifier->connectSignals ( this );
	this->onInitialized(PyAccount(*account,this->getAllOrders(),this->getAllPositions()));
	_notifier->onBusinessDateChanged ( StrategyMgr::instance ()->currentTradingDay () );
	if (mts::ConfigParams::instance()->mode() == mts::ENVIR_SIMU && StrategyMgr::instance()->subscribedInstrumentCount() == 0) {
		MTS_ERROR("please subscribe instrument\n");
		QCoreApplication::instance()->exit(1);
	}
}

void PythonStrategy::doTimeout(int timerId) {
	this->onTimer ( timerId );
}

void PythonStrategy::doQuoteSnapshotUpdate(QuoteSnapshotPtr quote) {
	this->onQuoteSnapshotUpdate(PyQuote(*quote.data()));
}

void PythonStrategy::doOrderNewDone(OrderReportNewDonePtr report) {
	this->onOrderNewDone(PyOrderNewDone(*report.data()));
}

void PythonStrategy::doOrderNewReject(OrderReportNewRejectPtr report) {
	this->onOrderNewReject( PyOrderNewReject ( *report.data () ) );
}

void PythonStrategy::doOrderFill(OrderReportFillPtr report) {
	this->onOrderFill( PyOrderFill(*report.data()));
}

void PythonStrategy::doOrderCancelDone(OrderReportCancelDonePtr report) {
	this->onOrderCancelDone( PyOrderCancelDone(*report.data()));
}

void PythonStrategy::doOrderCancelReject(OrderReportCancelRejectPtr report) {
	this->onOrderCancelReject ( PyOrderCancelReject ( *report.data ()));
}

void PythonStrategy::doOrderOtherReport(OrderReportPtr report) {
	this->onOrderOtherReport( PyOrderReport(*report.data()));
}

void PythonStrategy::doPositionUpdate(PositionPtr pos) {
	this->onPositionUpdate(PyPosition(*pos.data()));
}

void PythonStrategy::doOrderUpdate(OrderPtr order) {
	this->onOrderUpdate(PyOrder(*order.data()));
}


void PythonStrategy::doBusinessDateChanged(int date) {
	this->onBusinessDateChanged(date);
}

void PythonStrategy::doBarUpdate(CalcBarPtr bar) {
	this->onBarUpdate(PyBar(*bar.data()));
}

