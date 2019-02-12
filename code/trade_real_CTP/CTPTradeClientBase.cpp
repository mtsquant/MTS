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
#include "CTPTradeClientBase.h"

#include <boost/bind.hpp>
#include "QueryPositionAction.h"
#include "QueryOrderAction.h"
#include "QueryFillAction.h"
#include "QueryInstrumentAction.h"
#include "QuerySettlementInfoAction.h"
#include "QueryTradingAccountAction.h"
#include "mts_core/Environment.h"
#include "ctp_ext/CTPUtils.h"
//#include "PositionMgr.h"
//#include "CTPOrderMgr.h"


CTPTradeClientBase::CTPTradeClientBase(const QString& address, const QString& brokerId, const QString& userId, const QString& password/*,const MtsCallbacks& callbacks*/)
	:QObject(),CTPClientCommon<CThostFtdcTraderApi, CThostFtdcTraderSpi>(address, brokerId, userId, password),//_callbacks(callbacks),
	_functionsListMutex(QMutex::Recursive),
	_querySettlementStatus(QS_READY), 
	_queryInstrumentStatus(QS_READY), 
	_queryPosStatus(QS_READY),
	_queryOrderStatus(QS_READY),
	_queryFillStatus(QS_READY),
	_queryTradingAccountStatus(QS_READY),
	_qryPosAction(nullptr), _qryOrderAction(nullptr), _qryFillAction(nullptr), _qryInstrumentAction(nullptr), _qrySettlementAction(nullptr),
	_isQueryFinished(false), _qryTradingAccountAction(nullptr)
{

	_queryTimer = new QTimer(this);
	connect(_queryTimer, SIGNAL(timeout()), this, SLOT(doNextQuery()));
	_queryTimer->start(2000);

}


CTPTradeClientBase::~CTPTradeClientBase() 
{
	_queryTimer->stop();

	_userApi->RegisterSpi(nullptr);
	_userApi->Release();
	_userApi = nullptr;
	if (_qrySettlementAction) {
		delete _qrySettlementAction;
		_qrySettlementAction = nullptr;
	}
	if (_qryInstrumentAction) {
		delete _qryInstrumentAction;
		_qryInstrumentAction = nullptr;
	}
	if (_qryPosAction) {
		delete _qryPosAction;
		_qryPosAction = nullptr;
	}
	if (_qryOrderAction) {
		delete _qryOrderAction;
		_qryOrderAction = nullptr;
	}
	if (_qryFillAction) {
		delete _qryFillAction;
		_qryFillAction = nullptr;
	}
	if (_qryTradingAccountAction) {
		delete _qryTradingAccountAction;
		_qryTradingAccountAction = nullptr;
	}
}

void CTPTradeClientBase::beforeRegisterFront() {
	// ����˽����
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	_userApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	// ���Ĺ�����
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	_userApi->SubscribePublicTopic(THOST_TERT_QUICK);

}

void CTPTradeClientBase::startAsyncQueries() {
	MTS_DEBUG("\n");
	//1.query settlement
	//2.query instrument
	//3.query position
	//4.query order
	//5.query trade
	{
		QMutexLocker l(&_functionsListMutex);
		//_queryFunctionList.append(qMakePair(boost::bind(&CTPTradeClientBase::querySettlement, this),&_querySettlementStatus));
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::querySettlement, this), &_querySettlementStatus,1});
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::queryInstruments, this),&_queryInstrumentStatus,1 });
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::queryPositions, this),&_queryPosStatus,1 });
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::queryOrders, this),&_queryOrderStatus,1 });
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::queryFills, this),&_queryFillStatus,1 });
		_queryFunctionList.append({boost::bind(&CTPTradeClientBase::queryTradingAccount, this), &_queryTradingAccountStatus,INT64_MAX });
		//_querySettlementFinish = QS_DONE;
		//_queryInstrumentFinish = QS_DONE;
		//_queryFillFinish = QS_DONE;
	}

}

bool CTPTradeClientBase::querySettlement() {
    //MTS_DEBUG("\n");
	CThostFtdcSettlementInfoConfirmField pSettlementInfoConfirm;
	memset(&pSettlementInfoConfirm, 0, sizeof(CThostFtdcSettlementInfoConfirmField));
	strcpy(pSettlementInfoConfirm.BrokerID, qPrintable(this->_BROKER_ID));
	strcpy(pSettlementInfoConfirm.InvestorID, qPrintable(this->_USER_ID));
	_qrySettlementAction = new QuerySettlementInfoAction(pSettlementInfoConfirm);
	QObject::connect(_qrySettlementAction, SIGNAL(responseQuerySettlementInfo(const QList<CThostFtdcSettlementInfoConfirmField>&)),
		this, SLOT(onResponseQuerySettlementInfo(const QList<CThostFtdcSettlementInfoConfirmField>&)));
	QObject::connect(_qrySettlementAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this,SLOT(onErrorQuerySettlementInfo(int, const QString&, OwnerLevel)));
    auto qryRet=_qrySettlementAction->request(this->_userApi,this->nextRequestId());
    return processQryRet("querySettlement",qryRet );
}

bool CTPTradeClientBase::queryInstruments() {
    //MTS_DEBUG("\n");
	CThostFtdcQryInstrumentField qryInstrument;
	memset(&qryInstrument, 0, sizeof(CThostFtdcQryInstrumentField));
	_qryInstrumentAction = new QueryInstrumentAction(qryInstrument);
	QObject::connect(_qryInstrumentAction, SIGNAL(responseQueryInstruments(const QList<CThostFtdcInstrumentField>&)),
		this, SLOT(onResponseQueryInstruments(const QList<CThostFtdcInstrumentField>&)));
	QObject::connect(_qryInstrumentAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this, SLOT(onErrorQueryInstruments(int, const QString&, OwnerLevel)));
	return processQryRet("queryInstruments", _qryInstrumentAction->request(this->_userApi, this->nextRequestId()));
}

bool CTPTradeClientBase::queryTradingAccount()
{
	//MTS_DEBUG("\n");
	CThostFtdcQryTradingAccountField qryTradingAccount;
	memset(&qryTradingAccount, 0, sizeof(CThostFtdcQryTradingAccountField));
	_qryTradingAccountAction = new QueryTradingAccountAction(qryTradingAccount);
	QObject::connect(_qryTradingAccountAction, SIGNAL(responseQueryTradingAccount(const QList<CThostFtdcTradingAccountField>&)),
		this, SLOT(onResponseQueryTradingAccounts(const QList<CThostFtdcTradingAccountField>&)));
	QObject::connect(_qryTradingAccountAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this, SLOT(onErrorQueryQueryTradingAccounts(int, const QString&, OwnerLevel)));
	return processQryRet("queryTradingAccount", _qryTradingAccountAction->request(this->_userApi, this->nextRequestId()));
}

const QList<CThostFtdcSettlementInfoConfirmField>& CTPTradeClientBase::settlementInfos()
{
	return _settlementInfos;
}

const QList<CThostFtdcInvestorPositionField>& CTPTradeClientBase::positions()
{
	return _positions;
}

const QList<CThostFtdcOrderField>& CTPTradeClientBase::orders()
{
	return _orders;
}

const QList<CThostFtdcTradeField>& CTPTradeClientBase::fills()
{
	return _fills;
}

const QList<CThostFtdcInstrumentField>& CTPTradeClientBase::instruments()
{
	return _instruments;
}

bool CTPTradeClientBase::isQueryFinished()
{
	return _isQueryFinished;
}

int CTPTradeClientBase::frontId() const {
	return _frontId;
}

int CTPTradeClientBase::sessionId() const {
	return _sessionId;
}

bool CTPTradeClientBase::queryPositions() {
	MTS_DEBUG("\n");
	CThostFtdcQryInvestorPositionField qryPositionField;
	memset(&qryPositionField, 0, sizeof(CThostFtdcQryInvestorPositionField));
	strcpy(qryPositionField.BrokerID, qPrintable(this->_BROKER_ID));
	strcpy(qryPositionField.InvestorID, qPrintable(this->_USER_ID));
	_qryPosAction = new QueryPositionAction(qryPositionField);
	QObject::connect(_qryPosAction, SIGNAL(responseQueryPositions(const QList<CThostFtdcInvestorPositionField>&)),
		this, SLOT(onResponseQueryPositions(const QList<CThostFtdcInvestorPositionField>&)));
	QObject::connect(_qryPosAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this, SLOT(onErrorQueryQueryPositions(int, const QString&, OwnerLevel)));
	return processQryRet("queryPositions", _qryPosAction->request(this->_userApi, this->nextRequestId()));
}

bool CTPTradeClientBase::queryOrders() {
	MTS_DEBUG("\n");
	CThostFtdcQryOrderField qryOrder;
	memset(&qryOrder, 0, sizeof(CThostFtdcQryOrderField));
	strcpy(qryOrder.BrokerID, qPrintable(this->_BROKER_ID));
	strcpy(qryOrder.InvestorID, qPrintable(this->_USER_ID));
	_qryOrderAction = new QueryOrderAction(qryOrder);
	QObject::connect(_qryOrderAction, SIGNAL(responseQueryOrders(const QList<CThostFtdcOrderField>&)),
		this, SLOT(onResponseQueryOrders(const QList<CThostFtdcOrderField>&)));
	QObject::connect(_qryOrderAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this, SLOT(onErrorQueryQueryOrders(int, const QString&, OwnerLevel)));
	return processQryRet("queryOrders", _qryOrderAction->request(this->_userApi, this->nextRequestId()));
}

bool CTPTradeClientBase::queryFills() {
	MTS_DEBUG("\n");
	CThostFtdcQryTradeField qryTrade;
	memset(&qryTrade, 0, sizeof(CThostFtdcQryTradeField));
	strcpy(qryTrade.BrokerID, qPrintable(this->_BROKER_ID));
	strcpy(qryTrade.InvestorID, qPrintable(this->_USER_ID));
	_qryFillAction = new QueryFillAction(qryTrade);
	QObject::connect(_qryFillAction, SIGNAL(responseQueryFills(const QList<CThostFtdcTradeField>&)),
		this, SLOT(onResponseQueryFills(const QList<CThostFtdcTradeField>&)));
	QObject::connect(_qryFillAction, SIGNAL(actionError(int, const QString&, OwnerLevel)),
		this, SLOT(onErrorQueryQueryFills(int, const QString&, OwnerLevel)));
	return processQryRet("queryFills", _qryFillAction->request(this->_userApi, this->nextRequestId()));
}



void CTPTradeClientBase::onResponseQuerySettlementInfo(const QList<CThostFtdcSettlementInfoConfirmField>& settlementInfos) {
	//MTS_DEBUG("\n");
	_settlementInfos = settlementInfos;
	_querySettlementStatus = QS_DONE;
}

void CTPTradeClientBase::onErrorQuerySettlementInfo(int errorId, const QString & errorMsg, OwnerLevel) {
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_querySettlementStatus = QS_ERROR;
}


void CTPTradeClientBase::onResponseQueryInstruments(const QList<CThostFtdcInstrumentField>& instruments) {
	//MTS_DEBUG("\n");
	_instruments = instruments;
	_queryInstrumentStatus = QS_DONE;
}

void CTPTradeClientBase::onErrorQueryInstruments(int errorId, const QString & errorMsg, OwnerLevel) {
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_queryInstrumentStatus = QS_ERROR;
}


void CTPTradeClientBase::onResponseQueryPositions(const QList<CThostFtdcInvestorPositionField>& positions) {
	//MTS_DEBUG("\n");
	_positions = positions;
	_queryPosStatus = QS_DONE;
}

void CTPTradeClientBase::onErrorQueryQueryPositions(int errorId, const QString & errorMsg, OwnerLevel) {
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_queryPosStatus = QS_ERROR;
}

void CTPTradeClientBase::onResponseQueryOrders(const QList<CThostFtdcOrderField>& orders) {
	//MTS_DEBUG("\n");
	_orders.clear();
	for (int i = 0, size = orders.size(); i < size; ++i) {
		const CThostFtdcOrderField& order = orders[i];
		//if (mts::Environment::instance()->isCurrentInstanceReferenceId(order.OrderRef)) {
			_orders.append(order);
		//}else {
		//	MTS_ERROR("Receive out order:%s\n", qUtf8Printable (CTPUtils::toJsonString(&order)));
		//}
	}
	_queryOrderStatus = QS_DONE;
}

void CTPTradeClientBase::onResponseQueryTradingAccounts(const QList<CThostFtdcTradingAccountField>& tradingAccounts)
{
	_queryTradingAccountStatus = QS_DONE;
}

void CTPTradeClientBase::onErrorQueryQueryOrders(int errorId, const QString & errorMsg, OwnerLevel) {
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_queryOrderStatus = QS_ERROR;
}

void CTPTradeClientBase::onResponseQueryFills(const QList<CThostFtdcTradeField>& fills) {
	//MTS_DEBUG("\n");
	_fills = fills;
	_queryFillStatus = QS_DONE;
}

void CTPTradeClientBase::onErrorQueryQueryFills(int errorId, const QString & errorMsg, OwnerLevel) {
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_queryFillStatus = QS_ERROR;
}


void CTPTradeClientBase::onErrorQueryQueryTradingAccounts(int errorId, const QString& errorMsg, OwnerLevel)
{
	MTS_DEBUG("errorId=%d,errorMsg=%s\n", errorId, errorMsg.toUtf8().constData());
	_queryTradingAccountStatus = QS_ERROR;
}

void CTPTradeClientBase::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//MTS_DEBUG("(%p,%p,%d,%d)\n", pSettlementInfo, pRspInfo, nRequestID, bIsLast);
	_qrySettlementAction->OnRspSettlementInfoConfirm(pSettlementInfo, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeClientBase::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//MTS_DEBUG("(%p,%p,%d,%d)\n", pInvestorPosition, pRspInfo, nRequestID, bIsLast);
	_qryPosAction->OnRspQryInvestorPosition(pInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeClientBase::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (pOrder) {
		MTS_DEBUG("(nRequestID=%d,bIsLast=%d,%s,%s,%c)\n", nRequestID, bIsLast, pOrder->InstrumentID, pOrder->OrderRef, pOrder->OrderStatus);
	} else {
		MTS_DEBUG("(nRequestID=%d,bIsLast=%d)\n", nRequestID, bIsLast);
	}
	_qryOrderAction->OnRspQryOrder(pOrder, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeClientBase::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		MTS_DEBUG("(nRequestID=%d,bIsLast=%d)\n",nRequestID, bIsLast);
		_qryFillAction->OnRspQryTrade(pTrade, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeClientBase::OnRspQryInstrument(CThostFtdcInstrumentField * pInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast) {
	//MTS_DEBUG("(%p,%p,%d,%d)\n", pInstrument, pRspInfo, nRequestID, bIsLast);
	_qryInstrumentAction->OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
}


void CTPTradeClientBase::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	_qryTradingAccountAction->OnRspQryTradingAccount(pTradingAccount, pRspInfo, nRequestID, bIsLast);
}

void CTPTradeClientBase::doNextQuery() {
	{
		QMutexLocker l(&_functionsListMutex);
		if (_queryFunctionList.isEmpty()) {
			return;
		}

		//MTS_DEBUG("\n");
		QueryStatus* qryStatus = _queryFunctionList.first().status;
		if (*qryStatus == QS_READY || *qryStatus == QS_ERROR) {
			auto fun=_queryFunctionList.first().fun;
			if (fun()) {
				*qryStatus = QS_SENT;
			}
		}
		else if (*qryStatus == QS_DONE) {
			if (--_queryFunctionList.first().count > 0) {
				*qryStatus = QS_READY;
				_queryFunctionList.push_back(_queryFunctionList.first());
			}
			_queryFunctionList.removeFirst();
			doNextQuery(/*handle*/);
		} else { //QS_SENT
			//waitting 
		}
	}
	checkQueryStatus();
}

void CTPTradeClientBase::checkQueryStatus() {
	if (
		_querySettlementStatus==QS_DONE &&
		_queryInstrumentStatus == QS_DONE &&
		_queryPosStatus == QS_DONE&&
		_queryOrderStatus == QS_DONE&&
		_queryFillStatus == QS_DONE)
	{
		//assert(_queryFunctionList.isEmpty());
		if (_queryFunctionList.isEmpty()) {
			_queryTimer->stop();
		}
		if (!_isQueryFinished) {
			_isQueryFinished = true;
			MTS_DEBUG("\n");
			Q_EMIT loginDone();
		}
	}

}

bool CTPTradeClientBase::doAuthenticate() {
	return false;
}

int CTPTradeClientBase::subscribe() {
	return 0; //OK
}


void CTPTradeClientBase::onLoginSuccess() {
	MTS_DEBUG("onLoginSuccess\n");
	//PositionMgr::instance()->clear();
	this->startAsyncQueries();
}
