
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
#include "OrderReport.h"
#include "TradingAccount.h"

namespace mts {
	class MTS_CORE_API Order:public OrderReportNewDone
	{
	public:
		Order();
        Order(const Order&);
		virtual ~Order();
		virtual double leftVolume() const; //TODO check double
		int status()const;
		bool isFinished() const;

		void setStatus(int);

		virtual void processOrderActionNew(OrderActionNew*);
		virtual void processOrderReportNewDone(OrderReportNewDone*);
		virtual void processOrderReportNewReject(OrderReportNewReject*);

		virtual void processOrderActionCancel(OrderActionCancel*);
		virtual void processOrderReportCancelDone(OrderReportCancelDone*);
		virtual void processOrderReportCancelReject(OrderReportCancelReject*);


		virtual void processOrderReportFill(OrderReportFill* fillrpt,OrderReportFill* lastFillrpt);
		virtual Order* clone() const override;
		virtual QJsonObject& toJson(QJsonObject& jsonObj) const override;
		void copyTo(OrderReport*) const;
		void copyTo(Order*) const;
		void copyTo(OrderActionCancel*) const;
		QString statusString() const;
		static void registerMetaType();

		void setLastCancelSinceEpoch(qint64);
		qint64 lastCancelSinceEpoch()const;
	private:
		int _status;
		qint64 _lastCxlTimestamp;
	};

	typedef QSharedPointer<Order> OrderPtr;

}

Q_DECLARE_METATYPE(mts::OrderPtr);
