
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
#include "ConfigParams.h"
#include "base/MtsLog.h"
#include "mts_core/TradingDateMgr.h"
#include "base/MtsUtils.h"
#include "mts_core/OrderType.h"
#include "base/MtsPath.h"

#define DEFAULT_MATCH_MODEL "fast"
#define DEFAULT_FEEDS_PROTOCOL FP_CTP
#define DEFAULT_TRADE_PROTOCOL FP_CTP
#define DEFAULT_FEEDS_GRADE 0

#ifdef _WIN32
	#define MATCH_MODEL_PATH_PREFIX	"/mm_"
#else
	#define MATCH_MODEL_PATH_PREFIX	"/libmm_"
#endif



namespace mts
{
	CommonConfigParams::CommonConfigParams()
		:SysConfigParams()//,_barFeedsDirPath(MtsPath::dataDirPath() + "/bkd/60s")
	{
	}

	CommonConfigParams::~CommonConfigParams() 
	{
	}

	bool CommonConfigParams::readExchParamsSection(const QVariantMap & fullParams, const QString& exchSession){
		QVariant session = fullParams.value(exchSession);
		QVariantMap params;
		if (session.isNull()) {
			if (exchSession != "*") { //for backward compatible
				return false;
			}
			params = fullParams;
		}else {
			params = session.toMap();
		}

		Front& front = _fronts[exchSession];
		if (isSimu()){
			QString matchModel = params.value(SIMU_MATCH_MODEL, DEFAULT_MATCH_MODEL).toString();
			QString feedsFrontStr = params.value ( FEEDS_FRONT ).toString ();
			if ( feedsFrontStr.isEmpty () ){
				QString dataPath = MtsPath::dataDirPath(); 
				if (dataPath.isEmpty()) {
					MTS_ERROR("Can not found data dir from '%s' and missing config item '%s' in json file \n", qPrintable(MtsPath::appDirPath()),FEEDS_FRONT);
					return false;

				}
				front._feedsFront = UrlPath::fromLocalPath (dataPath);
			} else{
				front._feedsFront = UrlPath(feedsFrontStr);
			}
			QString tradeFrontStr = params.value ( TRADE_FRONT ).toString ();
			if ( tradeFrontStr.isEmpty () ){
				front._tradeFront = UrlPath::fromLocalPath ( MtsPath::mtsDirPath () + MATCH_MODEL_PATH_PREFIX + matchModel );
			} else{
				front._tradeFront = UrlPath(tradeFrontStr );
			}
		}else{
			if (!params.contains(FEEDS_FRONT)) {
				MTS_ERROR("Missing param '%s'\n",FEEDS_FRONT);
				return false;
			}
			if (!params.contains(TRADE_FRONT)) {
				MTS_ERROR("Missing param '%s'\n", TRADE_FRONT);
				return false;
			}

			if (!params.contains(TAG_FEEDS_FRONT_PROTOCOL)) {
				MTS_WARN("Use default feeds front protocol : CTP\n");
				front._feedsFrontProtocol = DEFAULT_FEEDS_PROTOCOL;
			} else {
				QString tpStr = params.value(TAG_FEEDS_FRONT_PROTOCOL).toString();
				front._feedsFrontProtocol = mts::frontProtocol(qPrintable(tpStr));
				if (front._feedsFrontProtocol == FP_UNKNOWN) {
					MTS_ERROR("Invalid feeds front protocol '%s'\n", qPrintable(tpStr));
					return false;
				}
			}

			if (!params.contains(TAG_TRADE_FRONT_PROTOCOL)) {
				MTS_WARN("Use default trade front protocol : CTP\n");
				front._tradeFrontProtocol = DEFAULT_TRADE_PROTOCOL;
			} else {
				QString tpStr = params.value(TAG_TRADE_FRONT_PROTOCOL).toString();
				front._tradeFrontProtocol= mts::frontProtocol(qPrintable(tpStr));
				if (front._tradeFrontProtocol == FP_UNKNOWN) {
					MTS_ERROR("Invalid trade front protocol '%s'\n", qPrintable(tpStr));
					return false;
				}
			}

			QString feedsFrontStr = params.value(FEEDS_FRONT).toString();
			QString tradeFrontStr = params.value(TRADE_FRONT).toString();
			front._feedsFront = UrlPath(feedsFrontStr);
			front._tradeFront = UrlPath(tradeFrontStr);

			auto it = params.constFind(TRADE_BROKER);
			if (it != params.constEnd()) {
				front._brokerId = it.value().toString();
			}else {
				auto it = params.constFind(FEEDS_BROKER);
				if (it != params.constEnd()) {
					front._brokerId = it.value().toString();
				}else {
					front._brokerId = params.value(BROKER).toString();
				}
			}
			if (front._brokerId.isEmpty()) {
				MTS_ERROR("Missing or invalid 'broker'\n");
				return false;
			}

		}
		return true;
	}

	bool CommonConfigParams::doLoad(const QVariantMap & params) {
		if (!SysConfigParams::doLoad(params)) {
			return false;
		}

		if (!readExchParamsSection(params,"*")){
			return false;
		}

		const Front& front = _fronts["*"];
		if (!front._feedsFront.isValid()) {
            MTS_ERROR("Invalid param '%s' - '%s'\n", FEEDS_FRONT,qPrintable(front._feedsFront.toString()));
			return false;
		}

		if (!front._tradeFront.isValid()) {
            MTS_ERROR("Invalid param '%s' - '%s'\n", TRADE_FRONT, qPrintable(front._tradeFront.toString()));
			return false;
		}

		_feedsGrade = params.value(FEEDS_GRADE, DEFAULT_FEEDS_GRADE).toInt();

		_indicator = params.value(INDICATOR_FILE, MtsPath::mtsDirPath() + 
#ifdef _WIN32
            "/std_indicator"
#else
            "/libstd_indicator"
#endif
#ifndef NDEBUG
            "_d"
#endif
		).toString();

		_mktTradingTimeType = params.value(MARKET_TRADING_TIME_TYPE, "").toString();

		return true;
	}

	UrlPath CommonConfigParams::feedsFront(const QString& exchSession ) const {
		if (isProxyMode()) {
			return _fronts.value(exchSession)._feedsFront;
		} else {
			return _fronts.value("*")._feedsFront;
		}
	}

	UrlPath CommonConfigParams::tradeFront(const QString& exchSession) const {
		if (isProxyMode()) {
			return _fronts.value(exchSession)._tradeFront;
		} else {
			return _fronts.value("*")._tradeFront;
		}
	}

	FrontProtocol CommonConfigParams::feedsFrontProtocol(const QString& exchSession) const {
		if (isProxyMode()) {
			return _fronts.value(exchSession)._feedsFrontProtocol;
		} else {
			return _fronts.value("*")._feedsFrontProtocol;
		}
	}

	FrontProtocol CommonConfigParams::tradeFrontProtocol(const QString& exchSession) const {
		if (isProxyMode()) {
			return _fronts.value(exchSession)._tradeFrontProtocol;
		} else {
			return _fronts.value("*")._tradeFrontProtocol;
		}
	}

	bool CommonConfigParams::isProxyMode() const {
		return _fronts.size()>1;
	}

	QStringList CommonConfigParams::allExchSessions() const {
		return _fronts.keys();
	}

	int CommonConfigParams::feedsGrade() const {
		return abs(_feedsGrade);
	}

	FeedsType CommonConfigParams::feedsType() const {
		return _feedsGrade>=0? FT_TICKS: FT_BAR;
	}

	UrlPath CommonConfigParams::indicator() const {
		return _indicator;
	}

	QString CommonConfigParams::marketTradingTimeType() const{
		return _mktTradingTimeType;
	}

	//real & beta
	QString CommonConfigParams::brokerId(const QString& exchSession) const {
		if (isProxyMode()) {
			return _fronts.value(exchSession)._brokerId;
		} else {
			return _fronts.value("*")._brokerId;
		}
	}

	//=====

	bool ConfigParams::_isInitialized = false;


	ConfigParams::ConfigParams()
		:CommonConfigParams() , _feedsBeginTime(-1)
		,_beginTradingDate(0), _endTradingDate(0)
		, _beginUTCTicksSinceEpoch(0), _endUTCTicksSinceEpoch(0)
	{
		_isInitialized = true;
	}

	ConfigParams::~ConfigParams() 
	{
		_isInitialized = false;
	}

	ConfigParams * ConfigParams::instance() {
		static ConfigParams cp;
		return &cp;
	}

	bool ConfigParams::isInitialized() {
		return _isInitialized;
	}

	bool ConfigParams::doLoad(const QVariantMap & params) {
		if (!CommonConfigParams::doLoad(params)) {
			return false;
		}


		auto configDir =this->configDir ();
		if ( !TradingDateMgr::init ( configDir , this->marketTradingTimeType()=="24H") ){
			MTS_ERROR ( "Failed to load trading date of '%s'\n" , qPrintable ( configDir ) );
			return false;
		}

		if (isSimu()) {

			_feedsBeginTime = params.value(SIMU_FEEDS_BEGIN_TIME, -1).toInt();

			const int inputBeginDate = params[SIMU_BEING_DATE].toInt();
			const int inputEndDate = params[SIMU_END_DATE].toInt();
			if (inputEndDate < inputBeginDate) {
				MTS_ERROR(" The begin date  (%d) should be early than the end date (%d) \n", inputBeginDate, inputEndDate);
				return false;
			}


			int beginTradingDate = TradingDateMgr::instance()->forceToDate(inputBeginDate);
			//int beginDate = TradingDateMgr::instance()->prevDate(beginTradingDate);
			//int beginTime = TT_NIGHT_BEGIN_TIME;  // use 20:00:00 as begin time for night trading
			int beginTime = TradingDateMgr::instance() ->openTradingTime(beginTradingDate);
			int beginDate = TradingDateMgr::instance()->realDate(beginTradingDate,beginTime);
			DateTime beginDt(beginTime, beginDate);
			if (beginDt.date() != beginDate || beginDt.time() != beginTime) {
				MTS_ERROR("Invalid  '" SIMU_BEING_DATE "' \n");
				return false;
			}
			_beginUTCTicksSinceEpoch = beginDt.toUTCMillisecsSinceEpoch();
			_beginTradingDate = beginTradingDate;

			int endTradingDate = TradingDateMgr::instance()->forceToDate(params[SIMU_END_DATE].toInt());
			int endDate = endTradingDate;
			//int endTime = 195959999;
			int endTime = TradingDateMgr::instance()->closeTradingTime(endTradingDate);
			DateTime endDt(endTime, endDate);
			if (endDt.date() != endDate || endDt.time() != endTime) {
				MTS_ERROR("Invalid  '" SIMU_END_DATE "' \n");
				return false;
			}
			_endUTCTicksSinceEpoch = endDt.toUTCMillisecsSinceEpoch();
			_endTradingDate = endTradingDate;
			assert(_endUTCTicksSinceEpoch>_beginUTCTicksSinceEpoch);
		} else {


		}

		return true;
	}

	//simu
	int ConfigParams::feedsBeginTime() const {
		return _feedsBeginTime;
	}

	int ConfigParams::beginTradingDate() const {
		return _beginTradingDate;
	}

	int ConfigParams::endTradingDate() const {
		return _endTradingDate;
	}

	qint64 ConfigParams::beginUTCTicksSinceEpoch() const {
		return _beginUTCTicksSinceEpoch;
	}

	qint64 ConfigParams::endUTCTicksSinceEpoch() const {
		return _endUTCTicksSinceEpoch;
	}





	//=====


	//QString ConfigParamsFeedsSimu::toLocalPath ( const UrlPath & url , bool checkExists ){
	//	if ( url.scheme () != "file" ){
	//		MTS_ERROR ( "'%s' should be 'file://' on 'simu' mode\n" ,qPrintable(url.toDisplayString()));
 //           return QString();
	//	}

	//	QString fullPath = url.localPath();
	//	if ( checkExists ){
	//		QDir dir(fullPath );
	//		if ( !dir.exists () ){
	//			MTS_ERROR ( "No such dir '%s'\n" , qPrintable ( fullPath ) );
 //               return QString();
	//		}
	//	}
	//	return fullPath;
	//}




}
