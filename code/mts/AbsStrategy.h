
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
#include <QtCore/QObject>
#include "mts/mts_api.h"
#include "mts_core/Account.h"
#include "mts_core/OrderAction.h"
#include "mts_core/Quote.h"
#include "mts_core/Position.h"
#include "mts_core/Order.h"
#include "mts_core/CalcBar.h"

namespace mts{
    class CrossThreadNotifier;
}

using namespace mts;

class MTS_API AbsStrategy : public QObject
{
	Q_OBJECT
    public:
        AbsStrategy(const std::string& name , int strategyId, const std::string& strategyPath);
        virtual ~AbsStrategy();

		virtual int newTimer(int beginTime, int interval); //return timer id
		virtual int newOnceTimer(int beginTime); //return timer id
		virtual bool removeTimer(int id);

    private:
        friend class mts::CrossThreadNotifier;
	protected:
        CrossThreadNotifier* _notifier;

	private Q_SLOTS:
		virtual void doEnvirInitialized() =0;
		virtual void doInitialized(AccountPtr)  =0;
		virtual void doTimeout(int)=0;
		virtual void doQuoteSnapshotUpdate(QuoteSnapshotPtr)=0;
		virtual void doOrderNewDone(OrderReportNewDonePtr)=0;
		virtual void doOrderNewReject(OrderReportNewRejectPtr)=0;
		virtual void doOrderFill(OrderReportFillPtr)=0;
		virtual void doOrderCancelDone(OrderReportCancelDonePtr)=0;
		virtual void doOrderCancelReject(OrderReportCancelRejectPtr)=0;
		virtual void doOrderOtherReport(OrderReportPtr)=0;
		virtual void doPositionUpdate(PositionPtr)=0;
		virtual void doOrderUpdate(OrderPtr)=0;
		virtual void doBusinessDateChanged(int)=0;
		virtual void doBarUpdate(CalcBarPtr)=0;

};