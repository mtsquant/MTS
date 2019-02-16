
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
#include "mts_core/Account.h"
#include <boost/python.hpp>
#include "PyInstrumentProperty.h"




class PyAccount :	public mts::Account
{
public:
	PyAccount (const mts::Account &,const boost::python::list& activeOrders, const boost::python::list& positions);
	virtual ~PyAccount ();

	PyInstrumentProperty findInstrumentProperty ( const std::string& symbol );
	boost::python::list allActiveOrders() const;

	boost::python::list allPositions() const;
private:
	boost::python::list _activeOrders;
	boost::python::list _positions;
};
