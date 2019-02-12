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
#include "enums.h"
#include <QtCore/QHash>

namespace mts
{
	const char * feedsTypeName(FeedsType type) {
		static const char* names[] = {
			FEEDS_TYPE(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		} else {
			return names[0];
		}
	}

	const char * feedsTypeDirName(FeedsType type) {
		static const char* descriptions[] = {
			FEEDS_TYPE(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(descriptions)) {
			return descriptions[type];
		} else {
			return descriptions[0];
		}
	}

	FeedsType feedsType(const char * type) {
		static QHash<QString, FeedsType> map;
		if (map.isEmpty()) {
			FEEDS_TYPE(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[QString(type).toUpper()];
	}



	const char * environmentModeName(EnvironmentMode mode) {
		static const char* names[] = {
			ENVIRONMENT_MODE(SELECT_2_AND_COMMA_IN_3)
		};
		if (mode >= 0 && mode < ARRAY_SIZE(names)) {
			return names[mode];
		} else {
			return names[0];
		}
	}

	const char * environmentModeDescription(EnvironmentMode mode) {
		static const char* descriptions[] = {
			ENVIRONMENT_MODE(SELECT_3_AND_COMMA_IN_3)
		};
		if (mode >= 0 && mode < ARRAY_SIZE(descriptions)) {
			return descriptions[mode];
		} else {
			return descriptions[0];
		}
	}

	EnvironmentMode environmentMode(const char * environmentModeName) {
		static QHash<QString, EnvironmentMode> map;
		if (map.isEmpty()) {
			ENVIRONMENT_MODE(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[QString(environmentModeName).toUpper()];
	}

	//===============================

	const char * exchIdName(ExchId exch) {
		static const char* names[] = {
			EXCH_ID_ENUM(SELECT_2_AND_COMMA_IN_4)
		};
		if (exch >= 0 && exch < ARRAY_SIZE(names)) {
			return names[exch];
		} else {
			return names[0];
		}
	}

	const char * exchIdShortName(ExchId exch) {
		static const char* shortNames[] = {
			EXCH_ID_ENUM(SELECT_3_AND_COMMA_IN_4)
		};
		if (exch >= 0 && exch < ARRAY_SIZE(shortNames)) {
			return shortNames[exch];
		} else {
			return shortNames[0];
		}

	}

	const char * exchIdDescription(ExchId exch) {
		static const char* descriptions[] = {
			EXCH_ID_ENUM(SELECT_4_AND_COMMA_IN_4)
		};
		if (exch >= 0 && exch < ARRAY_SIZE(descriptions)) {
			return descriptions[exch];
		} else {
			return descriptions[0];
		}
	}

	ExchId exchId(const char * exchIdName) {
		static QHash<QString, ExchId> map;
		if (map.isEmpty()) {
			EXCH_ID_ENUM(DEFINE_NAME_ENUM_MAP4_KEY_2);
		}
		return map[exchIdName];
	}

	ExchId exchIdFromShort(const char * exchIdShortName) {
		static QHash<QString, ExchId> map;
		if (map.isEmpty()) {
			EXCH_ID_ENUM(DEFINE_NAME_ENUM_MAP4_KEY_3);
		}
		return map[exchIdShortName];
	}

	MTS_CORE_API bool exchHasBothPos(ExchId id)
	{
		return id == ExchId::EXCH_SHFE || id == ExchId::EXCH_INE;
	}

	//=========================

	const char* priceTypeName(PriceType type)
	{
		static const char* names[] = {
			PRICE_TYPE_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	PriceType priceType(const char* typeName)
	{
		static QHash<QString, PriceType> map;
		if (map.isEmpty()) {
			PRICE_TYPE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char* priceTypeDescription(PriceType type)
	{
		static const char* names[] = {
			PRICE_TYPE_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	const char* directionSideName(DirectionSide type)
	{
		static const char* names[] = {
			DIRECTION_SIDE_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	DirectionSide directionSide(const char* typeName)
	{
		static QHash<QString, DirectionSide> map;
		if (map.isEmpty()) {
			DIRECTION_SIDE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char* directionSideDescription(DirectionSide type)
	{
		static const char* names[] = {
			DIRECTION_SIDE_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	const char* timeConditionName(TimeCondition type)
	{
		static const char* names[] = {
			TIME_CONDITION_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	TimeCondition timeCondition(const char* typeName)
	{
		static QHash<QString, TimeCondition> map;
		if (map.isEmpty()) {
			TIME_CONDITION_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char* timeConditionDescription(TimeCondition type)
	{
		static const char* names[] = {
			TIME_CONDITION_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	const char* combOffsetFlagName(CombOffsetFlag type)
	{
		static const char* names[] = {
			COMBOFFSETFLAG_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	CombOffsetFlag combOffsetFlag(const char* typeName)
	{
		static QHash<QString, CombOffsetFlag> map;
		if (map.isEmpty()) {
			COMBOFFSETFLAG_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char* combOffsetFlagDescription(CombOffsetFlag type)
	{
		static const char* names[] = {
			COMBOFFSETFLAG_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	const char * frontProtocolName(FrontProtocol protocol) {
		static const char* names[] = {
			FRONT_PROTOCOL_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (protocol >= 0 && protocol < ARRAY_SIZE(names)) {
			return names[protocol];
		} else {
			return names[0];
		}
	}

	FrontProtocol frontProtocol(const char * name) {
		static QHash<QString, FrontProtocol> map;
		if (map.isEmpty()) {
			FRONT_PROTOCOL_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[name];

	}

	 const char * frontProtocolDescription(FrontProtocol protocol) {
		 static const char* names[] = {
			 FRONT_PROTOCOL_ENUM(SELECT_3_AND_COMMA_IN_3)
		 };
		 if (protocol >= 0 && protocol < ARRAY_SIZE(names)) {
			 return names[protocol];
		 } else {
			 return names[0];
		 }
	 }

	const char* apiTypenName(ApiType type)
	{
		static const char* names[] = {
			API_TYPE_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	ApiType apiType(const char* typeName)
	{
		static QHash<QString, ApiType> map;
		if (map.isEmpty()) {
			API_TYPE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	const char* apiTypeDescription(ApiType type)
	{
		static const char* names[] = {
			API_TYPE_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API const char* instrumentKindName(InstrumentKind type)
	{
		static const char* names[] = {
			INSTRUMENT_KIND_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API InstrumentKind instrumentKind(const char* typeName)
	{
		static QHash<QString, InstrumentKind> map;
		if (map.isEmpty()) {
			INSTRUMENT_KIND_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	MTS_CORE_API const char* instrumentKindDescription(InstrumentKind type)
	{
		static const char* names[] = {
			INSTRUMENT_KIND_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API const char* algoStatusName(AlgoStatus type)
	{
		static const char* names[] = {
			ALOG_STATUS_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API AlgoStatus algoStatus(const char* typeName)
	{
		static QHash<QString, AlgoStatus> map;
		if (map.isEmpty()) {
			ALOG_STATUS_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	MTS_CORE_API const char* algoStatusDescription(AlgoStatus type)
	{
		static const char* names[] = {
			ALOG_STATUS_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API const char* algoTypeName(AlgoType type)
	{
		static const char* names[] = {
			ALOG_TYPE_ENUM(SELECT_2_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API AlgoType algoType(const char* typeName)
	{
		static QHash<QString, AlgoType> map;
		if (map.isEmpty()) {
			ALOG_TYPE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_2);
		}
		return map[typeName];
	}

	MTS_CORE_API const char* algoTypeDescription(AlgoType type)
	{
		static const char* names[] = {
			ALOG_TYPE_ENUM(SELECT_3_AND_COMMA_IN_3)
		};
		if (type >= 0 && type < ARRAY_SIZE(names)) {
			return names[type];
		}
		else {
			return names[0];
		}
	}

	MTS_CORE_API AlgoType algoDescription2Type(const char* description)
	{
		static QHash<QString, AlgoType> map;
		if (map.isEmpty()) {
			ALOG_TYPE_ENUM(DEFINE_NAME_ENUM_MAP3_KEY_3);
		}
		return map[description];
	}

}