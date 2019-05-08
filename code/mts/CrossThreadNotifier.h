
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
#pragma once
#include "mts_core/StrategyInterface.h"
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include "mts_api.h"

using namespace mts;

namespace mts
{
	class MTS_API CrossThreadNotifier :public QObject, public StrategyInterface
	{
		Q_OBJECT
	public:
		CrossThreadNotifier(const std::string& name, int strategyId,const std::string& strategyPath=std::string());
		virtual ~CrossThreadNotifier();

		virtual std::string name () const override{
			return _name;
		}
		virtual std::string strategyPath() const override {
			return _strategyPath;
		}
		virtual int strategyId () const override{
			return _strategyId;
		}

		template<typename _Ty>
		void connectSignalInitializedl(_Ty* observer) {
			connect(this, &CrossThreadNotifier::envirInitialized, observer, &_Ty::doEnvirInitialized, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
			connect(this, &CrossThreadNotifier::initialized, observer, &_Ty::doInitialized, Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection ));
		}

		template<typename _Ty>
		void connectSignals(_Ty* observer) {
			const Qt::ConnectionType  ct = connectionType();
			connect(this, &CrossThreadNotifier::timeout,							observer, &_Ty::doTimeout,					ct);
			connect(this, &CrossThreadNotifier::quoteSnapshotUpdate,		observer, &_Ty::doQuoteSnapshotUpdate,ct );
			connect(this, &CrossThreadNotifier::orderNewDone,				observer, &_Ty::doOrderNewDone,			ct );
			connect(this, &CrossThreadNotifier::orderNewReject,				observer, &_Ty::doOrderNewReject,			ct );
			connect(this, &CrossThreadNotifier::orderFill,						observer, &_Ty::doOrderFill,					ct);
			connect(this, &CrossThreadNotifier::orderCancelDone,			observer, &_Ty::doOrderCancelDone,		ct );
			connect(this, &CrossThreadNotifier::orderCancelReject,			observer, &_Ty::doOrderCancelReject,		ct );
			connect(this, &CrossThreadNotifier::orderOtherReport,			observer, &_Ty::doOrderOtherReport,		ct );
			connect(this, &CrossThreadNotifier::positionUpdate,				observer, &_Ty::doPositionUpdate,			ct );
			connect(this, &CrossThreadNotifier::orderUpdate,					observer, &_Ty::doOrderUpdate,				ct );
			connect(this, &CrossThreadNotifier::businessDateChanged,	observer, &_Ty::doBusinessDateChanged,		ct );
			connect(this, &CrossThreadNotifier::barUpdate,					observer, &_Ty::doBarUpdate,					ct );

		}

	Q_SIGNALS: //main thread use the signal 
		void envirInitialized();
		void initialized(AccountPtr);
		void timeout(int);
		void quoteSnapshotUpdate(QuoteSnapshotPtr);
		void orderNewDone(OrderReportNewDonePtr);
		void orderNewReject(OrderReportNewRejectPtr);
		void orderFill(OrderReportFillPtr);
		void orderCancelDone(OrderReportCancelDonePtr);
		void orderCancelReject(OrderReportCancelRejectPtr);
		void orderOtherReport(OrderReportPtr);
		void positionUpdate(PositionPtr);
		void orderUpdate(OrderPtr);
		void businessDateChanged(int);
		void barUpdate(CalcBarPtr);

	public: // for Agent to call on onInitalized()
		virtual void onBusinessDateChanged(int) override;
	private:
		virtual void onEnvirInitialized() override;
		virtual void onInitialized(Account *) override;
		virtual void onTimeout(int) override;

		virtual void onQuoteSnapshotUpdate(QuoteSnapshotPtr) override;

		virtual void onOrderNewDone(OrderReportNewDone *) override;
		virtual void onOrderNewReject(OrderReportNewReject *) override;
		virtual void onOrderFill(OrderReportFill *) override;
		virtual void onOrderCancelDone(OrderReportCancelDone *) override;
		virtual void onOrderCancelReject(OrderReportCancelReject *) override;
		virtual void onOrderOtherReport(OrderReport *) override;
		virtual void onPositionUpdate(Position *) override;
		virtual void onOrderUpdate(Order*)override;

		virtual void onBarUpdate(CalcBarPtr) override;
	private:
		virtual Qt::ConnectionType  connectionType() const;

		const std::string _name;
		const std::string _strategyPath;
		const int _strategyId;
	};

}
