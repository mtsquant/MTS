
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
#include "base/Library.h"
#include "mts_core/enums.h"


namespace mts {
	class Environment;
	class Clock;
	class Feeds;
	class Trade;
	class  MTS_CORE_API EnvironmentFactory
	{
	public:
		static QString getLibraryFileName(const QString& componentName, const QString& mode);


		EnvironmentFactory(const QString& libraryPath);
		~EnvironmentFactory(void);

		bool loadTo(Environment*, EnvironmentMode);

	private:
		const QString _libraryPath;

	private:
		Clock* createClockComponent(EnvironmentMode);
		Feeds* createFeedsComponent(EnvironmentMode);
		Trade* createTradeComponent(EnvironmentMode);

		Library  _clockLib;
		Library _feedsLib;
		Library _tradeLib;
	};
}