
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
#include "CTPUtils.h"
#include "base/MtsUtils.h"
#include <QtCore/QStringList>
#include <iostream>
#include <QtCore/QJsonDocument>
#include "base/enum_ext.h"

#include "mts_core/InstrumentId.h"
#include "mts_core/Quote.h"
#include "mts_core/InstrumentPropertyDb.h"
#include "mts_core/TradingDateMgr.h"
#include "base/DateTime.h"
#include "base/MtsType.h"

QString getFormatString(const QStringList& fieldNames) {
	QStringList fields;
	int number = 0;
	for (int i = 0, size = fieldNames.size(); i < size; ++i) {
		fields.append("\"" + fieldNames[i] + "\":\"%" + QString::number(++number) + "\"");
	}
	return "{"+fields.join(',')+"}";
}


QString CTPUtils::toJsonString(const CThostFtdcRspInfoField * rspInfo) {
	if (rspInfo == nullptr) {
        return QString();
	}

	const QStringList fieldsNames = QStringList()
		<< "ErrorId"
		<< "ErrorMsg"
		;

	return getFormatString(fieldsNames)
		.arg(rspInfo->ErrorID)
		.arg(MtsUtils::toUtf8(rspInfo->ErrorMsg).constData())
		;
}

QString CTPUtils::toJsonString(const CThostFtdcInputOrderField * orderInput) {
	if (orderInput == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "BrokerID"
		<< "InvestorID"
		<< "InstrumentID"
		<< "OrderRef"
		<< "UserID"
		<< "OrderPriceType"
		<< "Direction"
		<< "CombOffsetFlag"
		<< "CombHedgeFlag"
		<< "LimitPrice"
		<< "VolumeTotalOriginal"
		<< "TimeCondition"
		<< "GTDDate"
		<< "VolumeCondition"
		<< "MinVolume"
		<< "ContingentCondition"
		<< "StopPrice"
		<< "ForceCloseReason"
		<< "IsAutoSuspend"
		<< "BusinessUnit"
		<< "RequestID"
		<< "UserForceClose"
		<< "IsSwapOrder"
		/*
		<< "ExchangeID"
		<< "InvestUnitID"
		<< "AccountID"
		<< "CurrencyID"
		<< "ClientID"
		<< "IPAddress"
		<< "MacAddress"
		*/
		;
	return 	getFormatString(fieldsNames)
		.arg(orderInput->BrokerID)
		.arg(orderInput->InvestorID)
		.arg(orderInput->InstrumentID)
		.arg(orderInput->OrderRef)
		.arg(orderInput->UserID)
		.arg(orderInput->OrderPriceType)
		.arg(orderInput->Direction)
		.arg(orderInput->CombOffsetFlag)
		.arg(orderInput->CombHedgeFlag)
		.arg(orderInput->LimitPrice)
		.arg(orderInput->VolumeTotalOriginal)
		.arg(orderInput->TimeCondition)
		.arg(orderInput->GTDDate)
		.arg(orderInput->VolumeCondition)
		.arg(orderInput->MinVolume)
		.arg(orderInput->ContingentCondition)
		.arg(orderInput->StopPrice)
		.arg(orderInput->ForceCloseReason)
		.arg(orderInput->IsAutoSuspend)
		.arg(orderInput->BusinessUnit)
		.arg(orderInput->RequestID)
		.arg(orderInput->UserForceClose)
		.arg(orderInput->IsSwapOrder)
		/*
		.arg(orderInput->ExchangeID)
		.arg(orderInput->InvestUnitID)
		.arg(orderInput->AccountID)
		.arg(orderInput->CurrencyID)
		.arg(orderInput->ClientID)
		.arg(orderInput->IPAddress)
		.arg(orderInput->MacAddress)
		*/
		;
}

QString CTPUtils::toJsonString(const CThostFtdcOrderField * order) {
	if (order == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "BrokerID"
		<< "InvestorID"
		<< "InstrumentID"
		<< "OrderRef"
		<< "UserID"
		<< "OrderPriceType"
		<< "Direction"
		<< "CombOffsetFlag"
		<< "CombHedgeFlag"
		<< "LimitPrice"
		<< "VolumeTotalOriginal"
		<< "TimeCondition"
		<< "GTDDate"
		<< "VolumeCondition"
		<< "MinVolume"
		<< "ContingentCondition"
		<< "StopPrice"
		<< "ForceCloseReason"
		<< "IsAutoSuspend"
		<< "BusinessUnit"
		<< "RequestID"
		<< "OrderLocalID"
		<< "ExchangeID"
		<< "ParticipantID"
		<< "ClientID"
		<< "ExchangeInstID"
		<< "TraderID"
		<< "InstallID"
		<< "OrderSubmitStatus"
		<< "NotifySequence"
		<< "TradingDay"
		<< "SettlementID"
		<< "OrderSysID"
		<< "OrderSource"
		<< "OrderStatus"
		<< "OrderType"
		<< "VolumeTraded"
		<< "VolumeTotal"
		<< "InsertDate"
		<< "InsertTime"
		<< "ActiveTime"
		<< "SuspendTime"
		<< "UpdateTime"
		<< "CancelTime"
		<< "ActiveTraderID"
		<< "ClearingPartID"
		<< "SequenceNo"
		<< "FrontID"
		<< "SessionID"
		<< "UserProductInfo"
		<< "StatusMsg"
		<< "UserForceClose"
		<< "ActiveUserID"
		<< "BrokerOrderSeq"
		<< "RelativeOrderSysID"
		<< "ZCETotalTradedVolume"
		<< "IsSwapOrder"
		;
	return 	getFormatString(fieldsNames)
		.arg(order->BrokerID)
		.arg(order->InvestorID)
		.arg(order->InstrumentID)
		.arg(order->OrderRef)
		.arg(order->UserID)
		.arg(order->OrderPriceType)
		.arg(order->Direction)
		.arg(order->CombOffsetFlag)
		.arg(order->CombHedgeFlag)
		.arg(order->LimitPrice)
		.arg(order->VolumeTotalOriginal)
		.arg(order->TimeCondition)
		.arg(order->GTDDate)
		.arg(order->VolumeCondition)
		.arg(order->MinVolume)
		.arg(order->ContingentCondition)
		.arg(order->StopPrice)
		.arg(order->ForceCloseReason)
		.arg(order->IsAutoSuspend)
		.arg(order->BusinessUnit)
		.arg(order->RequestID)
		.arg(order->OrderLocalID)
		.arg(order->ExchangeID)
		.arg(order->ParticipantID)
		.arg(order->ClientID)
		.arg(order->ExchangeInstID)
		.arg(order->TraderID)
		.arg(order->InstallID)
		.arg(order->OrderSubmitStatus)
		.arg(order->NotifySequence)
		.arg(order->TradingDay)
		.arg(order->SettlementID)
		.arg(order->OrderSysID)
		.arg(order->OrderSource=='\0'?'0': order->OrderSource)
		.arg(order->OrderStatus)
		.arg(order->OrderType == '\0' ? '0' : order->OrderType)
		.arg(order->VolumeTraded)
		.arg(order->VolumeTotal)
		.arg(order->InsertDate)
		.arg(order->InsertTime)
		.arg(order->ActiveTime)
		.arg(order->SuspendTime)
		.arg(order->UpdateTime)
		.arg(order->CancelTime)
		.arg(order->ActiveTraderID)
		.arg(order->ClearingPartID)
		.arg(order->SequenceNo)
		.arg(order->FrontID)
		.arg(order->SessionID)
		.arg(order->UserProductInfo)
		.arg(MtsUtils::toUtf8(order->StatusMsg).constData())
		.arg(order->UserForceClose)
		.arg(order->ActiveUserID)
		.arg(order->BrokerOrderSeq)
		.arg(order->RelativeOrderSysID)
		.arg(order->ZCETotalTradedVolume)
		.arg(order->IsSwapOrder)
		/*
		.arg(order->BranchID)
		.arg(order->InvestUnitID)
		.arg(order->AccountID)
		.arg(order->CurrencyID)
		.arg(order->IPAddress)
		.arg(order->MacAddress)
		*/
		;
}

QString CTPUtils::toJsonString(const CThostFtdcTradeField * trade) {
	if (trade == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "BrokerID"
		<< "InvestorID"
		<< "InstrumentID"
		<< "OrderRef"
		<< "UserID"
		<< "ExchangeID"
		<< "TradeID"
		<< "Direction"
		<< "OrderSysID"
		<< "ParticipantID"
		<< "ClientID"
		<< "TradingRole"
		<< "ExchangeInstID"
		<< "OffsetFlag"
		<< "HedgeFlag"
		<< "Price"
		<< "Volume"
		<< "TradeDate"
		<< "TradeTime"
		<< "TradeType"
		<< "PriceSource"
		<< "TraderID"
		<< "OrderLocalID"
		<< "ClearingPartID"
		<< "BusinessUnit"
		<< "SequenceNo"
		<< "TradingDay"
		<< "SettlementID"
		<< "BrokerOrderSeq"
		<< "TradeSource"
		;
	return 	getFormatString(fieldsNames)
		.arg(trade->BrokerID)
		.arg(trade->InvestorID)
		.arg(trade->InstrumentID)
		.arg(trade->OrderRef)
		.arg(trade->UserID)
		.arg(trade->ExchangeID)
		.arg(trade->TradeID)
		.arg(trade->Direction)
		.arg(trade->OrderSysID)
		.arg(trade->ParticipantID)
		.arg(trade->ClientID)
		.arg(trade->TradingRole)
		.arg(trade->ExchangeInstID)
		.arg(trade->OffsetFlag)
		.arg(trade->HedgeFlag)
		.arg(trade->Price)
		.arg(trade->Volume)
		.arg(trade->TradeDate)
		.arg(trade->TradeTime)
		.arg(trade->TradeType)
		.arg(trade->PriceSource)
		.arg(trade->TraderID)
		.arg(trade->OrderLocalID)
		.arg(trade->ClearingPartID)
		.arg(trade->BusinessUnit)
		.arg(trade->SequenceNo)
		.arg(trade->TradingDay)
		.arg(trade->SettlementID)
		.arg(trade->BrokerOrderSeq)
		.arg(trade->TradeSource)
		;
}

QString CTPUtils::toJsonString(const CThostFtdcInputOrderActionField * orderActionInput) {
	if (orderActionInput == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "BrokerID"
		<< "InvestorID"
		<< "InstrumentID"
		<< "OrderRef"
		<< "RequestID"
		<< "FrontID"
		<< "SessionID"
		<< "ExchangeID"
		<< "OrderSysID"
		<< "ActionFlag"
		<< "LimitPrice"
		<< "VolumeChange"
		<< "UserID"
		<< "InstrumentID"
		;
	return 	getFormatString(fieldsNames)
		.arg(orderActionInput->BrokerID)
		.arg(orderActionInput->InvestorID)
		.arg(orderActionInput->InstrumentID)
		.arg(orderActionInput->OrderRef)
		.arg(orderActionInput->RequestID)
		.arg(orderActionInput->FrontID)
		.arg(orderActionInput->SessionID)
		.arg(orderActionInput->ExchangeID)
		.arg(orderActionInput->OrderSysID)
		.arg(orderActionInput->ActionFlag)
		.arg(orderActionInput->LimitPrice)
		.arg(orderActionInput->VolumeChange)
		.arg(orderActionInput->UserID)
		.arg(orderActionInput->InstrumentID)
		;
}

QString CTPUtils::toJsonString(const CThostFtdcOrderActionField * orderAction) {
	if (orderAction == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "BrokerID"
		<< "InvestorID"
		<< "OrderActionRef"
		<< "OrderRef"
		<< "RequestID"
		<< "FrontID"
		<< "SessionID"
		<< "ExchangeID"
		<< "OrderSysID"
		<< "ActionFlag"
		<< "LimitPrice"
		<< "VolumeChange"
		<< "ActionDate"
		<< "ActionTime"
		<< "TraderID"
		<< "InstallID"
		<< "OrderLocalID"
		<< "ActionLocalID"
		<< "ParticipantID"
		<< "ClientID"
		<< "BusinessUnit"
		<< "OrderActionStatus"
		<< "UserID"
		<< "StatusMsg"
		<< "InstrumentID"
		;
	return 	getFormatString(fieldsNames)
		.arg(orderAction->BrokerID)
		.arg(orderAction->InvestorID)
		.arg(orderAction->InstrumentID)
		.arg(orderAction->OrderRef)
		.arg(orderAction->RequestID)
		.arg(orderAction->FrontID)
		.arg(orderAction->SessionID)
		.arg(orderAction->ExchangeID)
		.arg(orderAction->OrderSysID)
		.arg(orderAction->ActionFlag)
		.arg(orderAction->LimitPrice)
		.arg(orderAction->VolumeChange)
		.arg(orderAction->ActionDate)
		.arg(orderAction->ActionTime)
		.arg(orderAction->TraderID)
		.arg(orderAction->InstallID)
		.arg(orderAction->OrderLocalID)
		.arg(orderAction->ActionLocalID)
		.arg(orderAction->ParticipantID)
		.arg(orderAction->ClientID)
		.arg(orderAction->BusinessUnit)
		.arg(orderAction->OrderActionStatus)
		.arg(orderAction->UserID)
		.arg(MtsUtils::toUtf8(orderAction->StatusMsg).constData())
		.arg(orderAction->InstrumentID)
		;
}

QString CTPUtils::toJsonString(const CThostFtdcInstrumentField * instrumentField) {
	if (instrumentField == nullptr) {
        return QString();
	}
	const QStringList fieldsNames = QStringList()
		<< "InstrumentID"
		<< "ExchangeID"
		<< "InstrumentName"
		<< "ExchangeInstID"
		<< "ProductID"
		<< "ProductClass"
		<< "DeliveryYear"
		<< "DeliveryMonth"
		<< "MaxMarketOrderVolume"
		<< "MinMarketOrderVolume"
		<< "MaxLimitOrderVolume"
		<< "MinLimitOrderVolume"
		<< "VolumeMultiple"
		<< "PriceTick"
		<< "CreateDate"
		<< "OpenDate"
		<< "ExpireDate"
		<< "StartDelivDate"
		<< "EndDelivDate"
		<< "InstLifePhase"
		<< "IsTrading"
		<< "PositionType"
		<< "PositionDateType"
		<< "LongMarginRatio"
		<< "ShortMarginRatio"
		<< "MaxMarginSideAlgorithm"
		<< "UnderlyingInstrID"
		<< "StrikePrice"
		<< "OptionsType"
		<<"UnderlyingMultiple"
		<<"CombinationType"
		;
	return 	getFormatString(fieldsNames)
		.arg(instrumentField->InstrumentID)
		.arg(instrumentField->ExchangeID)
		.arg(LS(instrumentField->InstrumentName))
		.arg(instrumentField->ExchangeInstID)
		.arg(instrumentField->ProductID)
		.arg(instrumentField->ProductClass)
		.arg(instrumentField->DeliveryYear)
		.arg(instrumentField->DeliveryMonth)
		.arg(instrumentField->MaxMarketOrderVolume)
		.arg(instrumentField->MinMarketOrderVolume)
		.arg(instrumentField->MaxLimitOrderVolume)
		.arg(instrumentField->MinLimitOrderVolume)
		.arg(instrumentField->VolumeMultiple)
		.arg(instrumentField->PriceTick)
		.arg(instrumentField->CreateDate)
		.arg(instrumentField->OpenDate)
		.arg(instrumentField->ExpireDate)
		.arg(instrumentField->StartDelivDate)
		.arg(instrumentField->EndDelivDate)
		.arg(instrumentField->InstLifePhase)
		.arg(instrumentField->IsTrading)
		.arg(instrumentField->PositionType)
		.arg(instrumentField->PositionDateType)
		.arg(instrumentField->LongMarginRatio)
		.arg(instrumentField->ShortMarginRatio)
		.arg(instrumentField->MaxMarginSideAlgorithm)
		.arg(instrumentField->UnderlyingInstrID)
		.arg(instrumentField->StrikePrice)
		.arg(instrumentField->OptionsType==0?' ':instrumentField->OptionsType)
		.arg(instrumentField->UnderlyingMultiple)
		.arg(instrumentField->CombinationType)
		;

}





template<class _Ty>
inline _Ty getValue(_Ty v) {
	return v;
}

inline QString getValue(double v) {
	if (v == DBL_MAX) {
		return "NA";
	} else {
		return QString::number(v, 'f', 4);
	}
}


void CTPUtils::toJson(const CThostFtdcTradingAccountField& account, QJsonObject& json)
{
	json.insert("BrokerID", account.BrokerID);
	json.insert("AccountID", account.AccountID);
	json.insert("PreMortgage", account.PreMortgage);
	json.insert("PreCredit", account.PreCredit);
	json.insert("PreDeposit", account.PreDeposit);
	json.insert("PreBalance", account.PreBalance);
	json.insert("PreMargin", account.PreMargin);
	json.insert("InterestBase", account.InterestBase);
	json.insert("Interest", account.Interest);
	json.insert("Deposit", account.Deposit);
	json.insert("Withdraw", account.Withdraw);
	json.insert("FrozenMargin", account.FrozenMargin);
	json.insert("FrozenCash", account.FrozenCash);
	json.insert("FrozenCommission", account.FrozenCommission);
	json.insert("CurrMargin", account.CurrMargin);
	json.insert("CashIn", account.CashIn);
	json.insert("Commission", account.Commission);
	json.insert("CloseProfit", account.CloseProfit);
	json.insert("PositionProfit", account.PositionProfit);
	json.insert("Balance", account.Balance);
	json.insert("Available", account.Available);
	json.insert("WithdrawQuota", account.WithdrawQuota);
	json.insert("Reserve", account.Reserve);
	json.insert("TradingDay", account.TradingDay);
	json.insert("SettlementID", account.SettlementID);
	json.insert("Credit", account.Credit);
	json.insert("Mortgage", account.Mortgage);
	json.insert("ExchangeMargin", account.ExchangeMargin);
	json.insert("DeliveryMargin", account.DeliveryMargin);
	json.insert("ExchangeDeliveryMargin", account.ExchangeDeliveryMargin);
	json.insert("ReserveBalance", account.ReserveBalance);
	json.insert("CurrencyID", account.CurrencyID);
	json.insert("PreFundMortgageIn", account.PreFundMortgageIn);
	json.insert("PreFundMortgageOut", account.PreFundMortgageOut);
	json.insert("FundMortgageIn", account.FundMortgageIn);
	json.insert("FundMortgageOut", account.FundMortgageOut);
	json.insert("FundMortgageAvailable", account.FundMortgageAvailable);
	json.insert("MortgageableFund", account.MortgageableFund);
	json.insert("SpecProductMargin", account.SpecProductMargin);
	json.insert("SpecProductFrozenMargin", account.SpecProductFrozenMargin);
	json.insert("SpecProductCommission", account.SpecProductCommission);
	json.insert("SpecProductFrozenCommission", account.SpecProductFrozenCommission);
	json.insert("SpecProductPositionProfit", account.SpecProductPositionProfit);
	json.insert("SpecProductCloseProfit", account.SpecProductCloseProfit);
	json.insert("SpecProductPositionProfitByAlg", account.SpecProductPositionProfitByAlg);
	json.insert("SpecProductExchangeMargin", account.SpecProductExchangeMargin);
}

void CTPUtils::toJson(const CThostFtdcInvestorPositionField& pos, QJsonObject& json)
{
	json.insert("InstrumentID", pos.InstrumentID);
	json.insert("BrokerID", pos.BrokerID);
	json.insert("InvestorID", pos.InvestorID);
	json.insert("PosiDirection", pos.PosiDirection);
	json.insert("HedgeFlag", pos.HedgeFlag);
	json.insert("PositionDate", pos.PositionDate);
	json.insert("YdPosition", pos.YdPosition);
	json.insert("Position", pos.Position);
	json.insert("LongFrozen", pos.LongFrozen);
	json.insert("ShortFrozen", pos.ShortFrozen);
	json.insert("LongFrozenAmount", pos.LongFrozenAmount);
	json.insert("ShortFrozenAmount", pos.ShortFrozenAmount);
	json.insert("OpenVolume", pos.OpenVolume);
	json.insert("CloseVolume", pos.CloseVolume);
	json.insert("OpenAmount", pos.OpenAmount);
	json.insert("CloseAmount", pos.CloseAmount);
	json.insert("PositionCost", pos.PositionCost);
	json.insert("PreMargin", pos.PreMargin);
	json.insert("UseMargin", pos.UseMargin);
	json.insert("FrozenMargin", pos.FrozenMargin);
	json.insert("FrozenCash", pos.FrozenCash);
	json.insert("FrozenCommission", pos.FrozenCommission);
	json.insert("CashIn", pos.CashIn);
	json.insert("Commission", pos.Commission);
	json.insert("CloseProfit", pos.CloseProfit);
	json.insert("PositionProfit", pos.PositionProfit);
	json.insert("PreSettlementPrice", pos.PreSettlementPrice);
	json.insert("SettlementPrice", pos.SettlementPrice);
	json.insert("TradingDay", pos.TradingDay);
	json.insert("SettlementID", pos.SettlementID);
	json.insert("OpenCost", pos.OpenCost);
	json.insert("ExchangeMargin", pos.ExchangeMargin);
	json.insert("CombPosition", pos.CombPosition);
	json.insert("CombLongFrozen", pos.CombLongFrozen);
	json.insert("CombShortFrozen", pos.CombShortFrozen);
	json.insert("CloseProfitByDate", pos.CloseProfitByDate);
	json.insert("CloseProfitByTrade", pos.CloseProfitByTrade);
	json.insert("TodayPosition", pos.TodayPosition);
	json.insert("MarginRateByMoney", pos.MarginRateByMoney);
	json.insert("MarginRateByVolume", pos.MarginRateByVolume);
	json.insert("StrikeFrozen", pos.StrikeFrozen);
	json.insert("StrikeFrozenAmount", pos.StrikeFrozenAmount);
	json.insert("AbandonFrozen", pos.AbandonFrozen);
}

void CTPUtils::toJson(const CThostFtdcInvestorPositionDetailField& pos, QJsonObject& json)
{
	json.insert("InstrumentID", pos.InstrumentID);
	json.insert("BrokerID", pos.BrokerID);
	json.insert("InvestorID", pos.InvestorID);
	json.insert("HedgeFlag", pos.HedgeFlag);
	json.insert("Direction", pos.Direction);
	json.insert("OpenDate", pos.OpenDate);
	json.insert("TradeID", pos.TradeID);
	json.insert("Volume", pos.Volume);
	json.insert("OpenPrice", pos.OpenPrice);
	json.insert("TradingDay", pos.TradingDay);
	json.insert("SettlementID", pos.SettlementID);
	json.insert("TradeType", pos.TradeType);
	json.insert("CombInstrumentID", pos.CombInstrumentID);
	json.insert("ExchangeID", pos.ExchangeID);
	json.insert("CloseProfitByDate", pos.CloseProfitByDate);
	json.insert("CloseProfitByTrade", pos.CloseProfitByTrade);
	json.insert("PositionProfitByDate", pos.PositionProfitByDate);
	json.insert("PositionProfitByTrade", pos.PositionProfitByTrade);
	json.insert("Margin", pos.Margin);
	json.insert("ExchMargin", pos.ExchMargin);
	json.insert("MarginRateByMoney", pos.MarginRateByMoney);
	json.insert("MarginRateByVolume", pos.MarginRateByVolume);
	json.insert("LastSettlementPrice", pos.LastSettlementPrice);
	json.insert("SettlementPrice", pos.SettlementPrice);
	json.insert("CloseVolume", pos.CloseVolume);
	json.insert("CloseAmount", pos.CloseAmount);
}

void CTPUtils::toJson(const CThostFtdcOrderField& order, QJsonObject& json)
{
	json.insert("BrokerID", order.BrokerID);
	json.insert("InvestorID", order.InvestorID);
	json.insert("InstrumentID", order.InstrumentID);
	json.insert("OrderRef", order.OrderRef);
	json.insert("UserID", order.UserID);
	json.insert("OrderPriceType", order.OrderPriceType);
	json.insert("Direction", order.Direction);
	json.insert("CombOffsetFlag", order.CombOffsetFlag);
	json.insert("CombHedgeFlag", order.CombHedgeFlag);
	json.insert("LimitPrice", order.LimitPrice);
	json.insert("VolumeTotalOriginal", order.VolumeTotalOriginal);
	json.insert("TimeCondition", order.TimeCondition);
	json.insert("GTDDate", order.GTDDate);
	json.insert("VolumeCondition", order.VolumeCondition);
	json.insert("MinVolume", order.MinVolume);
	json.insert("ContingentCondition", order.ContingentCondition);
	json.insert("StopPrice", order.StopPrice);
	json.insert("ForceCloseReason", order.ForceCloseReason);
	json.insert("IsAutoSuspend", order.IsAutoSuspend);
	json.insert("BusinessUnit", order.BusinessUnit);
	json.insert("RequestID", order.RequestID);
	json.insert("OrderLocalID", order.OrderLocalID);
	json.insert("ExchangeID", order.ExchangeID);
	json.insert("ParticipantID", order.ParticipantID);
	json.insert("ClientID", order.ClientID);
	json.insert("ExchangeInstID", order.ExchangeInstID);
	json.insert("TraderID", order.TraderID);
	json.insert("InstallID", order.InstallID);
	json.insert("OrderSubmitStatus", order.OrderSubmitStatus);
	json.insert("NotifySequence", order.NotifySequence);
	json.insert("TradingDay", order.TradingDay);
	json.insert("SettlementID", order.SettlementID);
	json.insert("OrderSysID", order.OrderSysID);
	json.insert("OrderSource", order.OrderSource == '\0' ? '0' : order.OrderSource);
	json.insert("OrderStatus", order.OrderStatus);
	json.insert("OrderType", order.OrderType == '\0' ? '0' : order.OrderType);
	json.insert("VolumeTraded", order.VolumeTraded);
	json.insert("VolumeTotal", order.VolumeTotal);
	json.insert("InsertDate", order.InsertDate);
	json.insert("InsertTime", order.InsertTime);
	json.insert("ActiveTime", order.ActiveTime);
	json.insert("SuspendTime", order.SuspendTime);
	json.insert("UpdateTime", order.UpdateTime);
	json.insert("CancelTime", order.CancelTime);
	json.insert("ActiveTraderID", order.ActiveTraderID);
	json.insert("ClearingPartID", order.ClearingPartID);
	json.insert("SequenceNo", order.SequenceNo);
	json.insert("FrontID", order.FrontID);
	json.insert("SessionID", order.SessionID);
	json.insert("UserProductInfo", order.UserProductInfo);
	json.insert("StatusMsg", MtsUtils::toUtf8(order.StatusMsg).constData());
	json.insert("UserForceClose", order.UserForceClose);
	json.insert("ActiveUserID", order.ActiveUserID);
	json.insert("BrokerOrderSeq", order.BrokerOrderSeq);
	json.insert("RelativeOrderSysID", order.RelativeOrderSysID);
	json.insert("ZCETotalTradedVolume", order.ZCETotalTradedVolume);
	json.insert("IsSwapOrder", order.IsSwapOrder);
}

void CTPUtils::toJson(const CThostFtdcTradeField& fill, QJsonObject& json)
{
	json.insert("BrokerID", fill.BrokerID);
	json.insert("InvestorID", fill.InvestorID);
	json.insert("InstrumentID", fill.InstrumentID);
	json.insert("OrderRef", fill.OrderRef);
	json.insert("UserID", fill.UserID);
	json.insert("ExchangeID", fill.ExchangeID);
	json.insert("TradeID", fill.TradeID);
	json.insert("Direction", fill.Direction);
	json.insert("OrderSysID", fill.OrderSysID);
	json.insert("ParticipantID", fill.ParticipantID);
	json.insert("ClientID", fill.ClientID);
	json.insert("TradingRole", fill.TradingRole);
	json.insert("ExchangeInstID", fill.ExchangeInstID);
	json.insert("OffsetFlag", fill.OffsetFlag);
	json.insert("HedgeFlag", fill.HedgeFlag);
	json.insert("Price", fill.Price);
	json.insert("Volume", fill.Volume);
	json.insert("TradeDate", fill.TradeDate);
	json.insert("TradeTime", fill.TradeTime);
	json.insert("TradeType", fill.TradeType);
	json.insert("PriceSource", fill.PriceSource);
	json.insert("TraderID", fill.TraderID);
	json.insert("OrderLocalID", fill.OrderLocalID);
	json.insert("ClearingPartID", fill.ClearingPartID);
	json.insert("BusinessUnit", fill.BusinessUnit);
	json.insert("SequenceNo", fill.SequenceNo);
	json.insert("TradingDay", fill.TradingDay);
	json.insert("SettlementID", fill.SettlementID);
	json.insert("BrokerOrderSeq", fill.BrokerOrderSeq);
	json.insert("TradeSource", fill.TradeSource);
}

void CTPUtils::toJson(const CThostFtdcInstrumentField& instrument, QJsonObject& json)
{
	json.insert("InstrumentID", instrument.InstrumentID);
	json.insert("ExchangeID", instrument.ExchangeID);
	json.insert("InstrumentName", LS(instrument.InstrumentName));
	json.insert("ExchangeInstID", instrument.ExchangeInstID);
	json.insert("ProductID", instrument.ProductID);
	json.insert("ProductClass", instrument.ProductClass);
	json.insert("DeliveryYear", instrument.DeliveryYear);
	json.insert("DeliveryMonth", instrument.DeliveryMonth);
	json.insert("MaxMarketOrderVolume", instrument.MaxMarketOrderVolume);
	json.insert("MinMarketOrderVolume", instrument.MinMarketOrderVolume);
	json.insert("MaxLimitOrderVolume", instrument.MaxLimitOrderVolume);
	json.insert("MinLimitOrderVolume", instrument.MinLimitOrderVolume);
	json.insert("VolumeMultiple", instrument.VolumeMultiple);
	json.insert("PriceTick", instrument.PriceTick);
	json.insert("CreateDate", instrument.CreateDate);
	json.insert("OpenDate", instrument.OpenDate);
	json.insert("ExpireDate", instrument.ExpireDate);
	json.insert("StartDelivDate", instrument.StartDelivDate);
	json.insert("EndDelivDate", instrument.EndDelivDate);
	json.insert("InstLifePhase", instrument.InstLifePhase);
	json.insert("IsTrading", instrument.IsTrading);
	json.insert("PositionType", instrument.PositionType);
	json.insert("PositionDateType", instrument.PositionDateType);
	json.insert("LongMarginRatio", instrument.LongMarginRatio);
	json.insert("ShortMarginRatio", instrument.ShortMarginRatio);
	json.insert("MaxMarginSideAlgorithm", instrument.MaxMarginSideAlgorithm);
	json.insert("UnderlyingInstrID", instrument.UnderlyingInstrID);
	json.insert("StrikePrice", instrument.StrikePrice);
	json.insert("OptionsType", instrument.OptionsType);
	json.insert("UnderlyingMultiple", instrument.UnderlyingMultiple);
	json.insert("CombinationType", instrument.CombinationType);
}

void CTPUtils::toJson(const CThostFtdcInstrumentMarginRateField& field, QJsonObject& json)
{
	json.insert("InstrumentID", field.InstrumentID);
	json.insert("InvestorRange", field.InvestorRange);
	json.insert("BrokerID", field.BrokerID);
	json.insert("InvestorID", field.InvestorID);
	json.insert("HedgeFlag", field.HedgeFlag);
	json.insert("LongMarginRatioByMoney", field.LongMarginRatioByMoney);
	json.insert("LongMarginRatioByVolume", field.LongMarginRatioByVolume);
	json.insert("ShortMarginRatioByMoney", field.ShortMarginRatioByMoney);
	json.insert("ShortMarginRatioByVolume", field.ShortMarginRatioByVolume);
	json.insert("IsRelative", field.IsRelative);
}

void CTPUtils::toJson(const CThostFtdcInstrumentCommissionRateField& field, QJsonObject& json)
{
	json.insert("InstrumentID", field.InstrumentID);
	json.insert("InvestorRange", field.InvestorRange);
	json.insert("BrokerID", field.BrokerID);
	json.insert("InvestorID", field.InvestorID);
	json.insert("OpenRatioByMoney", field.OpenRatioByMoney);
	json.insert("OpenRatioByVolume", field.OpenRatioByVolume);
	json.insert("CloseRatioByMoney", field.CloseRatioByMoney);
	json.insert("CloseRatioByVolume", field.CloseRatioByVolume);
	json.insert("CloseTodayRatioByMoney", field.CloseTodayRatioByMoney);
	json.insert("CloseTodayRatioByVolume", field.CloseTodayRatioByVolume);
}


#define INSERT_FIELD_TO_JSON(fieldName) json.insert(#fieldName,f.fieldName);


void CTPUtils::toJson ( const CThostFtdcDepthMarketDataField & f, QJsonObject & json ){
	INSERT_FIELD_TO_JSON ( TradingDay );
	INSERT_FIELD_TO_JSON ( InstrumentID );
	INSERT_FIELD_TO_JSON ( ExchangeID );
	INSERT_FIELD_TO_JSON ( ExchangeInstID )
	INSERT_FIELD_TO_JSON ( LastPrice )
	INSERT_FIELD_TO_JSON ( PreSettlementPrice )
	INSERT_FIELD_TO_JSON ( PreClosePrice )
	INSERT_FIELD_TO_JSON ( PreOpenInterest )
	INSERT_FIELD_TO_JSON ( OpenPrice )
	INSERT_FIELD_TO_JSON ( HighestPrice )
	INSERT_FIELD_TO_JSON ( LowestPrice )
	INSERT_FIELD_TO_JSON ( Volume )
	INSERT_FIELD_TO_JSON ( Turnover )
	INSERT_FIELD_TO_JSON ( OpenInterest )
	INSERT_FIELD_TO_JSON ( ClosePrice )
	INSERT_FIELD_TO_JSON ( SettlementPrice )
	INSERT_FIELD_TO_JSON ( UpperLimitPrice )
	INSERT_FIELD_TO_JSON ( LowerLimitPrice )
	INSERT_FIELD_TO_JSON ( PreDelta )
	INSERT_FIELD_TO_JSON ( CurrDelta )
	INSERT_FIELD_TO_JSON ( UpdateTime )
	INSERT_FIELD_TO_JSON ( UpdateMillisec )
	INSERT_FIELD_TO_JSON ( BidPrice1 )
	INSERT_FIELD_TO_JSON ( BidVolume1 )
	INSERT_FIELD_TO_JSON ( AskPrice1 )
	INSERT_FIELD_TO_JSON ( AskVolume1 )
	INSERT_FIELD_TO_JSON ( BidPrice2 )
	INSERT_FIELD_TO_JSON ( BidVolume2 )
	INSERT_FIELD_TO_JSON ( AskPrice2 )
	INSERT_FIELD_TO_JSON ( AskVolume2 )
	INSERT_FIELD_TO_JSON ( BidPrice3 )
	INSERT_FIELD_TO_JSON ( BidVolume3 )
	INSERT_FIELD_TO_JSON ( AskPrice3 )
	INSERT_FIELD_TO_JSON ( AskVolume3 )
	INSERT_FIELD_TO_JSON ( BidPrice4 )
	INSERT_FIELD_TO_JSON ( BidVolume4 )
	INSERT_FIELD_TO_JSON ( AskPrice4 )
	INSERT_FIELD_TO_JSON ( AskVolume4 )
	INSERT_FIELD_TO_JSON ( BidPrice5 )
	INSERT_FIELD_TO_JSON ( BidVolume5 )
	INSERT_FIELD_TO_JSON ( AskPrice5 )
	INSERT_FIELD_TO_JSON ( AskVolume5 )
	INSERT_FIELD_TO_JSON ( AveragePrice )
	INSERT_FIELD_TO_JSON ( ActionDay )
}

QString CTPUtils::depthMarketData2String(const CThostFtdcDepthMarketDataField * f) {
	QJsonObject json;
	toJson ( *f , json );
	QJsonDocument jsonDoc (json );
	return jsonDoc.toJson ().simplified ();
}

void CTPUtils::printDepthMarketData(CThostFtdcDepthMarketDataField * f) {
	auto info = depthMarketData2String(f);
	std::cout << qPrintable(info) << std::endl;
}

QString CTPUtils::investorPositionField2String(const CThostFtdcInvestorPositionField * pos) {
	return QString(
		"InstrumentID:%1\n"
		"BrokerID:%2\n"
		"InvestorID:%3\n"
		"PosiDirection:%4\n"
		"HedgeFlag:%5\n"
		"PositionDate:%6\n"
		"YdPosition:%7\n"
		"Position:%8\n"
		"LongFrozen:%9\n"
		"ShortFrozen:%10\n"
		"LongFrozenAmount:%11\n"
		"ShortFrozenAmount:%12\n"
		"OpenVolume:%13\n"
		"CloseVolume:%14\n"
		"OpenAmount:%15\n"
		"CloseAmount:%16\n"
		"PositionCost:%17\n"
		"PreMargin:%18\n"
		"UseMargin:%19\n"
		"FrozenMargin:%20\n"
		"FrozenCash:%21\n"
		"FrozenCommission:%22\n"
		"CashIn:%23\n"
		"Commission:%24\n"
		"CloseProfit:%25\n"
		"PositionProfit:%26\n"
		"PreSettlementPrice:%27\n"
		"SettlementPrice:%28\n"
		"TradingDay:%29\n"
		"SettlementID:%30\n"
		"OpenCost:%31\n"
		"ExchangeMargin:%32\n"
		"CombPosition:%33\n"
		"CombLongFrozen:%34\n"
		"CombShortFrozen:%35\n"
		"CloseProfitByDate:%36\n"
		"CloseProfitByTrade:%37\n"
		"TodayPosition:%38\n"
		"MarginRateByMoney:%39\n"
		"MarginRateByVolume:%40\n"
		"StrikeFrozen:%41\n"
		"StrikeFrozenAmount:%42\n"
		"AbandonFrozen:%43\n"
	)
		.arg(pos->InstrumentID)
		.arg(pos->BrokerID)
		.arg(pos->InvestorID)
		.arg(pos->PosiDirection)
		.arg(pos->HedgeFlag)
		.arg(pos->PositionDate)
		.arg(pos->YdPosition)
		.arg(pos->Position)
		.arg(pos->LongFrozen)
		.arg(pos->ShortFrozen)
		.arg(pos->LongFrozenAmount)
		.arg(pos->ShortFrozenAmount)
		.arg(pos->OpenVolume)
		.arg(pos->CloseVolume)
		.arg(pos->OpenAmount)
		.arg(pos->CloseAmount)
		.arg(pos->PositionCost)
		.arg(pos->PreMargin)
		.arg(pos->UseMargin)
		.arg(pos->FrozenMargin)
		.arg(pos->FrozenCash)
		.arg(pos->FrozenCommission)
		.arg(pos->CashIn)
		.arg(pos->Commission)
		.arg(pos->CloseProfit)
		.arg(pos->PositionProfit)
		.arg(pos->PreSettlementPrice)
		.arg(pos->SettlementPrice)
		.arg(pos->TradingDay)
		.arg(pos->SettlementID)
		.arg(pos->OpenCost)
		.arg(pos->ExchangeMargin)
		.arg(pos->CombPosition)
		.arg(pos->CombLongFrozen)
		.arg(pos->CombShortFrozen)
		.arg(pos->CloseProfitByDate)
		.arg(pos->CloseProfitByTrade)
		.arg(pos->TodayPosition)
		.arg(pos->MarginRateByMoney)
		.arg(pos->MarginRateByVolume)
		.arg(pos->StrikeFrozen)
		.arg(pos->StrikeFrozenAmount)
		.arg(pos->AbandonFrozen)
		;
}

bool CTPUtils::convertMarketData2Quote(CThostFtdcDepthMarketDataField * md, mts::Quote * qt, const QString & asSymbol) {

	static QHash<QString/*symbol*/, double /*volume*/>  preTotalVolumeMap; //TODO check double //TODO cross day for simu

	mts::InstrumentId id = mts::InstrumentPropertyDb::instance()->findInstrumentId(md->InstrumentID);
	if (!asSymbol.isNull() && id.symbol != asSymbol) {
		id.symbol = asSymbol;
	}
	qt->setInstrumentId(id);
	QString  updateTimeStr = QString(md->UpdateTime);
	int actionDay = atoi(md->ActionDay);
	int tradingDay = atoi(md->TradingDay);
	int hour = updateTimeStr.left(2).toInt();
	int minute = updateTimeStr.mid(3, 2).toInt();
	int second = updateTimeStr.right(2).toInt();
	if (id.exchId == mts::EXCH_DCE) {
		if (hour > 20) {
			actionDay = mts::TradingDateMgr::instance()->prevDate(tradingDay);
		}
		else if (hour < 3) {
			int preTradingDay = mts::TradingDateMgr::instance()->prevDate(tradingDay);
			QDate dt(preTradingDay / 10000, preTradingDay / 100 % 100, preTradingDay % 100);
			dt = dt.addDays(1);
			actionDay = dt.year() * 10000 + dt.month() * 100 + dt.day();
		}
	}
	else if (id.exchId == mts::EXCH_CZCE) {
		if (hour > 20) {
			tradingDay = mts::TradingDateMgr::instance()->nextDate(tradingDay);
		}
	}
	DateTime dt(hour, minute, second, md->UpdateMillisec, actionDay);
	qt->setTicksSinceEpoch(dt.toUTCMillisecsSinceEpoch());
	qt->setTradingDay(tradingDay);
	qt->setPreClosePrice(md->PreClosePrice);
	qt->setOpenPrice(md->OpenPrice);
	qt->setHighPrice(md->HighestPrice);
	qt->setLowPrice(md->LowestPrice);
	qt->setClosePrice(md->ClosePrice);
	qt->setLastPrice(md->LastPrice);
	double& preTotalVolume = preTotalVolumeMap[md->InstrumentID]; //TODO check double
	qt->setLastVolume(qMax(double(0), double(md->Volume - preTotalVolume))); //TODO check double
	qt->setTotalVolume(md->Volume);
	preTotalVolume = md->Volume;
	double bidprice = mts::Double::isValidPrice(md->BidPrice1)?md->BidPrice1: md->LowerLimitPrice;
	double askprice = mts::Double::isValidPrice(md->AskPrice1)?md->AskPrice1: md->UpperLimitPrice;
	qt->setBidPrice(md->BidPrice1);
	qt->setAskPrice(md->AskPrice1);

	qt->setBidVolume(md->BidVolume1);
	qt->setAskVolume(md->AskVolume1);

	qt->setUpperLimitPrice(md->UpperLimitPrice);
	qt->setLowerLimitPrice(md->LowerLimitPrice);
	qt->setPreSettlementPrice(md->PreSettlementPrice);
	qt->setSettlementPrice(md->SettlementPrice);
	qt->setOpenInterest(md->OpenInterest);
	qt->setTurnover(md->Turnover);
	qt->setRevMicrosecond(DateTime::nowToUTCMicrosecsSinceEpoch());
	return true;
}
