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
#include "mts_core_api.h"
#include "base/MtsType.h"
#include "const.h"

namespace mts
{
#define FEEDS_TYPE(declare) \
	declare(	FT_TICKS,"TICKS","fmd")\
	declare(	FT_BAR,"BAR","bkd")

	enum FeedsType { FEEDS_TYPE (SELECT_1_AND_COMMA_IN_3) FT_MAX};
	MTS_CORE_API const char*  feedsTypeName(FeedsType type);
	MTS_CORE_API const char*  feedsTypeDirName(FeedsType type);
	MTS_CORE_API FeedsType feedsType(const char* type);


#define ENVIRONMENT_MODE(declare) \
	declare(ENVIR_UNKNOWN,"UNKNOWN","��Чģʽ")\
	declare(ENVIR_SIMU,"SIMU","ģ��Simu")\
	declare(ENVIR_BETA,"BETA","ģ��Beta")\
	declare(ENVIR_REAL,"REAL","ʵ�̽���Real")

	enum EnvironmentMode { ENVIRONMENT_MODE(SELECT_1_AND_COMMA_IN_3) ENVIR_MAX };
	MTS_CORE_API const char*  environmentModeName(EnvironmentMode mode);
	MTS_CORE_API const char*  environmentModeDescription(EnvironmentMode mode);
	MTS_CORE_API EnvironmentMode environmentMode(const char* environmentModeName);



#define EXCH_ID_ENUM(declare) \
	declare(EXCH_UNKNOWN ,"UNKNOWN","--","��Ч������ID")			\
	declare(EXCH_SHSE	, "SHSE",	"SH"	,"�Ϻ�֤ȯ������")			\
	declare(EXCH_SZSE	, "SZSE",	"SZ"	,"����֤ȯ������" )		\
	declare(EXCH_SHFE	, "SHFE",	"SC"	, "�Ϻ��ڻ�������")		\
	declare(EXCH_DCE		, "DCE",		"DC"	, "������Ʒ������")		\
	declare(EXCH_CZCE	, "CZCE",	"ZC"	, "֣����Ʒ������")		\
	declare(EXCH_CFFEX	, "CFFEX",	"SF"	,"�й������ڻ�������")	\
	declare(EXCH_INE		, "INE",		"IN"	,"�й���Դ������")	\
	declare(EXCH_HB		, "HB",		"HB"	,"���Huobi") \
	declare(EXCH_OKC		, "OKC",		"OK","OKCoin")\
	declare(EXCH_BMEX	, "BMEX",	"BX"	,"BitMEX")

	enum ExchId { EXCH_ID_ENUM(SELECT_1_AND_COMMA_IN_4) EXCH_MAX };

	MTS_CORE_API const char*  exchIdName(ExchId exch);
	MTS_CORE_API const char*  exchIdShortName(ExchId exch);
	MTS_CORE_API const char*  exchIdDescription(ExchId exch);
	MTS_CORE_API ExchId exchId(const char* exchIdName);
	MTS_CORE_API ExchId exchIdFromShort(const char* exchIdShortName);
	MTS_CORE_API bool exchHasBothPos(ExchId);




#define PRICE_TYPE_ENUM(declare)	\
	declare(PRICE_UNKNOWN,"UNKNOWN","��Ч��������")	\
	declare(PRICE_LIMIT,"limit","�޼�")	\
	declare(PRICE_MKT,"market","�м�") 

	enum PriceType { PRICE_TYPE_ENUM(SELECT_1_AND_COMMA_IN_3) PRICE_TYPE_MAX };

	MTS_CORE_API const char* priceTypeName(PriceType);
	MTS_CORE_API PriceType priceType(const char*);
	MTS_CORE_API const char* priceTypeDescription(PriceType);


#define DIRECTION_SIDE_ENUM(declare)	\
	declare(D_UNKNOWN,"UNKNOWN","��Ч��������")	\
	declare(D_BUY,"buy","���뿪��")	\
	declare(D_SELL,"sell","���ƽ��") \
	declare(D_SHORT,"short","�������") \
	declare(D_COVER,"cover","����ƽ��") 

	enum DirectionSide { DIRECTION_SIDE_ENUM(SELECT_1_AND_COMMA_IN_3) DIRECTION_SIDE_MAX };

	MTS_CORE_API const char* directionSideName(DirectionSide);
	MTS_CORE_API DirectionSide directionSide(const char*);
	MTS_CORE_API const char* directionSideDescription(DirectionSide);


#define TIME_CONDITION_ENUM(declare)	\
	declare(TC_UNKNOWN,"UNKNOWN","��Ч��Ч��")	\
	declare(TC_IOC,"ioc","������ɣ�������")	\
	declare(TC_GFS,"gfs","������Ч") \
	declare(TC_GFD,"gfd","ָ��������Ч") \
	declare(TC_GTD,"gtd","������Ч") \
	declare(TC_GTC,"gtc","����ǰ��Ч") \
	declare(TC_GFA,"gfa","���Ͼ�����Ч") 

	enum TimeCondition { TIME_CONDITION_ENUM(SELECT_1_AND_COMMA_IN_3) Time_Condition_MAX };

	MTS_CORE_API const char* timeConditionName(TimeCondition);
	MTS_CORE_API TimeCondition timeCondition(const char*);
	MTS_CORE_API const char* timeConditionDescription(TimeCondition);

#define COMBOFFSETFLAG_ENUM(declare)	\
	declare(OF_UNKNOWN,"UNKNOWN","��Ч��������")	\
	declare(OF_OPEN,"open","����")	\
	declare(OF_CLOSE,"close","ƽ��") \
	declare(OF_FORCECLOSE,"forceclose","ǿƽ") \
	declare(OF_CLOSETODAY,"closetoday","ƽ��") \
	declare(OF_CLOSEYESTERDAY,"closeyesterday","ƽ��") 

	enum CombOffsetFlag { COMBOFFSETFLAG_ENUM(SELECT_1_AND_COMMA_IN_3) COMBOFFSETFLAG_MAX };

	MTS_CORE_API const char* combOffsetFlagName(CombOffsetFlag);
	MTS_CORE_API CombOffsetFlag combOffsetFlag(const char*);
	MTS_CORE_API const char* combOffsetFlagDescription(CombOffsetFlag);

#define FRONT_PROTOCOL_ENUM(declare) \
	declare(FP_UNKNOWN,"UNKNOWN","��Ч����Э��") \
	declare(FP_CTP,"CTP","CTP") \
	declare(FP_TDF,"TDF","TDF") \
	declare(FP_IB,"IB","IB") \
	declare(FP_OKC,"OKC","OKCOIN") \
	declare(FP_HB,"HB","HUOBI") \
	declare(FP_BX,"BX","BITMEX")

	enum FrontProtocol { FRONT_PROTOCOL_ENUM(SELECT_1_AND_COMMA_IN_3) FRONT_PROTOCOL_MAX };
	MTS_CORE_API const char* frontProtocolName(FrontProtocol);
	MTS_CORE_API FrontProtocol frontProtocol(const char*);
	MTS_CORE_API const char* frontProtocolDescription(FrontProtocol);



	enum OrderStatus
	{
		OS_NULL				= 0x0000,

		OS_NEW_REQUEST		= 0x0001,
		OS_NEW_DONE			= 0x0002,
		OS_NEW_REJECT		= 0x0004,

		OS_FILL				= 0x0008,

		OS_CANCEL_REQUEST	= 0x0010,
		OS_CANCEL_DONE		= 0x0020,
		OS_CANCEL_REJECT	= 0x0040,

		//OS_REPLACE_REQUEST,
		//OS_REPLACE_DONE,
		//OS_REPLACE_REJECT,

		OS_ORDER_DONE		= 0x0080, 
		OS_ALL_FILL = OS_ORDER_DONE
	};

	enum OrderReportType
	{
		ORT_UNKNOWN,
		ORT_NEW_REQUEST ,
		ORT_NEW_DONE ,
		ORT_NEW_REJECT ,

		ORT_FILL,

		ORT_CANCEL_REQUEST ,
		ORT_CANCEL_DONE ,
		ORT_CANCEL_REJECT ,

		//ORT_REPLACE_REQUEST,
		//ORT_REPLACE_DONE,
		//ORT_REPLACE_REJECT,

		ORT_ORDER_DONE ,
		ORT_ORDER
	};


#define API_TYPE_ENUM(declare)	\
	declare(API_UNKNOWN,"UNKNOWN","��ЧAPI")	\
	declare(API_CTP,"CTP","CTP")

	enum ApiType { API_TYPE_ENUM(SELECT_1_AND_COMMA_IN_3) API_NAX };

	MTS_CORE_API const char* apiTypenName(ApiType);
	MTS_CORE_API ApiType apiType(const char*);
	MTS_CORE_API const char* apiTypeDescription(ApiType);


#define INSTRUMENT_KIND_ENUM(declare)	\
	declare(KIND_NORMAL,"NORMAL","��ͨ��Լ")	\
	declare(KIND_SERIES,"SERIES","������Լ")

	enum InstrumentKind { INSTRUMENT_KIND_ENUM(SELECT_1_AND_COMMA_IN_3) INSTRUMENT_KIND_ENUM};

	MTS_CORE_API const char* instrumentKindName(InstrumentKind);
	MTS_CORE_API InstrumentKind instrumentKind(const char*);
	MTS_CORE_API const char* instrumentKindDescription(InstrumentKind);

#define ALOG_STATUS_ENUM(declare)	\
	declare(ALGO_STATUS_NONE,"none","��ʼ")	\
	declare(ALGO_STATUS_RUNNING,"running","������")	\
	declare(ALGO_STATUS_STOP,"stop","����ͣ")	\
	declare(ALGO_STATUS_CANCELED,"canceled","��ȡ��")	\
	declare(ALGO_STATUS_DELETED,"deleted","��ɾ��")	\
	declare(ALGO_STATUS_FINISHED,"finished","�����")	

	enum AlgoStatus {ALOG_STATUS_ENUM(SELECT_1_AND_COMMA_IN_3) ALOG_STATUS_ENUM};

	MTS_CORE_API const char* algoStatusName(AlgoStatus);
	MTS_CORE_API AlgoStatus algoStatus(const char*);
	MTS_CORE_API const char* algoStatusDescription(AlgoStatus);

#define ALOG_TYPE_ENUM(declare)	\
	declare(ALGO_NONE,"none","��")	\
	declare(ALGO_TIME_SLICE,"timeslice","ʱ��Ƭ��ѯ")\
	declare(ALGO_VOLUME_WEIDGH,"vwap","�ɽ�����Ȩ����")\
	declare(ALGO_TIME_WEIDGH, "twap", "ʱ���Ȩ����")

	enum AlgoType {ALOG_TYPE_ENUM(SELECT_1_AND_COMMA_IN_3) ALOG_TYPE_ENUM};

	MTS_CORE_API const char* algoTypeName(AlgoType);
	MTS_CORE_API AlgoType algoType(const char*);
	MTS_CORE_API const char* algoTypeDescription(AlgoType);
	MTS_CORE_API AlgoType algoDescription2Type(const char*);
}
