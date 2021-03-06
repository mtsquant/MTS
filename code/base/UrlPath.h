
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
#include "base/base_api.h"

class BASE_API UrlPath
{
public:
	UrlPath();
	UrlPath(const QString& url);
	~UrlPath();

	QString schema () const;
	bool isValid () const;
	QString toString () const;
	QString userName () const;
	QString password () const;

	QString setUserPassword(const QString& user, const QString& password);
	QString clearUserPassword();
	QString toLocalPath () const;
	QVariantMap queryItems () const;

	bool operator==( const UrlPath& ) const;

	static UrlPath fromLocalPath(const QString&);
private:
	QString _url;
};

