
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
#include "PythonStrategy.h"
#include <iostream>
#include "mts/StrategyMgr.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include "base/MtsLog.h"
#include "base/MtsUtils.h"
#include "mts_core/InstrumentPropertyDb.h"

#include "mts_core/InstrumentPropertyDb.h"

#include "mts_core/OrderType.h"
#include "base/MtsPath.h"
#include "mts_core/ConfigParams.h"
#include "PyTradingAccount.h"
#include "PythonUtil.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Exception.h>
#include <Poco/StreamCopier.h>
#include<Poco/Timespan.h>

namespace p = boost::python;
using namespace Poco::Net;

bool remoteReportAndCheck() {
	//report the runing when on real mode
	assert(ConfigParams::isInitialized());
	if (ConfigParams::instance()->mode() != mts::EnvironmentMode::ENVIR_REAL) {
		return true;
	}

	try
	{
		HTTPClientSession session("www.mtsquant.com", 3006);
		//HTTPClientSession session("127.0.0.1", 3006);
		session.setTimeout(Poco::Timespan(5,0));
		HTTPRequest request(HTTPRequest::HTTP_POST, "/mts/check", HTTPRequest::HTTP_1_1);
		HTMLForm form;
		form.add(MTS_VERSION, ConfigParams::instance()->value(MTS_VERSION).toString().toStdString());
		form.add(MTS_OS, ConfigParams::instance()->value(MTS_OS).toString().toStdString());
		form.add(MODE_PARAM_NAME, ConfigParams::instance()->value(MODE_PARAM_NAME).toString().toStdString());
		form.add(INSTANCE_ID, ConfigParams::instance()->value(INSTANCE_ID).toString().toStdString());
		QUrl feedsFront(ConfigParams::instance()->value(FEEDS_FRONT).toString());
		feedsFront.setPassword("");
		form.add(FEEDS_FRONT, feedsFront.toString().toStdString());
		QUrl tradeFront(ConfigParams::instance()->value(TRADE_FRONT).toString());
		tradeFront.setPassword("");
		form.add(TRADE_FRONT, tradeFront.toString().toStdString());
		QString broker = ConfigParams::instance()->value(BROKER).toString();
		if (broker.isEmpty()) {
			broker = ConfigParams::instance()->value(FEEDS_BROKER).toString();
		}
		form.add(BROKER, broker.toStdString());
		form.prepareSubmit(request);
		form.write(session.sendRequest(request));
		HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);
		//Poco::StreamCopier::copyStream(rs, std::cout);
		char* recvBuffer = new char[1024];
		memset(recvBuffer, 0, sizeof(char)*1024);
		rs.read(recvBuffer,1024);
		//printf("recv:'%s'\n", recvBuffer);
		QJsonObject retJson = MtsUtils::str2Json(recvBuffer);
		int ok = retJson.value("ok").toInt(1);
		delete []recvBuffer;
		if (!ok) {
			return false;
		}
	}
	catch (Poco::Exception & ex)
	{
		//std::cout << "Exception:"<<ex.displayText();
	}
	return true;
}


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
	//setLogLevel if exists
	auto logLevelVar = paramMap.value("log_level");
	if (!logLevelVar.isNull()) {
		bool ok = false;
		int logLevel = logLevelVar.toInt(&ok);
		if (ok) {
			SET_MTS_LOG_LEVEL(logLevel);
		}
	}

	if ( !StrategyMgr::instance ()->initialize ( paramMap ) || !remoteReportAndCheck()){
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


PythonStrategy::PythonStrategy(const std::string& name , int strategyId, const std::string& strategyPath)
	:QObject() , _notifier(name,strategyId,strategyPath)
{
	_notifier.connectSignalInitializedl(this);
	int argc = 1;
	char** argv = new char*[argc];
	char * arg = new char[255];
	strcpy(arg, "mts");
	argv[0] = arg;
	_app = QCoreApplication::instance();
	if (_app == nullptr) {
		_app = new QCoreApplication(argc, argv);
	}
	StrategyMgr::instance()->registerStrategy(&_notifier);

	MTS_DEBUG("appPath() %s\n", qPrintable(MtsPath::appDirPath()));
	//MTS_DEBUG("scriptPath() %s\n", qPrintable(MtsPath::scriptDirPath()));

}


PythonStrategy::~PythonStrategy() 
{
	StrategyMgr::instance()->unregisterStrategy(&_notifier);
}


int PythonStrategy::newTimer(int beginTime, int interval)
{
	return StrategyMgr::instance()->newTimer(&_notifier,beginTime, interval);
}

int PythonStrategy::newOnceTimer(int beginTime) {
	return StrategyMgr::instance()->newOnceTimer(&_notifier, beginTime);
}

bool PythonStrategy::removeTimer(int id)
{
	return StrategyMgr::instance()->removeTimer(&_notifier, id);
}

bool PythonStrategy::checkParamsNewOrder ( const dict& params ){
	

	return true;
}

std::string PythonStrategy::newOrder ( dict params ){
	//printDict ( params );
	if ( !checkParamsNewOrder ( params ) ){
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
	//actNewOrder->setOrderType ( ot );
	actNewOrder->setInstrumentId(id);
	actNewOrder->setVolume(extract<double>(params["volume"])); //TODO check double
	actNewOrder->setPrice(ordPrice);
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
	return StrategyMgr::instance()->newOrder(&_notifier, actNewOrder,ot).toString().toStdString();
}

bool PythonStrategy::cancelOrder(dict params)
{
	OrderActionCancel* actCancelOrder = new OrderActionCancel;
	std::string symbol = extract<std::string>(params["symbol"]);
	std::string referenceId = extract<std::string>(params["referenceId"]);
	actCancelOrder->setReferenceId(mts::OrderId(referenceId.c_str()));
	actCancelOrder->setInstrumentId(mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str()));
	actCancelOrder->setPerfNote(QString("clxord=%1").arg(DateTime::nowToUTCMicrosecsSinceEpoch()));
	return StrategyMgr::instance()->cancelOrder(&_notifier, actCancelOrder);
}

bool PythonStrategy::subscribeQuotes(list symbols)
{
	QList<InstrumentId> ids;
	auto  symbolsSize = len(symbols);
	for (int i = 0; i <symbolsSize; ++i) {
		std::string sym= extract<std::string>(symbols[i]);
		//std::string sym = call_method<std::string>(symbols.ptr(), "__getitem__", i);
		auto id=InstrumentPropertyDb::instance()->findInstrumentId(sym.c_str());
		ids << id;
	}
	return StrategyMgr::instance()->subscribeQuotes(&_notifier, ids);
}


bool PythonStrategy::unsubscribeQuotes(list symbols)
{
	QList<InstrumentId> ids;
	for (int i = 0; i < len(symbols); ++i) {
		std::string sym = call_method<std::string>(symbols.ptr(), "__getitem__", i);
		ids << mts::InstrumentPropertyDb::instance()->findInstrumentId(sym.c_str());
	}
	return StrategyMgr::instance()->unsubscribeQuotes(&_notifier, ids);
}

void PythonStrategy::unsubscribeAllQuotes (){
	StrategyMgr::instance ()->unsubscribeAllQuotes ( &_notifier );
}



list PythonStrategy::getAllTradingCounts() const {
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
	QList<Order*> orders = StrategyMgr::instance()->getAllOrders(const_cast<mts::CrossThreadNotifier*>(&_notifier));
	list l;
	for (int i = 0, size = orders.size(); i < size; ++i) {
		l.append(PyOrder(*orders[i]));
	}
	return l;
}

list PythonStrategy::getActiveOrders(const std::string& symbol) const
{
	mts::InstrumentId id = mts::InstrumentPropertyDb::instance()->findInstrumentId(symbol.c_str());
	QList<Order*> orders = StrategyMgr::instance()->getActiveOrders(const_cast<mts::CrossThreadNotifier*>(&_notifier), id);
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

//list PythonStrategy::getAllInstrumentMtsSymbols() const {
//	auto ids = StrategyMgr::instance()->instrumentIds();
//	list l;
//	foreach(const mts::InstrumentId& id, ids) {
//		l.append(id.symbol);
//	}
//	return l;
//}

PyDateTime PythonStrategy::getNow() const {
	return PyDateTime(StrategyMgr::instance()->getNow());
}

qint64 PythonStrategy::getNowMicrosecs() const
{
	return DateTime::nowToUTCMicrosecsSinceEpoch();
}

//bool PythonStrategy::loadBars(const std::string & jsonFilePath) {
//	return  StrategyMgr::instance()->loadBars(jsonFilePath.c_str());
//}

void PythonStrategy::doEnvirInitialized() {
	onEnvirInitialized();
}

void PythonStrategy::doInitialized(AccountPtr account) {
	MTS_DEBUG ( "PythonStrategy::doInitialized\n" );
	_notifier.connectSignals ( this );
	onInitialized(PyAccount(*account,this->getAllOrders(),this->getAllPositions()));
	_notifier.onBusinessDateChanged ( StrategyMgr::instance ()->currentTradingDay () );
	//Exit if no subscribed symbol in simu mode 
	if (mts::ConfigParams::instance()->mode() == mts::ENVIR_SIMU && StrategyMgr::instance()->subscribedInstrumentCount() == 0) {
		MTS_ERROR("please subscribe instrument\n");
		QCoreApplication::instance()->exit(1);
	}
}

void PythonStrategy::doTimeout(int timerId) {
	this->onTimer ( timerId );
}

void PythonStrategy::doOrderNewDone(OrderReportNewDonePtr report) {
	onOrderNewDone(PyOrderNewDone(*report.data()));
}

void PythonStrategy::doOrderNewReject(OrderReportNewRejectPtr report) {
	onOrderNewReject( PyOrderNewReject ( *report.data () ) );
}

void PythonStrategy::doOrderFill(OrderReportFillPtr report) {
	onOrderFill( PyOrderFill(*report.data()));
}

void PythonStrategy::doOrderCancelDone(OrderReportCancelDonePtr report) {
	onOrderCancelDone( PyOrderCancelDone(*report.data()));
}

void PythonStrategy::doOrderCancelReject(OrderReportCancelRejectPtr report) {
	onOrderCancelReject ( PyOrderCancelReject ( *report.data ()));
}

void PythonStrategy::doOrderOtherReport(OrderReportPtr report) {
	onOrderOtherReport( PyOrderReport(*report.data()));
}

void PythonStrategy::doQuoteSnapshotUpdate(QuoteSnapshotPtr quote) {
	onQuoteSnapshotUpdate(PyQuote(*quote.data()));
}

void PythonStrategy::doPositionUpdate(PositionPtr pos) {
	onPositionUpdate(PyPosition(*pos.data()));
}

void PythonStrategy::doOrderUpdate(OrderPtr order) {
	onOrderUpdate(PyOrder(*order.data()));
}


void PythonStrategy::doBusinessDateChanged(int date) {
	this->onBusinessDateChanged(date);
}

void PythonStrategy::doBarUpdate(CalcBarPtr bar) {
	onBarUpdate(PyBar(*bar.data()));
}

