
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

#include "mts_core/Feeds.h"
#include "CTPFeedsClient.h"

#include "base/EventDispatcher.h"


namespace mts
{

	class FeedsReal :public Feeds, public EventCallback
	{
		Q_OBJECT
	public:
		FeedsReal();
		virtual ~FeedsReal();

		virtual bool initialize(const QVariantMap & params) override;

		virtual int subscribe(const QList<InstrumentId>&)  override;
		virtual int unsubscribe(const QList<InstrumentId>&) override;
	private:
		CTPFeedsClient* _feedsClient;
		virtual void onEvent(const EventPtr&) override;
	};


}
