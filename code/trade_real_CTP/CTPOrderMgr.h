
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

#include <QtCore/QString>
#include <QtCore/QHash>

#ifdef _WIN32
#include "../../api/win64/CTP/ThostFtdcTraderApi.h"
#else
#include "../../api/linux64/CTP/ThostFtdcTraderApi.h"
#include <float.h>
#endif

#include "CTPTradeClientBase.h"


#include "mts_core/enums.h"
#include "mts_core/OrderAction.h"
#include "mts_core/OrderReport.h"
#include "mts_core/Order.h"
#include "mts_core/Position.h"

struct ExchOrderInfo {
	ExchOrderInfo() 
		:tradingDay(0),sessionId(0),frontId(0),side(mts::D_UNKNOWN),status(mts::OS_CANCEL_REQUEST),
		sz(0),leftSz(0),fillSz(0),price(0.0), perfUs(0)
	{
	}

	int tradingDay;
	int sessionId;
	int frontId;
	int side;
	int status;
	int sz;
	int leftSz;
	int fillSz;
	double price;
	std::string clientId;//报单引用
	std::string exchOrderId;//报单编号
	std::string exch;
	std::string instrument;
	QString errMsg;
	qint64 perfUs;
};

Q_DECLARE_METATYPE(ExchOrderInfo);

class CTPOrderMgr : public CTPTradeClientBase
{
	Q_OBJECT
public:
	CTPOrderMgr(const QString& address, const QString& brokerId, const QString& userId, const QString& password/*,CThostFtdcTraderApi**/);
	virtual ~CTPOrderMgr();

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) override;

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade)  override;
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)  override;

	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);;
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) override;

	bool hasOrderClientId(const QString&) const;

	bool newOrder(mts::OrderActionNew* newAction);
	bool cancelOrder(mts::OrderActionCancel * cancelAction);

	static CThostFtdcOrderField convertInputOrder2OrderField(const CThostFtdcInputOrderField&);
Q_SIGNALS:
	void signalOrderReport(mts::OrderReportNewDone*);
	void signalOrderReport(mts::OrderReportNewReject*);
	void signalOrderReport(mts::OrderReportFill*);
	void signalOrderReport(mts::OrderReportCancelDone*);
	void signalOrderReport(mts::OrderReportCancelReject*);
	void signalRestoreOrder(mts::Order*);
	void signalRestorePosition(mts::Position*);

	void signalTradingAccount(mts::TradingAccount*);
	void signalOnRtnOrder(QByteArray order);
	void signalUpdateOrder(ExchOrderInfo info);
	void signalOnOrderFilled(QByteArray trade);
	void signalOnTradingAccount(QByteArray ta);
private Q_SLOTS: // the following will be called in main thread
	void onOrderFilled(QByteArray trade);
	void doRtnOrder(QByteArray order);
	void updateOrder(ExchOrderInfo info);
	void onTradingAccount(QByteArray);
	void onTradingAccount(CThostFtdcTradingAccountField* pTA);


	virtual void onResponseQueryInstruments(const QList<CThostFtdcInstrumentField>& instruments) override;
	virtual void onResponseQueryPositions(const QList<CThostFtdcInvestorPositionField>& positions) override;
	virtual void onResponseQueryOrders(const QList<CThostFtdcOrderField>& orders) override;
	virtual void onResponseQueryFills(const QList<CThostFtdcTradeField>& fills) override;
	virtual void onResponseQueryTradingAccounts(const QList<CThostFtdcTradingAccountField>& tradingAccounts) override;
private:
	void updateOrder(const CThostFtdcOrderField& pOrder);
	void removeOrder(const QString& clientId);
	void restoreOrdersAndFills();
	CTPOrderMgr& operator = (const CTPOrderMgr&);
	CTPOrderMgr(const CTPOrderMgr&);
	QHash<QString/*orderClientId*/, CThostFtdcOrderField> _allOrders;

	struct PositionInfo
	{
		PositionInfo()
			:yesterdayPosition(0), todayPosition(0), position(0)
		{
		}

		void init(const CThostFtdcInvestorPositionField& p) {
			this->yesterdayPosition = p.YdPosition;
			this->todayPosition = p.TodayPosition;
			this->position = p.Position;
		};

		int yesterdayPosition;
		int todayPosition;
		int position;
	};
	struct PositionDetail
	{
		PositionDetail()
		{
		}
		void update(bool isLongPos, bool isTodayPos, const CThostFtdcInvestorPositionField& p) {
			if (isLongPos) {
				if (isTodayPos) {
					this->longTodayPosition.init(p);
				}
				else {
					this->longYesterdayPosition.init(p);
				}
			}
			else {
				if (isTodayPos) {
					this->shortTodayPosition.init(p);
				}
				else {
					this->shortYesterdayPosition.init(p);
				}
			}
		}

		PositionInfo longTodayPosition;
		PositionInfo longYesterdayPosition;
		PositionInfo shortTodayPosition;
		PositionInfo shortYesterdayPosition;
	};
};

