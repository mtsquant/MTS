
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
#include <QtCore/QList>
#include <QtCore/QVariant>
#include "Component.h"
#include "mts_core/InstrumentId.h"
#include "FeedsCallback.h"
#include "DateCallback.h"

namespace mts
{
	class FeedsNotifier;
	class MTS_CORE_API FeedsBase :public Component
	{
	public:
		FeedsBase();
		virtual ~FeedsBase();
		virtual int subscribe(const QList<InstrumentId>&) = 0;
		virtual int unsubscribe(const QList<InstrumentId>&) = 0;


		virtual int addCallback(FeedsCallback* cb) =0;
		virtual int addFrontCallback(FeedsCallback* cb) = 0; //just for simu trade for add the match model
		virtual int findCallback(FeedsCallback* cb) = 0;
		virtual bool hasCallback(FeedsCallback* cb) = 0;
		virtual bool removeCallback(FeedsCallback* cb) =0;

	};

	class MTS_CORE_API Feeds :public FeedsBase,public DateCallback
	{
	public:
		Feeds();
		virtual ~Feeds(void);

		virtual int addCallback(FeedsCallback* cb) override;
		virtual int addFrontCallback(FeedsCallback* cb) override; //just for simu trade for add the match model
		virtual int findCallback(FeedsCallback* cb) override;
		virtual bool hasCallback(FeedsCallback* cb) override;
		virtual bool removeCallback(FeedsCallback* cb) override;

	protected:
		virtual void onBusinessDateChanged(int businessDate) override;
		void notifyAll(QuotePtr qt);
		virtual void notifyAll(CalcBarPtr bar);
		void notifyOne(int callbackIdx, CalcBarPtr bar);
		int callbacksCount() const;
	private:
		QList<FeedsCallback*> _callbacks;
	};


}
