
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

	QStringList findSubSections(const QVariantMap& params) {
		QStringList sections;
		for (auto i = params.constBegin(); i != params.constEnd(); ++i) {
			if (i.value().type() == QVariant::Map) {
				sections.append(i.key());
			}
		}
		return sections;
	}

	bool CommonConfigParams::readExchParamsSection(const QVariantMap & params, Front& front){
		auto feedsMode = this->componentMode(MtsComponent::MTS_COMP_FEEDS);
		if (feedsMode == ENVIR_SIMU) {
			QString feedsFrontStr = params.value(FEEDS_FRONT).toString();
			if (feedsFrontStr.isEmpty()) {
				QString dataPath = MtsPath::dataDirPath();
				if (dataPath.isEmpty()) {
					MTS_ERROR("Can not found data dir from '%s' and missing config item '%s' in json file \n", qPrintable(MtsPath::appDirPath()), FEEDS_FRONT);
					return false;

				}
				front._feedsFront = UrlPath::fromLocalPath(dataPath);
			} else {
				front._feedsFront = UrlPath(feedsFrontStr);
			}
		} else {
			if (!params.contains(FEEDS_FRONT)) {
				MTS_ERROR("Missing param '%s'\n", FEEDS_FRONT);
				return false;
			}

			if (!params.contains(TAG_FEEDS_FRONT_PROTOCOL)) {
				MTS_WARN("Use default feeds front protocol : CTP\n");
				front._feedsFrontProtocol = DEFAULT_FEEDS_PROTOCOL;
			} else {
				QString tpStr = params.value( TAG_FEEDS_FRONT_PROTOCOL).toString();
				front._feedsFrontProtocol = mts::frontProtocol(qPrintable(tpStr));
				if (front._feedsFrontProtocol == FP_UNKNOWN) {
					MTS_ERROR("Invalid feeds front protocol '%s'\n", qPrintable(tpStr));
					return false;
				}
			}
			QString feedsFrontStr = params.value(FEEDS_FRONT).toString();
			front._feedsFront = UrlPath(feedsFrontStr);
		}
		if (!front._feedsFront.isValid()) {
			MTS_ERROR("Invalid param '%s' - '%s'\n", FEEDS_FRONT, qPrintable(front._feedsFront.toString()));
			return false;
		}


		auto tradeMode = this->componentMode(MtsComponent::MTS_COMP_TRADE);

		if (tradeMode==ENVIR_SIMU){
			QString matchModel = params.value(SIMU_MATCH_MODEL,DEFAULT_MATCH_MODEL).toString();
			QString tradeFrontStr = params.value(TRADE_FRONT).toString ();
			if ( tradeFrontStr.isEmpty () ){
				front._tradeFront = UrlPath::fromLocalPath ( MtsPath::mtsDirPath () + MATCH_MODEL_PATH_PREFIX + matchModel );
			} else{
				front._tradeFront = UrlPath(tradeFrontStr );
			}
		}else{
			if (!params.contains(TRADE_FRONT)) {
				MTS_ERROR("Missing param '%s'\n", TRADE_FRONT);
				return false;
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

			QString tradeFrontStr = params.value( TRADE_FRONT).toString();
			front._tradeFront = UrlPath(tradeFrontStr);

			front._brokerId = params.value(TRADE_BROKER,
				params.value(FEEDS_BROKER,
					params.value(BROKER)
				)
			).toString();
			if (front._brokerId.isEmpty()) {
				MTS_ERROR("Missing or invalid 'broker'\n");
				return false;
			}
		}


		if (!front._tradeFront.isValid()) {
			MTS_ERROR("Invalid param '%s' - '%s'\n", TRADE_FRONT, qPrintable(front._tradeFront.toString()));
			return false;
		}

		return true;
	}

	bool CommonConfigParams::doLoad(const QVariantMap & params) {
		if (!SysConfigParams::doLoad(params)) {
			return false;
		}

		_fronts.clear();
		QStringList sections = findSubSections(params);
		if (sections.isEmpty()) {
			if (!readExchParamsSection(params, _fronts["*"])) {
				MTS_ERROR("Failed to load config \n");
				return false;
			}
		}else {
			for (const QString& section : sections) {
				assert(params.contains(section));
				assert(params.value(section).type() == QVariant::Map);
				if (!readExchParamsSection(params.value(section).toMap(), _fronts[section])) {
					MTS_ERROR("Failed to load section '%s' config \n", qPrintable(section));
					return false;
				}
			}
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
		if (!_fronts.contains(exchSession) ) {
			if (exchSession == "*") {
				return _fronts.first()._feedsFront;
			} else {
				return feedsFront("*");
			}
		}	else {
			return _fronts.value(exchSession)._feedsFront;
		}
	}

	UrlPath CommonConfigParams::tradeFront(const QString& exchSession) const {
		if (!_fronts.contains(exchSession)) {
			if (exchSession == "*") {
				return _fronts.first()._tradeFront;
			} else {
				return tradeFront("*");
			}
		} else {
			return _fronts.value(exchSession)._tradeFront;
		}
	}

	FrontProtocol CommonConfigParams::feedsFrontProtocol(const QString& exchSession) const {
		assert(_fronts.contains(exchSession));
		return _fronts.value(exchSession)._feedsFrontProtocol;
	}

	FrontProtocol CommonConfigParams::tradeFrontProtocol(const QString& exchSession) const {
		assert(_fronts.contains(exchSession));
		return _fronts.value(exchSession)._tradeFrontProtocol;
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

	QString CommonConfigParams::brokerId(const QString& exchSession) const {
		assert(_fronts.contains(exchSession));
		return _fronts.value(exchSession)._brokerId;
	}


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

		if (this->componentMode(MTS_COMP_FEEDS)==ENVIR_SIMU) {

			_feedsBeginTime = params.value(SIMU_FEEDS_BEGIN_TIME, -1).toInt();

			const int inputBeginDate = params[SIMU_BEING_DATE].toInt();
			const int inputEndDate = params[SIMU_END_DATE].toInt();
			if (inputEndDate < inputBeginDate) {
				MTS_ERROR(" The begin date  (%d) should be early than the end date (%d) \n", inputBeginDate, inputEndDate);
				return false;
			}


			int beginTradingDate = TradingDateMgr::instance()->forceToDate(inputBeginDate);
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












}
