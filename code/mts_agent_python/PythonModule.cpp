
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
锘�#include "PythonStrategyWrap.h"
#include "base/DateTime.h"
#include "base/MtsLog.h"
#include "PyAccount.h"
#include "PyDateTime.h"
#include "PyInstrumentProperty.h"
#include "PyQuote.h"
#include "PyOrderNewDone.h"
#include "mts_core/enums.h"
#include "mts_core/ContinuousSymbolMgr.h"
#include "mts_core/ConfigParams.h"

#include "mts_core/OrderType.h"

#include <boost/format.hpp>
#include "base/MtsPath.h"
#include "PyTradingAccount.h"
#include "mts_core/TradingDateMgr.h"
#include "PythonUtil.h"

using namespace mts;

std::ostream & operator<<( std::ostream &  os , const PyDateTime& dt ){
	os << dt.toString ();
	return os;
}

std::string getContinuousContract ( const std::string& productId , int date ){
	return ContinuousSymbolMgr::instance ()->getSymbol ( productId , date );
}



PyInstrumentProperty findInstrument(const std::string& sym) {
	QString symbol = QString::fromStdString(sym);
	auto id = InstrumentPropertyDb::instance()->findInstrumentId(symbol);
	if (id.isNull()) {
		return PyInstrumentProperty();
	}

	auto * p = InstrumentPropertyDb::instance()->find(id);
	mts::InstrumentProperty* pf = dynamic_cast<mts::InstrumentProperty*>(p);
	if (pf == nullptr) {
		return PyInstrumentProperty();
	}
	return PyInstrumentProperty(*pf);
}

std::string genMtsLogString(const boost::python::tuple& tp) {
	std::string format= extract<std::string>(tp[0]);
	boost::format formater(format);
	int sz = boost::python::len(tp) ;
	for (int i = 1; i < sz; ++i) {
		{
			extract<int> e(tp[i]);
			if (e.check()) {
				formater % e;
				continue;
			}
		}
		{
			extract<double> e(tp[i]);
			if (e.check()) {
				formater % e;
				continue;
			}
		}
		{
			extract<std::string> e(tp[i]);
			if (e.check()) {
				formater % std::string(e).c_str();
				continue;
			}
		}

	}
	return formater.str();
}

void mtsLog(const boost::python::tuple& tp) {
	std::string logStr = genMtsLogString(tp);
	MTS_LOG("%s", logStr.c_str());
}


void mtsWarn(const boost::python::tuple& tp) {
	std::string logStr = genMtsLogString(tp);
	MTS_WARN("%s", logStr.c_str());
}


void mtsError(const boost::python::tuple& tp) {
	std::string logStr = genMtsLogString(tp);
	MTS_ERROR("%s", logStr.c_str());
}

void mtsFile(const boost::python::tuple& tp) {
	std::string logStr = genMtsLogString(tp);
	MTS_PERF_FILE("%s\n", logStr.c_str());
}


int nextTradingDate(int date) {
	return mts::TradingDateMgr::instance()->nextDate(date);
}

int prevTradingDate(int date) {
	return mts::TradingDateMgr::instance()->prevDate(date);
}

int forceToTradingDate(int date) {
	return mts::TradingDateMgr::instance()->forceToDate(date);
}

bool isTradingDate(int date) {
	return mts::TradingDateMgr::instance()->isTradingDate(date);
}

int nextNTradingDate(int date,int n) {
	return mts::TradingDateMgr::instance()->nextNDate(date,n);
}


dict getConfigParams() {
	dict paramsDict;
	QStringList keys = mts::ConfigParams::instance()->keys();
	for (int i = 0, size = keys.size(); i < size; ++i) {
		const QString& key = keys[i];
		paramsDict.setdefault(key.toStdString(), mts::ConfigParams::instance()->value(key).toString().toStdString());
	}
	return paramsDict;
}

std::string getConfigParam(const std::string& key, const std::string& defaultValue) {
	auto v=mts::ConfigParams::instance()->value(QString::fromStdString(key), defaultValue.empty() ? QVariant() : QVariant(QString::fromStdString(defaultValue)));
	if (v.isNull()) {
		return "";
	} else {
		return v.toString().toStdString();
	}
}

inline
QStringList getScriptArgv() {
	int scriptArgc = extract<int>(eval("len(sys.argv)"));
	QStringList argv;
	for (int i = 0; i < scriptArgc; ++i) {
		const QString argvExp = QString("sys.argv[%1]").arg(i);
		const std::string arg = extract<std::string>(eval(qPrintable(argvExp)));
		argv.append(QString::fromStdString(arg));
	}
	return argv;
}


void initializePythonModule() {
	PyEval_InitThreads();	//瀵煎叆鏃惰繍琛岋紝淇濊瘉鍏堝垱寤篏IL
#ifdef _WIN32
	np::initialize();
#endif

	object main_module = import("__main__");
	object main_namespace = main_module.attr("__dict__");
	exec("import os", main_namespace, main_namespace);
	exec("import sys", main_namespace, main_namespace);

	object scriptPathObject = eval("sys.path[0] or os.getcwd()", main_namespace, main_namespace);
	std::string  scriptPath = extract<std::string>(scriptPathObject);
	MtsPath::setScriptDirPath(scriptPath.c_str());

	QStringList argv = getScriptArgv();


	typedef bool(*InitializeByObj) (dict);
	typedef bool(*InitializeByFile) (const std::string&);
	typedef bool(*InitializeByFileAndObj) (const std::string&, dict);

	def("mtsLog", &mtsLog);
	def("mtsInfo", &mtsLog);
	def("mtsWarn", &mtsWarn);
	def("mtsError", &mtsError);
	def("mtsFile", &mtsFile);

	def("nextTradingDate", &nextTradingDate);
	def("prevTradingDate", &prevTradingDate);
	def("forceToTradingDate", &forceToTradingDate);
	def("isTradingDate", &isTradingDate);
	def("nextNTradingDate", &nextNTradingDate);

	def("mtsInitialize", (InitializeByObj)(&PythonStrategy::initialize));
	def("mtsInitialize", (InitializeByFile)(&PythonStrategy::initialize));
	def("mtsInitialize", (InitializeByFileAndObj)(&PythonStrategy::initialize));

	def("mtsExecute", &PythonStrategy::exec);
	def("mtsExit", &PythonStrategy::exit);
	def("ticksFromMidnight", &PyDateTime::time2LocalTicksFromMidnight);
	def("getContinuousContract", &getContinuousContract);

	def("findInstrument", &findInstrument);

	def("params", &getConfigParams);
	def("param", &getConfigParam);


	enum_<TimezoneCode>("Timezone")
		.value("UNKNOWN", TIMEZONE_UNKNOWN)
		.value("CST", TIMEZONE_CST)
		.value("EST", TIMEZONE_EST)
		.value("UTC", TIMEZONE_UTC)
		;

	PythonUtil::addObject("OT_UNKNOWN", OT_UNKNOWN);
	PythonUtil::addObject("OT_DIRECT", OT_DIRECT);
	PythonUtil::addObject("OT_NET", OT_NET);
	PythonUtil::addObject("OT_BOX", OT_BOX);

	;

	enum_<DirectionSide>("Direction")
		.value(directionSideName(D_UNKNOWN), D_UNKNOWN)
		.value(directionSideName(D_BUY), D_BUY)
		.value(directionSideName(D_SELL), D_SELL)
		.value(directionSideName(D_SHORT), D_SHORT)
		.value(directionSideName(D_COVER), D_COVER)
		;

	enum_<CombOffsetFlag>("OpenCloseFlag")
		.value(combOffsetFlagName(OF_UNKNOWN), OF_UNKNOWN)
		.value(combOffsetFlagName(OF_OPEN), OF_OPEN)
		.value(combOffsetFlagName(OF_CLOSE), OF_CLOSE)
		.value(combOffsetFlagName(OF_CLOSETODAY), OF_CLOSETODAY)
		.value(combOffsetFlagName(OF_CLOSEYESTERDAY), OF_CLOSEYESTERDAY)
		;


	class_<PyDateTime>("DateTime", init<qint64>())
		.def(init<int, int>())
		.add_property("date", &PyDateTime::date)
		.add_property("time", &PyDateTime::time)
		.add_property("ticksSinceMidnight", &PyDateTime::ticksSinceMidnight)
		.add_property("year", &PyDateTime::year)
		.add_property("month", &PyDateTime::month)
		.add_property("day", &PyDateTime::day)
		.add_property("hour", &PyDateTime::hour)
		.add_property("minute", &PyDateTime::minute)
		.add_property("second", &PyDateTime::second)
		.add_property("millisecond", &PyDateTime::millisecond)
		.def("getDate", &PyDateTime::date)
		.def("getTime", &PyDateTime::time)
		.def("setDate", &PyDateTime::setDate)
		.def("setTime", &PyDateTime::setTime)
		.def("getTicksSinceMidnight", &PyDateTime::ticksSinceMidnight)
		.def("getYear", &PyDateTime::year)
		.def("getMonth", &PyDateTime::month)
		.def("getDay", &PyDateTime::day)
		.def("getHour", &PyDateTime::hour)
		.def("getMinute", &PyDateTime::minute)
		.def("getSecond", &PyDateTime::second)
		.def("getMillisecond", &PyDateTime::millisecond)
		.def("toUTCMillisecsSinceEpoch", &PyDateTime::toUTCMillisecsSinceEpoch)
		.def("isNull", &PyDateTime::isNull)
		.def("isUTC", &PyDateTime::isUTC)
		.def("toUTC", &PyDateTime::toUTC)
		.def("toString", &PyDateTime::toString)
		.def(self + qint64())          // __add__
		.def(self - self)           // __sub__
		.def(self - qint64())          // __sub__
		.def(self < self)          // __lt__
		.def(self > self)          // __lg__
		.def(self >= self)
		.def(self <= self)
		.def(self != self)
		.def(self == self)
		.def("__str__", &PyDateTime::toString)
		.def("__repr__", &PyDateTime::toString)
		;

	class_<PyInstrumentProperty /*, bases<InstrumentBaseProperty>*/ >("InstrumentProperty", no_init)
		.add_property("name", &PyInstrumentProperty::pyName)
		.add_property("symbol", &PyInstrumentProperty::pySymbol)
		.add_property("mtsSymbol", &PyInstrumentProperty::pyMtsSymbol)
		.add_property("volumeMultiple", &PyInstrumentProperty::volumeMultiple/*, return_value_policy <copy_const_reference>()*/)
		.add_property("priceTick", &PyInstrumentProperty::priceTick)
		.add_property("minOrderSize", &PyInstrumentProperty::minOrderSize)
		.add_property("orderSizeIncrement", &PyInstrumentProperty::orderSizeIncrement)
		.add_property("createdDate", &PyInstrumentProperty::createDate)
		.add_property("openDate", &PyInstrumentProperty::openDate)
		.add_property("expireDate", &PyInstrumentProperty::expireDate)
		.add_property("startDelivDate", &PyInstrumentProperty::startDelivDate)
		.add_property("endDelivDate", &PyInstrumentProperty::endDelivDate)
		.def("getName", &PyInstrumentProperty::pyName)
		.def("getSymbol", &PyInstrumentProperty::pySymbol)
		.def("getMtsSymbol", &PyInstrumentProperty::pyMtsSymbol)
		.def("getVolumeMultiple", &PyInstrumentProperty::volumeMultiple/*, return_value_policy <copy_const_reference>()*/)
		.def("getPriceTick", &PyInstrumentProperty::priceTick)
		.def("getMinOrderSize", &PyInstrumentProperty::minOrderSize)
		.def("getOrderSizeIncrement", &PyInstrumentProperty::orderSizeIncrement)
		.def("getCreatedDate", &PyInstrumentProperty::createDate)
		.def("getOpenDate", &PyInstrumentProperty::openDate)
		.def("getExpireDate", &PyInstrumentProperty::expireDate)
		.def("getStartDelivDate", &PyInstrumentProperty::startDelivDate)
		.def("getEndDelivDate", &PyInstrumentProperty::endDelivDate)
		.def("isNull", &PyInstrumentProperty::isNull)
		.def("toString", &PyInstrumentProperty::toString)
		.def("__str__", &PyInstrumentProperty::toString)
		;

	class_<PyAccount>("Account", no_init)
		.def("find", &PyAccount::findInstrumentProperty /*,return_value_policy<manage_new_object>()*/)
		.add_property("activeOrders", &PyAccount::allActiveOrders)
		.add_property("positions", &PyAccount::allPositions)
		.def("getActiveOrders", &PyAccount::allActiveOrders)
		.def("getPositions", &PyAccount::allPositions)
		;


	class_<PyQuote>("Quote", no_init)
		.add_property("symbol", &PyQuote::pySymbol)
		.add_property("mtsSymbol", &PyQuote::pyMtsSymbol)
		.add_property("exchId", &PyQuote::pyExchId)
		.add_property("dateTime", &PyQuote::pyDateTime)
		.add_property("tradingDay", &PyQuote::tradingDay)
		.add_property("ticksSinceEpoch", &PyQuote::ticksSinceEpoch)
		.add_property("preClosePrice", &PyQuote::preClosePrice)
		.add_property("openPrice", &PyQuote::openPrice)
		.add_property("highPrice", &PyQuote::highPrice)
		.add_property("lowPrice", &PyQuote::lowPrice)
		.add_property("closePrice", &PyQuote::closePrice)
		.add_property("lastPrice", &PyQuote::lastPrice)
		.add_property("lastVolume", &PyQuote::lastVolume)
		.add_property("totalVolume", &PyQuote::totalVolume)
		.add_property("bidPrice", &PyQuote::bidPrice)
		.add_property("askPrice", &PyQuote::askPrice)
		.add_property("bidVolume", &PyQuote::bidVolume)
		.add_property("askVolume", &PyQuote::askVolume)
		.add_property("upperLimitPrice", &PyQuote::upperLimitPrice)
		.add_property("lowerLimitPrice", &PyQuote::lowerLimitPrice)
		.add_property("preSettlementPrice", &PyQuote::preSettlementPrice)
		.add_property("settlementPrice", &PyQuote::settlementPrice)
		.add_property("turnover", &PyQuote::turnover)
		.add_property("openInterest", &PyQuote::openInterest)
		.add_property("isTradeUpdate", &PyQuote::isTradeChg)
		.add_property("isQuoteUpdate", &PyQuote::isQuoteChg)
		.add_property("isBidUpdate", &PyQuote::isBidChg)
		.add_property("isAskUpdate", &PyQuote::isAskChg)
		.def("isNull", &PyQuote::isNull)
		.def("toString", &PyQuote::toString)
		.def("__str__", &PyQuote::toString)
		.add_property("fairPrice", &PyQuote::fairPrice)
		.add_property("revMicrosecond", &PyQuote::revMicrosecond)
		.add_property("cppRevMicrosecond", &PyQuote::cppRevMicrosecond)
		;

	class_<PyBar>("Bar", no_init)
		.add_property("symbol", &PyBar::pySymbol)
		.add_property("mtsSymbol", &PyBar::pyMtsSymbol)
		.add_property("exchId", &PyBar::pyExchId)
		.add_property("beginTicksSinceEpoch", &PyBar::beginTicksSinceEpoch)
		.add_property("endTicksSinceEpoch", &PyBar::endTicksSinceEpoch)
		.add_property("beginDateTime", &PyBar::pyBeginDateTime)
		.add_property("endDateTime", &PyBar::pyEndDateTime)
		.add_property("interval", &PyBar::interval)
		.add_property("openPrice", &PyBar::openPrice)
		.add_property("highPrice", &PyBar::highPrice)
		.add_property("lowPrice", &PyBar::lowPrice)
		.add_property("closePrice", &PyBar::closePrice)
		.add_property("volume", &PyBar::volume)
		.add_property("vwap", &PyBar::vwap)
		.add_property("openInterest", &PyBar::openInterest)
		.add_property("fairPrice", &PyBar::fairPrice)
		.add_property("midPrice", &PyBar::midPrice)
		.add_property("spread", &PyBar::spread)
		.add_property("quoteSize", &PyBar::bidAskDepth)
		.add_property("bidAskDepth", &PyBar::bidAskDepth)
		.def("toString", &PyBar::toString)
		.def("__str__", &PyBar::toString)
		;

	class_<PyOrderNewDone>("OrderNewDone", no_init)
		.add_property("symbol", &PyOrderNewDone::pySymbol)
		.add_property("mtsSymbol", &PyOrderNewDone::pyMtsSymbol)
		.add_property("exchId", &PyOrderNewDone::pyExchId)
		.add_property("createdDateTime", &PyOrderNewDone::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrderNewDone::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrderNewDone::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderNewDone::tradingDay)
		.add_property("note", &PyOrderNewDone::pyNote)
		.add_property("volume", &PyOrderNewDone::volume)
		.add_property("fillVolume", &PyOrderNewDone::fillVolume)
		.add_property("leftVolume", &PyOrderNewDone::leftVolume)
		.add_property("price", &PyOrderNewDone::price)
		.add_property("fillAmount", &PyOrderNewDone::fillAmount)
		.add_property("priceType", &PyOrderNewDone::priceType)
		.add_property("directionSide", &PyOrderNewDone::directionSide)
		.add_property("offsetFlag", &PyOrderNewDone::offsetFlag)
		.add_property("orderType", &PyOrderNewDone::orderType)
		.add_property("referenceId", &PyOrderNewDone::referenceId2String)
		.add_property("createSrc", &PyOrderNewDone::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.add_property("perfNote", &PyOrderNewDone::pyPerfNote)
		.def("toString", &PyOrderNewDone::toString)
		.def("__str__", &PyOrderNewDone::toString)
		;

	class_<PyOrderNewReject>("OrderNewReject", no_init)
		.add_property("symbol", &PyOrderNewReject::pySymbol)
		.add_property("mtsSymbol", &PyOrderNewReject::pyMtsSymbol)
		.add_property("exchId", &PyOrderNewReject::pyExchId)
		.add_property("createdDateTime", &PyOrderNewReject::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrderNewReject::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrderNewReject::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderNewReject::tradingDay)
		.add_property("note", &PyOrderNewReject::pyNote)
		.add_property("volume", &PyOrderNewReject::volume)
		.add_property("fillVolume", &PyOrderNewReject::fillVolume)
		.add_property("leftVolume", &PyOrderNewReject::leftVolume)
		.add_property("price", &PyOrderNewReject::price)
		.add_property("fillAmount", &PyOrderNewReject::fillAmount)
		.add_property("priceType", &PyOrderNewReject::priceType)
		.add_property("directionSide", &PyOrderNewReject::directionSide)
		.add_property("offsetFlag", &PyOrderNewReject::offsetFlag)
		.add_property("orderType", &PyOrderNewReject::orderType)
		.add_property("referenceId", &PyOrderNewReject::referenceId2String)
		.add_property("createSrc", &PyOrderNewReject::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.def("toString", &PyOrderNewReject::toString)
		.def("__str__", &PyOrderNewReject::toString)
		;

	class_<PyOrderCancelDone>("OrderCancelDone", no_init)
		.add_property("symbol", &PyOrderCancelDone::pySymbol)
		.add_property("mtsSymbol", &PyOrderCancelDone::pyMtsSymbol)
		.add_property("exchId", &PyOrderCancelDone::pyExchId)
		.add_property("createdDateTime", &PyOrderCancelDone::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrderCancelDone::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrderCancelDone::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderCancelDone::tradingDay)
		.add_property("note", &PyOrderCancelDone::pyNote)
		.add_property("volume", &PyOrderCancelDone::volume)
		.add_property("fillVolume", &PyOrderCancelDone::fillVolume)
		.add_property("leftVolume", &PyOrderCancelDone::leftVolume)
		.add_property("price", &PyOrderCancelDone::price)
		.add_property("fillAmount", &PyOrderCancelDone::fillAmount)
		.add_property("priceType", &PyOrderCancelDone::priceType)
		.add_property("directionSide", &PyOrderCancelDone::directionSide)
		.add_property("offsetFlag", &PyOrderCancelDone::offsetFlag)
		.add_property("orderType", &PyOrderCancelDone::orderType)
		.add_property("referenceId", &PyOrderCancelDone::referenceId2String)
		.add_property("createSrc", &PyOrderCancelDone::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.def("toString", &PyOrderCancelDone::toString)
		.def("__str__", &PyOrderCancelDone::toString)
		;

	class_<PyOrderCancelReject>("OrderCancelReject", no_init)
		.add_property("symbol", &PyOrderCancelReject::pySymbol)
		.add_property("mtsSymbol", &PyOrderCancelReject::pyMtsSymbol)
		.add_property("exchId", &PyOrderCancelReject::pyExchId)
		.add_property("createdDateTime", &PyOrderCancelReject::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrderCancelReject::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrderCancelReject::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderCancelReject::tradingDay)
		.add_property("note", &PyOrderCancelReject::pyNote)
		.add_property("volume", &PyOrderCancelReject::volume)
		.add_property("fillVolume", &PyOrderCancelReject::fillVolume)
		.add_property("leftVolume", &PyOrderCancelReject::leftVolume)
		.add_property("price", &PyOrderCancelReject::price)
		.add_property("fillAmount", &PyOrderCancelReject::fillAmount)
		.add_property("priceType", &PyOrderCancelReject::priceType)
		.add_property("directionSide", &PyOrderCancelReject::directionSide)
		.add_property("offsetFlag", &PyOrderCancelReject::offsetFlag)
		.add_property("orderType", &PyOrderCancelReject::orderType)
		.add_property("referenceId", &PyOrderCancelReject::referenceId2String)
		.add_property("createSrc", &PyOrderCancelReject::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.def("toString", &PyOrderCancelReject::toString)
		.def("__str__", &PyOrderCancelReject::toString)
		;

	class_<PyOrderReport>("OrderReport", no_init)
		.add_property("symbol", &PyOrderReport::pySymbol)
		.add_property("mtsSymbol", &PyOrderReport::pyMtsSymbol)
		.add_property("exchId", &PyOrderReport::pyExchId)
		.add_property("createdDateTime", &PyOrderReport::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrderReport::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrderReport::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderReport::tradingDay)
		.add_property("note", &PyOrderReport::pyNote)
		.add_property("volume", &PyOrderReport::volume)
		.add_property("fillVolume", &PyOrderReport::fillVolume)
		.add_property("leftVolume", &PyOrderReport::leftVolume)
		.add_property("price", &PyOrderReport::price)
		.add_property("fillAmount", &PyOrderReport::fillAmount)
		.add_property("priceType", &PyOrderReport::priceType)
		.add_property("directionSide", &PyOrderReport::directionSide)
		.add_property("offsetFlag", &PyOrderReport::offsetFlag)
		.add_property("orderType", &PyOrderReport::orderType)
		.add_property("referenceId", &PyOrderReport::referenceId2String)
		.add_property("createSrc", &PyOrderReport::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.def("toString", &PyOrderReport::toString)
		.def("__str__", &PyOrderReport::toString)
		;

	class_<PyOrderFill>("OrderFill", no_init)
		.add_property("symbol", &PyOrderFill::pySymbol)
		.add_property("mtsSymbol", &PyOrderFill::pyMtsSymbol)
		.add_property("exchId", &PyOrderFill::pyExchId)
		.add_property("referenceId", &PyOrderFill::referenceId2String)
		.add_property("orderExchId", &PyOrderFill::pyOrderExchId)
		.add_property("createdDateTime", &PyOrderFill::pyCreatedDateTime)
		.add_property("createTicksSinceEpoch", &PyOrderFill::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrderFill::tradingDay)
		.add_property("note", &PyOrderFill::pyNote)
		.add_property("fillVolume", &PyOrderFill::fillVolume)
		.add_property("fillPrice", &PyOrderFill::fillPrice)
		.add_property("fillAmount", &PyOrderFill::fillAmount)
		.add_property("fillId", &PyOrderFill::pyFillId)
		.add_property("fillTicksSinceEpoch", &PyOrderFill::fillTicksSinceEpoch)
		.add_property("fillDateTime", &PyOrderFill::pyFillDateTime)
		.add_property("price", &PyOrderFill::price)
		.add_property("volume", &PyOrderFill::volume)
		.add_property("priceType", &PyOrderFill::priceType)
		.add_property("directionSide", &PyOrderFill::directionSide)
		.add_property("offsetFlag", &PyOrderFill::offsetFlag)
		.add_property("createSrc", &PyOrderFill::pyCreateSrc)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.def("toString", &PyOrderFill::toString)
		.def("__str__", &PyOrderFill::toString)
		;


	class_<PyOrder>("Order", no_init)
		.add_property("symbol", &PyOrder::pySymbol)
		.add_property("mtsSymbol", &PyOrder::pyMtsSymbol)
		.add_property("exchId", &PyOrder::pyExchId)
		.add_property("createdDateTime", &PyOrder::pyCreatedDateTime)
		.add_property("beginDateTime", &PyOrder::pyOrderExchId)
		.add_property("createTicksSinceEpoch", &PyOrder::createTicksSinceEpoch)
		.add_property("tradingDay", &PyOrder::tradingDay)
		.add_property("note", &PyOrder::pyNote)
		.add_property("volume", &PyOrder::volume)
		.add_property("fillVolume", &PyOrder::fillVolume)
		.add_property("leftVolume", &PyOrder::leftVolume)
		.add_property("price", &PyOrder::price)
		.add_property("fillAmount", &PyOrder::fillAmount)
		.add_property("priceType", &PyOrder::priceType)
		.add_property("directionSide", &PyOrder::directionSide)
		.add_property("offsetFlag", &PyOrder::offsetFlag)
		.add_property("orderType", &PyOrder::orderType)
		.add_property("referenceId", &PyOrder::referenceId2String)
		.add_property("createSrc", &PyOrder::pyCreateSrc)
		.add_property("status", &PyOrder::status)
		.add_property("statusString", &PyOrder::pyStatusString)
		.add_property("instanceId", &PyOrderNewDone::instanceId)
		.add_property("strategyId", &PyOrderNewDone::strategyId)
		.add_property("perfNote", &PyOrder::pyPerfNote)
		.def("toString", &PyOrder::toString)
		.def("__str__", &PyOrder::toString)
		;

	class_<PyPosition>("Position", no_init)
		.add_property("symbol", &PyPosition::pySymbol)
		.add_property("mtsSymbol", &PyPosition::pyMtsSymbol)
		.add_property("exchId", &PyPosition::pyExchId)
		.add_property("longOpenVolume", &PyPosition::longOpenVolume)
		.add_property("shortOpenVolume", &PyPosition::shortOpenVolume)
		.add_property("longCurrentVolume", &PyPosition::longCurrentVolume)
		.add_property("shortCurrentVolume", &PyPosition::shortCurrentVolume)
		.add_property("longActiveVolume", &PyPosition::longActiveVolume)
		.add_property("shortActiveVolume", &PyPosition::shortActiveVolume)
		.add_property("longCloseVolume", &PyPosition::longCloseVolume)
		.add_property("shortCloseVolume", &PyPosition::shortCloseVolume)
		.add_property("longActiveCloseVolume", &PyPosition::longActiveCloseVolume)
		.add_property("shortActiveCloseVolume", &PyPosition::shortActiveCloseVolume)
		.add_property("longCloseYesterdayVolume", &PyPosition::longCloseYesterdayVolume)
		.add_property("shortCloseYesterdayVolume", &PyPosition::shortCloseYesterdayVolume)
		.add_property("longActiveCloseYesterdayVolume", &PyPosition::longActiveCloseYesterdayVolume)
		.add_property("shortActiveCloseYesterdayVolume", &PyPosition::shortActiveCloseYesterdayVolume)
		.add_property("longLeftVolume", &PyPosition::longLeftVolume)
		.add_property("longLeftYesterdayVolume", &PyPosition::longLeftYesterdayVolume)
		.add_property("shortLeftVolume", &PyPosition::shortLeftVolume)
		.add_property("shortLeftYesterdayVolume", &PyPosition::shortLeftYesterdayVolume)
		.add_property("longFillVolume", &PyPosition::longFillVolume)
		.add_property("shortFillVolume", &PyPosition::shortFillVolume)
		.add_property("longFillAmount", &PyPosition::longFillAmount)
		.add_property("shortFillAmount", &PyPosition::shortFillAmount)
		.add_property("longOpenPnl", &PyPosition::longOpenPnl)
		.add_property("shortOpenPnl", &PyPosition::shortOpenPnl)
		.add_property("longFillPnl", &PyPosition::longFillPnl)
		.add_property("shortFillPnl", &PyPosition::shortFillPnl)
		.add_property("openPnl", &PyPosition::openPnl)
		.add_property("fillPnl", &PyPosition::fillPnl)
		.add_property("totoalPnl", &PyPosition::totoalPnl)
		.def("isNull", &PyPosition::isNull)
		.add_property("attribute", &PyPosition::pyAttribute)
		.def("toString", &PyPosition::toString)
		.def("__str__", &PyPosition::toString)
		;


	class_<PyTradingAccount>("TradingAccount", no_init)
		.add_property("id", &PyTradingAccount::pyId)
		.add_property("commission", &PyTradingAccount::commission)
		.add_property("available", &PyTradingAccount::available)
		.add_property("positionProfit", &PyTradingAccount::positionProfit)
		.add_property("posProfit", &PyTradingAccount::positionProfit)
		.add_property("closeProfit", &PyTradingAccount::closeProfit)
		.add_property("margin", &PyTradingAccount::currMargin)

		;


}

