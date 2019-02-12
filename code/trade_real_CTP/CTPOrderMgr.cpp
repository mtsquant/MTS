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
#include "CTPOrderMgr.h"
#include "CTPInputOrderField.h"
#include "CTPCancelOrderField.h"
#include <assert.h>
#include "mts_core/OrderAction.h"
#include "mts_core/OrderSet.h"
#include "mts_core/InstrumentPropertyDb.h"
#include "ctp_ext/CTPUtils.h"
#include "base/DateTime.h"
#include "OrderIdImplCtp.h"
#include <QtCore/QDir>

#include "base/PerfTest.h"
#include "base/MtsPath.h"

//#include "TradeEvent.h"
//#include "base/EventQueue.h"

static const int exchOrderInfo = qRegisterMetaType<ExchOrderInfo>("ExchOrderInfo");
int timesting2int(const QString& time/*hh:mm:ss*/) 
{
	int intTime=0;
	for (auto item : time.split(':')) {
		intTime= intTime*100+item.toInt();
	}
	return intTime*1000;
}
CTPOrderMgr::CTPOrderMgr(const QString& address, const QString& brokerId, const QString& userId, const QString& password/*,CThostFtdcTraderApi* api, const MtsCallbacks& cbs*/)
	:CTPTradeClientBase(address, brokerId, userId, password)//,_userApi(api)
{
	const char* pszFlowPath = "./trade_real_temp/";
	if (!QDir().exists(pszFlowPath)) {
		QDir().mkpath(pszFlowPath);
	}
	_userApi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
	_userApi->RegisterSpi(this);

	connect(this, SIGNAL(signalOnRtnOrder(QByteArray)), this, SLOT(doRtnOrder(QByteArray)));
	connect(this, SIGNAL(signalUpdateOrder(ExchOrderInfo)), this, SLOT(updateOrder(ExchOrderInfo)));
	connect(this, SIGNAL(signalOnOrderFilled(QByteArray)), this, SLOT(onOrderFilled(QByteArray)));
	connect(this, SIGNAL(signalOnTradingAccount(QByteArray)), this, SLOT(onTradingAccount(QByteArray)));
}

CTPOrderMgr::~CTPOrderMgr() {
}

void CTPOrderMgr::OnRtnOrder(CThostFtdcOrderField * pOrder) {
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnRtnOrder: OrderRef:%s,StatusMsg:%s,OrderStatus:%c,OrderSubmitStatus:%c\n"), pOrder->OrderRef, MtsUtils::toUtf8(pOrder->StatusMsg).constData(), pOrder->OrderStatus, pOrder->OrderSubmitStatus);
	MTS_DEBUG("OrderRef:%s,StatusMsg:%s,OrderStatus:%c,OrderSubmitStatus:%c\n", pOrder->OrderRef, MtsUtils::toUtf8(pOrder->StatusMsg).constData(), pOrder->OrderStatus, pOrder->OrderSubmitStatus);

	if (pOrder) {
		QString tics = QString::number(DateTime::nowToUTCMicrosecsSinceEpoch());
		memcpy(pOrder->ActiveTraderID, qPrintable(tics), tics.size());
	}
	Q_EMIT signalOnRtnOrder(QByteArray((char*)pOrder, sizeof(CThostFtdcOrderField)));
}

void CTPOrderMgr::OnRtnTrade(CThostFtdcTradeField * pTrade) {
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnRtnTrade:%s\n"), utf8Printable(CTPUtils::toJsonString(pTrade)));
	MTS_DEBUG("OrderMgrBase::OnRtnTrade:%s\n", utf8Printable(CTPUtils::toJsonString(pTrade)));
	Q_EMIT signalOnOrderFilled(QByteArray((char*)pTrade, sizeof(CThostFtdcTradeField)));
}

/*static void buildOrderReport(CThostFtdcOrderField *pOrder, mts::OrderReport* orderReport)
{
	orderReport->setVolume(pOrder->VolumeTotalOriginal);
	orderReport->setPrice(pOrder->LimitPrice);
	orderReport->setFillVolume(pOrder->VolumeTraded);
	//orderReport->setFillAmount(pOrder->);
	orderReport->setPriceType(pOrder->LimitPrice == 0 ? mts::PRICE_MKT : mts::PRICE_LIMIT);
	//orderReport->setDirectonSide(pOrder->);
	//orderReport->setOffsetFlag(pOrder->CombOffsetFlag);
	//orderReport->setTimeCondition(pOrder->);
	//orderReport->setCreateSrc(pOrder->);
	orderReport->setReferenceId(QString(pOrder->OrderRef).toLongLong());
	//orderReport->setTradingAccountId(pOrder->);
	mts::InstrumentId iid;
	iid.symbol = pOrder->InstrumentID;
	iid.typeId = mts::TYPE_FUTR;
	orderReport->setInstrumentId(iid);
	//orderReport->setBook(pOrder->);
	//orderReport->setTradingDay(pOrder->);
	//orderReport->setCreateTicksSinceEpoch(pOrder->);
}*/

static void buildOrderReport(mts::Order *pOrder, mts::OrderReport* orderReport)
{
	orderReport->setVolume(pOrder->volume());
	orderReport->setPrice(pOrder->price());
	orderReport->setFillVolume(pOrder->fillVolume());
	//orderReport->setFillAmount(pOrder->fillAmount());
	//orderReport->setOrderType ( pOrder->orderType () );
	orderReport->setPriceType(pOrder->priceType());
	orderReport->setDirectonSide(pOrder->directionSide());
	orderReport->setOffsetFlag(pOrder->offsetFlag());
	orderReport->setTimeCondition(pOrder->timeCondition());
	orderReport->setCreateSrc(pOrder->createSrc());
	orderReport->setStrategyId(pOrder->strategyId());
	orderReport->setInstanceId(pOrder->instanceId());
	orderReport->setReferenceId(pOrder->referenceId());
	orderReport->setTradingAccountId(pOrder->tradingAccountId());
	orderReport->setInstrumentId(pOrder->instrumentId());
	orderReport->setBook(pOrder->book());
	orderReport->setTradingDay(pOrder->tradingDay());
	orderReport->setCreateTicksSinceEpoch(pOrder->createTicksSinceEpoch());
	orderReport->setOrderExchId(pOrder->orderExchId());
}


static int getMtsOrderSide(char direct, char offsetFlag)
{
	switch (direct)
	{
	case THOST_FTDC_D_Buy:
		if (offsetFlag == THOST_FTDC_OF_Open) {
			return mts::D_BUY;
		}
		return mts::D_COVER;
		break;
	case THOST_FTDC_D_Sell:
		if (offsetFlag == THOST_FTDC_OF_Open) {
			return mts::D_SHORT;
		}
		return mts::D_SELL;
		break;
	default:
		MTS_ERROR("Unknown direct '%c'\n", direct);
		return mts::D_UNKNOWN;
	}
}

void CTPOrderMgr::OnRspOrderInsert(CThostFtdcInputOrderField * pInputOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnRspOrderInsert: %s, RspInfo:%s\n"), pInputOrder ? utf8Printable(CTPUtils::toJsonString(pInputOrder)) : "NULL", pRspInfo ? utf8Printable(CTPUtils::toJsonString(pRspInfo)) : "NULL");
	if (pRspInfo && pRspInfo->ErrorID != 0 && pInputOrder) {
		ExchOrderInfo info;
		//info.tradingDay
		info.sessionId = sessionId();
		info.frontId = frontId();
		info.side = getMtsOrderSide(pInputOrder->Direction, pInputOrder->CombOffsetFlag[0]);
		info.status = mts::OS_NEW_REJECT;
		info.sz = pInputOrder->VolumeTotalOriginal;
		info.leftSz = pInputOrder->VolumeTotalOriginal;
		info.fillSz = 0;
		info.price = pInputOrder->LimitPrice;
		info.clientId = pInputOrder->OrderRef;
		//info.exchOrderId
		//info.exch
		info.instrument = pInputOrder->InstrumentID;
		info.errMsg = LS(pRspInfo->ErrorMsg);
		Q_EMIT signalUpdateOrder(info);
	}
}

void CTPOrderMgr::OnErrRtnOrderInsert(CThostFtdcInputOrderField * pInputOrder, CThostFtdcRspInfoField * pRspInfo) {
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnErrRtnOrderInsert :%s, RspInfo:%s\n"), pInputOrder ? utf8Printable(CTPUtils::toJsonString(pInputOrder)) : "NULL", pRspInfo ? utf8Printable(CTPUtils::toJsonString(pRspInfo)) : "NULL");
	if (pRspInfo && pRspInfo->ErrorID != 0 && pInputOrder) {
		ExchOrderInfo info;
		//info.tradingDay
		info.sessionId = sessionId();
		info.frontId = frontId();
		info.side = getMtsOrderSide(pInputOrder->Direction, pInputOrder->CombOffsetFlag[0]);
		info.status = mts::OS_NEW_REJECT;
		info.sz = pInputOrder->VolumeTotalOriginal;
		info.leftSz = pInputOrder->VolumeTotalOriginal;
		info.fillSz = 0;
		info.price = pInputOrder->LimitPrice;
		info.clientId = pInputOrder->OrderRef;
		//info.exchOrderId
		//info.exch
		info.instrument = pInputOrder->InstrumentID;
		info.errMsg = LS(pRspInfo->ErrorMsg);
		Q_EMIT signalUpdateOrder(info);
	}
}

void CTPOrderMgr::OnRspOrderAction(CThostFtdcInputOrderActionField * pInputOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnRspOrderAction: %s, RspInfo:%s\n"), pInputOrderAction ? utf8Printable(CTPUtils::toJsonString(pInputOrderAction)) : "NULL", pRspInfo ? utf8Printable(CTPUtils::toJsonString(pRspInfo)) : "NULL");
	if (pRspInfo && pRspInfo->ErrorID != 0 && pInputOrderAction) {
		ExchOrderInfo info;
		//info.tradingDay
		info.sessionId = pInputOrderAction->SessionID;
		info.frontId = pInputOrderAction->FrontID;
		//info.side
		info.status = mts::OS_CANCEL_REJECT;
		//info.sz
		//info.leftSz
		//info.fillSz
		info.price = pInputOrderAction->LimitPrice;
		info.clientId = pInputOrderAction->OrderRef;
		//info.exchOrderId
		//info.exch
		info.instrument = pInputOrderAction->InstrumentID;
		info.errMsg = LS(pRspInfo->ErrorMsg);
		Q_EMIT signalUpdateOrder(info);
	}
}

void CTPOrderMgr::OnErrRtnOrderAction(CThostFtdcOrderActionField * pOrderAction, CThostFtdcRspInfoField * pRspInfo) {
	MTS_LOG_FILE("ctp", TIMESTAMP("CTPOrderMgr::OnErrRtnOrderAction: %s, RspInfo:%s\n"), pOrderAction ? utf8Printable(CTPUtils::toJsonString(pRspInfo)) : "NULL", pRspInfo ? utf8Printable(CTPUtils::toJsonString(pRspInfo)) : "NULL");
}

bool CTPOrderMgr::hasOrderClientId(const QString & orderClientId) const {
	return _allOrders.contains(orderClientId);
}

bool CTPOrderMgr::newOrder(mts::OrderActionNew* newAction)
{
	CTPInputOrderField req(this->_BROKER_ID, this->_USER_ID, this->_USER_ID);
	req.setInstrumentID(newAction->instrumentId().symbol); //����
	req.setOrderRef(mts::OrderIdImplCtp(newAction->referenceId()).ctpOrderReferenceId()); //����  
	mts::PriceType priceType = newAction->priceType();
	if (priceType == mts::PRICE_LIMIT) {
		req.setLimitPrice(newAction->price());
	} else if (priceType == mts::PRICE_MKT) {
		req.setLimitPrice(0);
	} else {
		return false;
	}

	req.VolumeTotalOriginal = int(qAbs(newAction->volume())); //����
	req.setDirection(newAction->directionSide());
	req.setCombOffsetFlag(newAction->offsetFlag());

	MTS_LOG_FILE("ctp", TIMESTAMP("OrderActionNew - %s\n"), utf8Printable(req.toJsonString()));
	CThostFtdcOrderField ord = convertInputOrder2OrderField(req);
	if (mts::Order* mtsOrder = mts::OrderSet::instance()->getOrder(newAction->referenceId(), false)) {
		mtsOrder->setPerfNote(QString("%1|apineword=%2").arg(mtsOrder->perfNote()).arg(DateTime::nowToUTCMicrosecsSinceEpoch()));
	}
	updateOrder(ord);
#if 0
    qlonglong nowUtcTicks=boost::chrono::high_resolution_clock::now().time_since_epoch().count();
#ifdef _WIN32
    MTS_LOG_FILE("ctp", TIMESTAMP("%I64d,newOrder,%I64d\n"), newAction->referenceId(), nowUtcTicks);
#else
    MTS_LOG_FILE("ctp", TIMESTAMP("%lld,newOrder,%lld\n"), newAction->referenceId(), nowUtcTicks);
#endif
#endif
	int ret = _userApi->ReqOrderInsert(&req, this->nextRequestId());
	if (ret) {
		MTS_LOG_FILE("ctp", TIMESTAMP("NewOrder Error:%s\n"), utf8Printable(returnValue2String(ret)));
		removeOrder(req.orderRef());
		return false;
	}
	//TODO: convert CTPInputOrderField to CTPOrderField and add it to active orders
	return true;
}

bool CTPOrderMgr::cancelOrder(mts::OrderActionCancel * cancelAction)
{
	QString clientId = cancelAction->referenceId().toString();
	if (!hasOrderClientId(clientId)) {
		MTS_ERROR( "cancelOrder error:can not find order:%s\n", qPrintable(clientId));
		return false;
	}
	CThostFtdcOrderField order = _allOrders[clientId];

	MTS_LOG_FILE("ctp", TIMESTAMP("OrderActionCancel: ClientId:%s,OrderRef:%s,OrderSysID:%s,InstrumentID%s\n"), qPrintable(clientId), order.OrderRef, order.OrderSysID,order.InstrumentID);
	//assert(QString(order.OrderRef) == clientId);

	CTPCancelOrderField req(this->_BROKER_ID, this->_USER_ID, this->_USER_ID);
	req.initFromOrder(order);
	int ret = _userApi->ReqOrderAction(&req, this->nextRequestId());
	if (ret) {
		MTS_LOG_FILE("ctp", "CancelOrder Error:%s\n", utf8Printable(returnValue2String(ret)));
		return false;
	}
	return true;
}

CThostFtdcOrderField CTPOrderMgr::convertInputOrder2OrderField(const CThostFtdcInputOrderField& inputOrder)
{
	CThostFtdcOrderField ord;
	memset(&ord, 0, sizeof(ord));
	memcpy(ord.BrokerID, inputOrder.BrokerID, sizeof(inputOrder.BrokerID));
	memcpy(ord.InvestorID, inputOrder.InvestorID, sizeof(inputOrder.InvestorID));
	memcpy(ord.InstrumentID, inputOrder.InstrumentID, sizeof(inputOrder.InstrumentID));
	memcpy(ord.OrderRef, inputOrder.OrderRef, sizeof(inputOrder.OrderRef));
	memcpy(ord.UserID, inputOrder.UserID, sizeof(inputOrder.UserID));
	ord.OrderPriceType = inputOrder.OrderPriceType;
	ord.Direction = inputOrder.Direction;
	memcpy(ord.CombOffsetFlag, inputOrder.CombOffsetFlag, sizeof(inputOrder.CombOffsetFlag));
	memcpy(ord.CombHedgeFlag, inputOrder.CombHedgeFlag, sizeof(inputOrder.CombHedgeFlag));
	ord.LimitPrice = inputOrder.LimitPrice;
	ord.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
	ord.VolumeTotal = inputOrder.VolumeTotalOriginal;
	ord.TimeCondition = inputOrder.TimeCondition;
	memcpy(ord.GTDDate, inputOrder.GTDDate, sizeof(inputOrder.GTDDate));
	ord.VolumeCondition = inputOrder.VolumeCondition;
	ord.MinVolume = inputOrder.MinVolume;

	ord.ContingentCondition = inputOrder.ContingentCondition;
	ord.StopPrice = inputOrder.StopPrice;
	ord.ForceCloseReason = inputOrder.ForceCloseReason;
	ord.IsAutoSuspend = inputOrder.IsAutoSuspend;
	memcpy(ord.BusinessUnit, inputOrder.BusinessUnit, sizeof(inputOrder.BusinessUnit));
	ord.RequestID = inputOrder.RequestID;
	ord.UserForceClose = inputOrder.UserForceClose;
	ord.IsSwapOrder = inputOrder.IsSwapOrder;
	//memcpy(ord.ExchangeID, inputOrder.ExchangeID, sizeof(inputOrder.ExchangeID));
	//memcpy(ord.InvestUnitID, inputOrder.InvestUnitID, sizeof(inputOrder.InvestUnitID));
	//memcpy(ord.AccountID, inputOrder.AccountID, sizeof(inputOrder.AccountID));
	//memcpy(ord.CurrencyID, inputOrder.CurrencyID, sizeof(inputOrder.CurrencyID));
	//memcpy(ord.ClientID, inputOrder.ClientID, sizeof(inputOrder.ClientID));
	//memcpy(ord.IPAddress, inputOrder.IPAddress, sizeof(inputOrder.IPAddress));
	//memcpy(ord.MacAddress, inputOrder.MacAddress, sizeof(inputOrder.MacAddress));
	ord.OrderSubmitStatus=THOST_FTDC_OSS_InsertSubmitted;
	ord.OrderStatus =THOST_FTDC_OST_Unknown;
	return ord;
}

void CTPOrderMgr::onResponseQueryOrders(const QList<CThostFtdcOrderField>& orders)
{
	CTPTradeClientBase::onResponseQueryOrders(orders);
	for (int i = 0, size = _orders.size(); i < size; ++i) {
		updateOrder(_orders[i]);
	}
}

void CTPOrderMgr::updateOrder(const CThostFtdcOrderField& pOrder)
{
	mts::OrderIdImplCtp cOrderId( pOrder.FrontID, pOrder.SessionID,QString(pOrder.OrderRef).toLongLong() );
	_allOrders[cOrderId.toString()] = pOrder;
}

void CTPOrderMgr::updateOrder(ExchOrderInfo info)
{
	mts::Order* order = mts::OrderSet::instance()->getOrder(info.exchOrderId.c_str(), mts::exchId(info.exch.c_str()));
	mts::OrderIdImplCtp clientId(info.frontId, info.sessionId, QString(info.clientId.c_str()).toLongLong());
	if (!order) {
		order = mts::OrderSet::instance()->getOrder(clientId.id(),false);
	}
	if (!order) {
		MTS_ERROR("can not find order:%s\n", qPrintable(clientId.toString()));
		return;
	}
	int oldTradingDay = order->tradingDay();
	int newTradingDay = info.tradingDay;
	if (newTradingDay !=0 && oldTradingDay != newTradingDay) {
		if (oldTradingDay != 0) {
			MTS_ERROR("update error trading day from '%d' to '%d',order id:%s\n", oldTradingDay,newTradingDay,qPrintable(clientId.toString()));
		}
		order->setTradingDay(newTradingDay);
	}
	QString exchOrderId = info.exchOrderId.c_str();
	if (exchOrderId.isEmpty()) {
		exchOrderId = order->orderExchId();
	}
	if (order->isFinished()) {
		if( info.status== mts::OS_CANCEL_REJECT){ //for cancel cancelled or filled order , should be call cancelReject
			mts::OrderReportCancelReject* rpt = new mts::OrderReportCancelReject ();
			buildOrderReport ( order , rpt );
			rpt->setInstrumentId ( mts::InstrumentId ( info.instrument.c_str () , mts::TYPE_FUTR , mts::exchId ( info.exch.c_str () ) ) );
			rpt->setOrderExchId (exchOrderId);
			rpt->setNote ( info.errMsg );
			Q_EMIT signalOrderReport ( rpt );
			//EventQueue::defaultQueue()->push(EventPtr(new OrderCancelRejectEvent(rpt)));

		}
		return;
	}

	if (info.status == mts::OS_NEW_REJECT) {
		mts::OrderReportNewReject* rpt = new mts::OrderReportNewReject();
		buildOrderReport(order, rpt);
		rpt->setInstrumentId(mts::InstrumentId(info.instrument.c_str(),mts::TYPE_FUTR,mts::exchId(info.exch.c_str())));
		rpt->setOrderExchId(exchOrderId);
		rpt->setNote(info.errMsg);
		rpt->setPerfNote(QString("apinewrjt=%1").arg(info.perfUs));
		Q_EMIT signalOrderReport(rpt);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderNewRejectEvent(rpt)));
		return;
	}
	if (exchOrderId.isEmpty()) {
		return;
	}

	if (!(order->status() & mts::OS_NEW_DONE)) {
		mts::OrderReportNewDone* rpt = new mts::OrderReportNewDone();
		buildOrderReport(order, rpt);
		rpt->setInstrumentId(mts::InstrumentId(info.instrument.c_str(), mts::TYPE_FUTR, mts::exchId(info.exch.c_str())));
		rpt->setOrderExchId(exchOrderId);
		rpt->setPerfNote(QString("apinewdone=%1").arg(info.perfUs));
		Q_EMIT signalOrderReport(rpt);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderNewDoneEvent(rpt)));

	}

	switch (info.status) {
	case mts::OS_CANCEL_REJECT:
	{
		mts::OrderReportCancelReject* rpt = new mts::OrderReportCancelReject();
		buildOrderReport(order, rpt);
		rpt->setInstrumentId(mts::InstrumentId(info.instrument.c_str(), mts::TYPE_FUTR, mts::exchId(info.exch.c_str())));
		rpt->setOrderExchId(exchOrderId);
		rpt->setNote(info.errMsg);
		rpt->setPerfNote(QString("apicxlrjt=%1").arg(info.perfUs));
		Q_EMIT signalOrderReport(rpt);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderCancelRejectEvent(rpt)));

		break;
	}
	case mts::OS_CANCEL_DONE:
	{
		mts::OrderReportCancelDone* rpt = new mts::OrderReportCancelDone();
		buildOrderReport(order, rpt);
		rpt->setInstrumentId(mts::InstrumentId(info.instrument.c_str(), mts::TYPE_FUTR, mts::exchId(info.exch.c_str())));
		rpt->setOrderExchId(exchOrderId);
		rpt->setNote(info.errMsg);
		rpt->setPerfNote(QString("apicxldone=%1").arg(info.perfUs));
		Q_EMIT signalOrderReport(rpt);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderCancelDoneEvent(rpt)));

		break;
	}
	default:
		break;
	}
}

void CTPOrderMgr::onTradingAccount(QByteArray taData)
{
	MTS_DEBUG("onTradingAccount\n");
	onTradingAccount((CThostFtdcTradingAccountField*)taData.data());
}

void CTPOrderMgr::onTradingAccount(CThostFtdcTradingAccountField* pTA)
{
	mts::TradingAccount* ta = new mts::TradingAccount();
	mts::TradingAccountId id;
	id.brokerId = pTA->BrokerID;
	id.username = pTA->AccountID;
	id.currency = currencyId(pTA->CurrencyID);
	ta->setAccountId(id);
	ta->setAvailable(pTA->Available);
	ta->setCommission(pTA->Commission);
	ta->setCurrMargin(pTA->CurrMargin);
	ta->setPositionProfit(pTA->PositionProfit);
	ta->setCloseProfit(pTA->CloseProfit);
	ta->setPreBalance(pTA->PreBalance);
	ta->setDeposit(pTA->Deposit);
	ta->setWithdraw(pTA->Withdraw);
	Q_EMIT signalTradingAccount(ta);
}
void CTPOrderMgr::onResponseQueryInstruments(const QList<CThostFtdcInstrumentField>& instruments)
{
	const QString ts = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
	const QString logFile = QString("instrument_%1").arg(ts);

	CTPTradeClientBase::onResponseQueryInstruments(instruments);
	//TODO: we should have /config/instrument_db.json insteads of querying it
	for (int i = 0, size = instruments.size(); i < size;++i){
		const CThostFtdcInstrumentField& instrument = instruments[i];
		QString jsonStr = CTPUtils::toJsonString(&instrument);
		MTS_LOG_FILE(qPrintable(logFile), "%s\n", qPrintable(jsonStr));


		mts::InstrumentProperty* instrumentProperty = new mts::InstrumentProperty();
		instrumentProperty->setMtsSymbol(instrument.InstrumentID);
		instrumentProperty->setName(LS(instrument.InstrumentName));
		if (instrument.ProductClass == THOST_FTDC_PC_Options) {
			instrumentProperty->setInstrumentId(mts::InstrumentId(instrument.InstrumentID, mts::TYPE_OPTION, mts::exchId(instrument.ExchangeID)));
		} else  if (instrument.ProductClass == THOST_FTDC_PC_Futures){
			instrumentProperty->setInstrumentId(mts::InstrumentId(instrument.InstrumentID, mts::TYPE_FUTR, mts::exchId(instrument.ExchangeID)));
			if (instrument.DeliveryYear != 0) {
				QString mtsSym = QString("%1%2%3").arg(instrument.ProductID).arg(int(instrument.DeliveryYear) % 100, 2, 10, QChar('0')).arg(instrument.DeliveryMonth, 2, 10, QChar('0')).toUpper();
				instrumentProperty->setMtsSymbol(mtsSym);
			}
		} else {
			///�ڻ�
			//#define THOST_FTDC_PC_Futures '1'
			///�ڻ���Ȩ
			//#define THOST_FTDC_PC_Options '2'
			///���
			//#define THOST_FTDC_PC_Combination '3'
			///����
			//#define THOST_FTDC_PC_Spot '4'
			///��ת��
			//#define THOST_FTDC_PC_EFP '5'
			///�ֻ���Ȩ
			//#define THOST_FTDC_PC_SpotOption '6'
			instrumentProperty->setInstrumentId(mts::InstrumentId(instrument.InstrumentID, mts::TYPE_UNKNOWN, mts::exchId(instrument.ExchangeID)));
		}
		instrumentProperty->setVolumeMultiple(instrument.VolumeMultiple);
		instrumentProperty->setPriceTick(instrument.PriceTick);
		instrumentProperty->setMinOrderSize(1);
		instrumentProperty->setOrderSizeIncrement(1);
		instrumentProperty->setCreateDate(atoi(instrument.CreateDate));
		instrumentProperty->setOpenDate(atoi(instrument.OpenDate));
		instrumentProperty->setExpireDate(atoi(instrument.ExpireDate));
		instrumentProperty->setStartDelivDate(atoi(instrument.StartDelivDate));
		instrumentProperty->setEndDelivDate(atoi(instrument.EndDelivDate));
		instrumentProperty->setProuduct(instrument.ProductID);
		mts::InstrumentPropertyDb::instance()->insert(instrumentProperty);
	}

    QString dumpFilePath = QString(MtsPath::configDirPath()+"/instrument_db_%1.json").arg(ts);
	mts::InstrumentPropertyDb::instance()->dumpTo(dumpFilePath);

}

void CTPOrderMgr::onResponseQueryPositions(const QList<CThostFtdcInvestorPositionField>& positions)
{
	CTPTradeClientBase::onResponseQueryPositions(positions);

	//PosiDirection:
	///��
	//#define THOST_FTDC_PD_Net '1'
	///��ͷ
	//#define THOST_FTDC_PD_Long '2'
	///��ͷ
	//#define THOST_FTDC_PD_Short '3'

	//PositionDate:
	///���ճֲ�
	//#define THOST_FTDC_PSD_Today '1'
	///��ʷ�ֲ�
	//#define THOST_FTDC_PSD_History '2'
	QMap<QString, PositionDetail> posDetailMap;
	for (int i = 0, size = positions.size(); i < size; ++i) {
		const CThostFtdcInvestorPositionField& p = positions[i];
		QString sym = p.InstrumentID;
		bool isLongPos = p.PosiDirection == THOST_FTDC_PD_Long;
		bool isTodayPos = p.PositionDate == THOST_FTDC_PSD_Today;
		if (posDetailMap.contains(sym)) {
			PositionDetail& posDetail = posDetailMap[sym];
			posDetail.update(isLongPos, isTodayPos, p);
		}
		else {
			PositionDetail posDetail;
			posDetail.update(isLongPos, isTodayPos, p);
			posDetailMap.insert(sym, posDetail);
		}
	}
	QMapIterator<QString, PositionDetail> i(posDetailMap);
	while (i.hasNext()) {
		i.next();
		QString sym = i.key();
		PositionDetail posDetail = i.value();
		int longYPos = posDetail.longTodayPosition.yesterdayPosition + posDetail.longYesterdayPosition.yesterdayPosition;
        //int longYLeftPos = posDetail.longTodayPosition.position + posDetail.longYesterdayPosition.position - posDetail.longTodayPosition.todayPosition - posDetail.longYesterdayPosition.todayPosition;
        //int longTodayPos = posDetail.longTodayPosition.todayPosition + posDetail.longYesterdayPosition.todayPosition;
		
		int shortYPos = posDetail.shortTodayPosition.yesterdayPosition + posDetail.shortYesterdayPosition.yesterdayPosition;
        //int shortYLeftPos = posDetail.shortTodayPosition.position + posDetail.shortYesterdayPosition.position - posDetail.shortTodayPosition.todayPosition - posDetail.shortYesterdayPosition.todayPosition;

		assert(posDetail.shortYesterdayPosition.todayPosition == 0);
        //int shortTodayPos = posDetail.shortTodayPosition.todayPosition;// + posDetail.shortYesterdayPosition.todayPosition;
		auto id = mts::InstrumentPropertyDb::instance()->findInstrumentId(sym);
		if (id.isNull()) {
			MTS_ERROR("No such symbol '%s' in instrument property db\n", qPrintable(sym));
			continue;
		}
		mts::Position* pos = new mts::Position;
		pos->setInstrumentId(id);
		pos->setLongOpenVolume(longYPos);
		pos->setLongCurrentVolume(longYPos);
		pos->setShortOpenVolume(-shortYPos);
		pos->setShortCurrentVolume(-shortYPos);
		Q_EMIT signalRestorePosition(pos);
		//EventQueue::defaultQueue()->push(EventPtr(new PositionEvent(pos)));

	}
}

void CTPOrderMgr::removeOrder(const QString& clientId)
{
	auto it = _allOrders.find(clientId);
	if (it != _allOrders.end()) {
		_allOrders.erase(it);
	}
}

static mts::CombOffsetFlag ctpCombOffsetFlag2MtsOffSetFlag(int ctpFlag) 
{
	switch (ctpFlag)
	{
	case THOST_FTDC_OF_Open:
		return mts::OF_OPEN;
	case THOST_FTDC_OF_Close:
		return mts::OF_CLOSE;
	case THOST_FTDC_OF_CloseToday:
		return mts::OF_CLOSETODAY;
	case THOST_FTDC_OF_CloseYesterday:
		return mts::OF_CLOSEYESTERDAY;
	default:
		break;
	}
	return mts::OF_UNKNOWN;
}

static mts::DirectionSide getSide(int ctpDirection, int ctpFlag) {
	mts::DirectionSide side = mts::D_UNKNOWN;
	if (ctpDirection == THOST_FTDC_D_Buy) {
		if (ctpFlag == THOST_FTDC_OF_Open) {
			side = mts::D_BUY;
		}
		else if (ctpFlag == THOST_FTDC_OF_Close || ctpFlag == THOST_FTDC_OF_CloseToday || ctpFlag == THOST_FTDC_OF_CloseYesterday) {
			side = mts::D_COVER;
		}
	}
	else if (ctpDirection == THOST_FTDC_D_Sell) {
		if (ctpFlag == THOST_FTDC_OF_Open) {
			side = mts::D_SHORT;
		}
		else if (ctpFlag == THOST_FTDC_OF_Close || ctpFlag == THOST_FTDC_OF_CloseToday || ctpFlag == THOST_FTDC_OF_CloseYesterday) {
			side = mts::D_SELL;
		}
	}
	return side;
}

static mts::Order* ctpOrderToMtsOrder(const CThostFtdcOrderField& ctpOrder)
{
	mts::Order* order = new mts::Order();
	mts::OrderIdImplCtp cOrderId( ctpOrder.FrontID, ctpOrder.SessionID,QString(ctpOrder.OrderRef).toLongLong());
	order->setInstrumentId(mts::InstrumentId(ctpOrder.InstrumentID, mts::TYPE_FUTR, mts::exchId(ctpOrder.ExchangeID)));
	order->setStrategyId(cOrderId.strategyId());
	order->setInstanceId(cOrderId.instanceId());
	order->setReferenceId(cOrderId.id());
	order->setPrice(ctpOrder.LimitPrice);
	order->setOrderExchId(ctpOrder.OrderSysID);
	int ctpFlag = ctpOrder.CombOffsetFlag[0];
	int ctpDirection = ctpOrder.Direction;
	mts::DirectionSide side = getSide(ctpDirection, ctpFlag);
	order->setDirectonSide(side);
	int volume = ctpOrder.VolumeTotalOriginal;
	if (side == mts::D_SELL || side == mts::D_SHORT) {
		volume *= -1;
	}
	order->setVolume(volume);
	//order->setFillVolume(ctpOrder.VolumeTraded);
	order->setOffsetFlag(ctpCombOffsetFlag2MtsOffSetFlag(ctpFlag));
	order->setPriceType(ctpOrder.LimitPrice == 0 ? mts::PRICE_MKT : mts::PRICE_LIMIT);
	order->setCreateTicksSinceEpoch(DateTime(timesting2int(ctpOrder.InsertTime),atoi(ctpOrder.InsertDate)).toUTCMillisecsSinceEpoch());
	order->setTradingDay ( atoi(ctpOrder.TradingDay) );
	int orderStatus = mts::OS_NEW_REQUEST;
	if (atoi(ctpOrder.OrderSysID) == 0 || ctpOrder.OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected) {
		if (ctpOrder.OrderStatus == THOST_FTDC_OST_Canceled || ctpOrder.OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected) {
			orderStatus |= mts::OS_NEW_REJECT;
			order->setNote(LS(ctpOrder.StatusMsg));
			order->setStatus(orderStatus);
			return order;
		}
	}
	if (ctpOrder.OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected) {
		orderStatus |= mts::OS_CANCEL_REJECT;
		order->setNote(LS(ctpOrder.StatusMsg));
		order->setStatus(orderStatus);
		return order;
	}

	if (ctpOrder.OrderStatus == THOST_FTDC_OST_Canceled) {
		orderStatus |= mts::OS_CANCEL_DONE;
	} else if (ctpOrder.OrderStatus == THOST_FTDC_OST_AllTraded) {
		orderStatus |= mts::OS_ORDER_DONE;
	} else if(strlen(ctpOrder.OrderSysID)>0){
		orderStatus |= mts::OS_NEW_DONE;
	}
	order->setStatus(orderStatus);
	order->setNote(LS(ctpOrder.StatusMsg));
	return order;
}

static void ctpFillToMtsFillReport(const CThostFtdcTradeField& ctpFill, mts::OrderReportFill* rpt)
{
	mts::DirectionSide side = getSide(ctpFill.Direction, ctpFill.OffsetFlag);
	rpt->setDirectonSide(side);
	int volume = ctpFill.Volume;
	if (side == mts::D_SELL || side == mts::D_SHORT) {
		volume = -volume;
	}
	mts::InstrumentId iid(ctpFill.InstrumentID, mts::TYPE_FUTR, mts::exchId(ctpFill.ExchangeID));
	//rpt->setPrice(ctpFill.Price);
	rpt->setFillVolume(volume);
	mts::InstrumentBaseProperty* instrumentProperty = mts::InstrumentPropertyDb::instance()->find(iid);
	if (instrumentProperty) {
		rpt->setFillAmount(ctpFill.Price*volume*instrumentProperty->volumeMultiple());
	}else {
		MTS_ERROR("can not find instrument property:%s\n", ctpFill.InstrumentID);
	}
	rpt->setOrderExchId(ctpFill.OrderSysID);
	rpt->setInstrumentId(mts::InstrumentId(ctpFill.InstrumentID, mts::TYPE_FUTR, mts::exchId(ctpFill.ExchangeID)));
	rpt->setFillId(ctpFill.TradeID);
	rpt->setTradingDay (atoi(ctpFill.TradeDate));
	rpt->setFillTicksSinceEpoch(DateTime(timesting2int(ctpFill.TradeTime),atoi(ctpFill.TradeDate)).toUTCMillisecsSinceEpoch());
	if ( strlen ( ctpFill.TradeDate ) > 0 && rpt->tradingDay () == 0 ){
		MTS_ERROR ("Failed to set tradingDay '%s' to int\n", ctpFill.TradeDate );
	}
}

void CTPOrderMgr::onResponseQueryFills(const QList<CThostFtdcTradeField>& fills)
{
	CTPTradeClientBase::onResponseQueryFills(fills);

	restoreOrdersAndFills();   
}

void CTPOrderMgr::onResponseQueryTradingAccounts(const QList<CThostFtdcTradingAccountField>& tradingAccounts)
{
	CTPTradeClientBase::onResponseQueryTradingAccounts(tradingAccounts);
	for (auto ta : tradingAccounts) {
		onTradingAccount(&ta);
	}
}

void CTPOrderMgr::restoreOrdersAndFills()
{
	QMap<QString, mts::Order*> orderMap;
	for (int i = 0, size = _orders.size(); i < size;++i){
		const CThostFtdcOrderField& ctpOrder = _orders[i];
		mts::Order* order = ctpOrderToMtsOrder(ctpOrder);
		order->setFillVolume(0);
		order->setFillAmount(0);
		orderMap.insert(order->orderExchId(), order);
	}

 //   foreach(auto ctpFill , _fills) {
	//	if (orderMap.contains(ctpFill.OrderSysID)) {
	//		mts::Order* order = orderMap[ctpFill.OrderSysID];
	//		mts::OrderReportFill* rpt = ctpFillToMtsFillReport(ctpFill);
	//		rpt->setStrategyId(order->strategyId());
	//		rpt->setInstanceId(order->instanceId());
	//		rpt->setReferenceId(order->referenceId());
	//		order->processOrderReportFill(rpt);
	//	}
	//	else {
	//		MTS_WARN("Unknown fill: 'ctp referenc id:%s,exch order id:%s '\n", ctpFill.OrderRef,ctpFill.OrderSysID);
	//	}
	//}
	for (auto it = orderMap.constBegin(), itEnd = orderMap.constEnd(); it != itEnd; ++it) {
		mts::Order* order = it.value();
		Q_EMIT signalRestoreOrder(order);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderEvent(order)));
	}

	for (int i = 0, size = _fills.size(); i < size; ++i) {
		const CThostFtdcTradeField& ctpFill = _fills[i];
		if (orderMap.contains(ctpFill.OrderSysID)) {
			mts::Order* order = orderMap[ctpFill.OrderSysID];
			mts::OrderReportFill* rpt = new mts::OrderReportFill;
			buildOrderReport(order, rpt);
			ctpFillToMtsFillReport(ctpFill,rpt);
			Q_EMIT signalOrderReport(rpt);
			//EventQueue::defaultQueue()->push(EventPtr(new OrderFillEvent(rpt)));

		}
		else {
			MTS_WARN("Unknown fill: 'ctp referenc id:%s,exch order id:%s '\n", ctpFill.OrderRef, ctpFill.OrderSysID);
		}
	}
}

void CTPOrderMgr::onOrderFilled(QByteArray trade) {
	MTS_DEBUG("onOrderFilled\n");
	CThostFtdcTradeField* pTrade = (CThostFtdcTradeField *)trade.data();
	//onResponseQueryFill(*pTrade);

	mts::Order* order = mts::OrderSet::instance()->getOrder(pTrade->OrderSysID,mts::exchId(pTrade->ExchangeID));
	if (!order) {
		MTS_ERROR("can not find order:exchOrderId=%s,exch=%s,fillId=%s\n", pTrade->OrderSysID,pTrade->ExchangeID,pTrade->TradeID);
		return;
	}
	mts::OrderReportFill* rpt = new mts::OrderReportFill();
	buildOrderReport(order, rpt);
	//rpt->setPrice(pTrade->Price);
	int volume = pTrade->Volume;
	if (order->directionSide() == mts::D_SELL || order->directionSide() == mts::D_SHORT) {
		volume = -volume;
	}
	rpt->setFillVolume(volume);
	mts::InstrumentId iid(pTrade->InstrumentID, mts::TYPE_FUTR, mts::exchId(pTrade->ExchangeID));
	mts::InstrumentBaseProperty* instrumentProperty = mts::InstrumentPropertyDb::instance()->find(iid);
	if (instrumentProperty) {
		rpt->setFillAmount(volume*pTrade->Price*instrumentProperty->volumeMultiple());
	} else {
		MTS_ERROR("can not find instrument property:%s\n", pTrade->InstrumentID);
	}
	rpt->setStrategyId(order->strategyId());
	rpt->setInstanceId(order->instanceId());
	rpt->setReferenceId(order->referenceId());
	rpt->setInstrumentId(order->instrumentId());
	rpt->setFillId(pTrade->TradeID);
	rpt->setTradingDay (atoi(pTrade->TradeDate));
	if ( strlen ( pTrade->TradeDate ) > 0 && rpt->tradingDay () == 0 ){
		MTS_ERROR ( "Failed to set tradingDay '%s' to int\n" , pTrade->TradeDate );
	}

	Q_EMIT signalOrderReport(rpt);
	//EventQueue::defaultQueue()->push(EventPtr(new OrderFillEvent(rpt)));

}

void CTPOrderMgr::doRtnOrder(QByteArray order)
{
	CThostFtdcOrderField *pOrder = (CThostFtdcOrderField *)order.data();
	if (!pOrder) {
		return;
	}
	updateOrder(*pOrder);

	mts::Order* mtsOrder = mts::OrderSet::instance()->getOrder(mts::OrderIdImplCtp(pOrder->FrontID, pOrder->SessionID, QString(pOrder->OrderRef).toLongLong()).id(), false);
	if (!mtsOrder) {
		mtsOrder = ctpOrderToMtsOrder(*pOrder);
		Q_EMIT signalRestoreOrder(mtsOrder);
		//EventQueue::defaultQueue()->push(EventPtr(new OrderEvent(mtsOrder)));
		return;
	}
	ExchOrderInfo info;
	info.tradingDay = atoi(pOrder->TradingDay);
	info.sessionId = pOrder->SessionID;
	info.frontId = pOrder->FrontID;
	info.side = getMtsOrderSide(pOrder->Direction, pOrder->CombOffsetFlag[0]);
	info.sz = pOrder->VolumeTotalOriginal;
	info.leftSz = pOrder->VolumeTotal;
	info.fillSz = pOrder->VolumeTraded;
	info.price = pOrder->LimitPrice;
	info.clientId = pOrder->OrderRef;
	info.exch = pOrder->ExchangeID;
	info.instrument = pOrder->InstrumentID;
	info.errMsg = LS(pOrder->StatusMsg);
    info.perfUs = QString(pOrder->ActiveTraderID).toLongLong();
	if (atoi(pOrder->OrderSysID) != 0) {
		info.exchOrderId = pOrder->OrderSysID;
	}
	if (atoi(pOrder->OrderSysID) == 0 || pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected) {
		if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled || pOrder->OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected) {
			info.status = mts::OS_NEW_REJECT;
			updateOrder(info);
			return;
		}
	}
	if (pOrder->OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected) {
		info.status = mts::OS_CANCEL_REJECT;
		updateOrder(info);
		return;
	}
	
	if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
		info.status = mts::OS_CANCEL_DONE;
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded) {
		info.status = mts::OS_ORDER_DONE;
	}
	else {
		info.status = mts::OS_NEW_DONE;
	}

	updateOrder(info);
}
