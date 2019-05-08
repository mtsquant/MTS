
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
#include <QtCore/QString>
#include <QtCore/QVariantMap>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include "mts_core/enums.h"
#include "base/DateTime.h"
#include "base/UrlPath.h"
#include "SysConfigParams.h"


namespace mts
{
	enum QuoteGrade
	{
		QG_TICK						= 0,
		QG_BAR_15SEC				= 15,
		QG_BAR_15SEC_ONLY	= -15,
		QG_BAR_30SEC				= 30,
		QG_BAR_30SEC_ONLY	= -30,
		QG_BAR_1MIN				= 1 * 60,
		QG_BAR_1MIN_ONLY		= -1 * 60,
		QG_BAR_5MIN				= 5 * 60,
		QG_BAR_5MIN_ONLY		= -5 * 60,
		QG_BAR_15MIN				= 15 * 60,
		QG_BAR_15MIN_ONLY	= -15 * 60
	};


	class MTS_CORE_API CommonConfigParams :public SysConfigParams
	{
	protected:
		CommonConfigParams();
		virtual ~CommonConfigParams();
		virtual bool doLoad(const QVariantMap & params) override;
	public:
		QStringList allExchSessions() const;
		UrlPath feedsFront(const QString& exchSession) const;
		UrlPath tradeFront(const QString& exchSession) const;
		FrontProtocol feedsFrontProtocol(const QString& exchSession ) const;
		FrontProtocol tradeFrontProtocol(const QString& exchSession ) const;

		int feedsGrade() const;
		FeedsType feedsType() const;

		UrlPath indicator() const;

		QString marketTradingTimeType() const;  /*'24H' or not*/
	protected:
		QString brokerId(const QString& exchSession ) const;
	private:
		struct Front {
			UrlPath _feedsFront;
			UrlPath _tradeFront;
			FrontProtocol _tradeFrontProtocol;
			FrontProtocol _feedsFrontProtocol;
			QString _brokerId;
		};
		bool readExchParamsSection(const QVariantMap & params, Front& front);

		QMap<QString/*exchSession*/, Front>  _fronts;
		int _feedsGrade;
		UrlPath _indicator;
		QString _mktTradingTimeType;
	};

	class MTS_CORE_API ConfigParams :public CommonConfigParams
	{
		ConfigParams();
		virtual ~ConfigParams();
	public:
		static ConfigParams* instance();
		static bool isInitialized();

		int feedsBeginTime() const;
		int beginTradingDate() const;
		int endTradingDate() const;
		qint64 beginUTCTicksSinceEpoch() const;
		qint64 endUTCTicksSinceEpoch() const;

		using CommonConfigParams::brokerId;
	protected:
		virtual bool doLoad(const QVariantMap & params) override;
	private:
		static bool _isInitialized;

		int _feedsBeginTime;
		int _beginTradingDate;
		int _endTradingDate;
		qint64 _beginUTCTicksSinceEpoch;
		qint64 _endUTCTicksSinceEpoch;
	};




}
