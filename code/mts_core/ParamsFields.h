
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

//The file defines all config params fields names 

//base
#define MODE_PARAM_NAME "mode"
#define INSTANCE_ID "instance_id"
#define STRATEGY_POS_CHECK "strategy_pos_check"
#define MTS_VERSION "mts_version"
#define MTS_OS "mts_os"

//sys
#define LIB_DIR "library_path"
#define CONFIG_DIR "config_dir"
#define DOC_DIR "doc_dir"
#define LOG_FILE "log_file"
#define CONTINUOUS_CONTRACT "continuous_contract"
#define RISK_SETTING "risk_setting"

//common
#define FEEDS_FRONT "feeds_front"
#define TRADE_FRONT "trade_front"
#define FEEDS_GRADE "feeds_grade"
#define CONTINUOUS_SUBSCRIBED "continuous_subscribed"
#define INDICATOR_FILE "indicator_library"
#define TAG_TRADE_FRONT_PROTOCOL "trade_front_protocol"
#define TAG_FEEDS_FRONT_PROTOCOL "feeds_front_protocol"

#define MARKET_TRADING_TIME_TYPE "trading_time"  /*'24H' or other for CN fut*/

//real/beta
#define BROKER "broker"
#define FEEDS_SERVER_NAME "feeds_name"
#define FEEDS_SERVER_TYPE "feeds_type"
#define FEEDS_USER_NAME "feeds_user"
#define FEEDS_BROKER "feeds_broker"

#define TRADE_SERVER_NAME "trade_name"
#define TRADE_SERVER_TYPE "trade_type"
#define TRADE_USER_NAME "trade_user"
#define TRADE_BROKER "trade_broker"



//simu
#define SIMU_MATCH_MODEL "match_model"
#define SIMU_BEING_DATE "clock_begin_date"
#define SIMU_END_DATE "clock_end_date"
#define SIMU_FEEDS_BEGIN_TIME "feeds_begin_time"
