
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
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <boost/function.hpp>

#include "ctp_ext/CTPClientCommon.h"

#include "mts_core/const.h"
#include "mts_core/Order.h"

class QueryPositionAction;
class QueryOrderAction;
class QueryFillAction;
class QueryInstrumentAction;
class QuerySettlementInfoAction;
class QueryTradingAccountAction;

class CTPTradeClientBase:public QObject, public CTPClientCommon<CThostFtdcTraderApi, CThostFtdcTraderSpi>
{
	Q_OBJECT
public:
	CTPTradeClientBase(	const QString& address, 
									const QString& brokerId, 
									const QString& userId, 
									const QString& password);
	virtual ~CTPTradeClientBase();
	virtual QString name() const override {
		return "CTPTradeClient";
	}


	bool querySettlement();
	bool queryPositions();
	bool queryOrders();
	bool queryFills();
	bool queryInstruments();
	bool queryTradingAccount();

	const QList<CThostFtdcSettlementInfoConfirmField>& settlementInfos();
	const QList<CThostFtdcInvestorPositionField>& positions();
	const QList<CThostFtdcOrderField>& orders();
	const QList<CThostFtdcTradeField>& fills();
	const QList<CThostFtdcInstrumentField>& instruments();

	bool isQueryFinished();
	enum QueryStatus
	{
		QS_READY = 0,
		QS_SENT,
		QS_ERROR,
		QS_DONE
	};

	int frontId() const;
	int sessionId() const;

Q_SIGNALS:
	void loginDone();
	void loginError(const QString&);


protected:
	virtual void beforeRegisterFront() override;

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
protected Q_SLOTS:
	void onResponseQuerySettlementInfo(const QList<CThostFtdcSettlementInfoConfirmField>& settlementInfos);
	virtual void onResponseQueryInstruments(const QList<CThostFtdcInstrumentField>& instruments);
	virtual void onResponseQueryPositions(const QList<CThostFtdcInvestorPositionField>& positions);
	virtual void onResponseQueryOrders(const QList<CThostFtdcOrderField>& orders);
	virtual void onResponseQueryFills(const QList<CThostFtdcTradeField>& fills);
	virtual void onResponseQueryTradingAccounts(const QList<CThostFtdcTradingAccountField>& tradingAccounts);


	void onErrorQuerySettlementInfo(int  errorId, const QString& errorMsg, OwnerLevel);
	void onErrorQueryInstruments(int  errorId, const QString& errorMsg, OwnerLevel);
	void onErrorQueryQueryPositions(int  errorId, const QString& errorMsg, OwnerLevel);
	void onErrorQueryQueryOrders(int  errorId, const QString& errorMsg, OwnerLevel);
	void onErrorQueryQueryFills(int  errorId, const QString& errorMsg, OwnerLevel);
	void onErrorQueryQueryTradingAccounts(int  errorId, const QString& errorMsg, OwnerLevel);

	void doNextQuery();
private:
	void checkQueryStatus();
	void startAsyncQueries(); //It will be called on CTP Thread
	virtual int subscribe() override;
	virtual void onLoginSuccess() override;
	virtual bool doAuthenticate() override;
private:
	QuerySettlementInfoAction* _qrySettlementAction;
	QueryInstrumentAction* _qryInstrumentAction;
	QueryPositionAction* _qryPosAction;
	QueryOrderAction* _qryOrderAction;
	QueryFillAction* _qryFillAction;
	QueryTradingAccountAction* _qryTradingAccountAction;

	QTimer* _queryTimer;
	typedef boost::function<bool()> QueryFunction;
	struct QueryInfo {
		QueryFunction fun;
		QueryStatus* status;
		qint64 count;
	};
	QList<QueryInfo> _queryFunctionList;
	QMutex _functionsListMutex;

	QueryStatus _querySettlementStatus;
	QueryStatus _queryInstrumentStatus;
	QueryStatus _queryPosStatus;
	QueryStatus _queryOrderStatus;
	QueryStatus _queryFillStatus;
	QueryStatus _queryTradingAccountStatus;
	bool _isQueryFinished;

protected:
	QList<CThostFtdcSettlementInfoConfirmField> _settlementInfos;
	QList<CThostFtdcInvestorPositionField> _positions;
	QList<CThostFtdcOrderField> _orders;
	QList<CThostFtdcTradeField> _fills;
	QList<CThostFtdcInstrumentField> _instruments;
};

