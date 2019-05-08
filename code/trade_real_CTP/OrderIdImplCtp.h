
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

namespace mts
{

	class OrderIdImplCtp
	{
	public:
		OrderIdImplCtp();
		OrderIdImplCtp(int frontId, int sessionId, int instanceId, int strategyId, int orderType,int index);
		OrderIdImplCtp( int frontId , int sessionId , qint64);
		OrderIdImplCtp(const QString& id);
		~OrderIdImplCtp();

		int orderType() const;
		int instanceId() const;
		int strategyId() const;
		bool isMtsOrder() const;
		QString toString() const;
		bool fromString(const QString & idStr);
		QString ctpOrderReferenceId() const;
		QString id()const;
	private:
		int _frontId;
		int _sessionId;
		qint64 _refId;



	};

}
