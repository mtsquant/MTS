
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
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/object.hpp>
#include <QtCore/QVariantMap>
#include <QtCore/QString>
#include <string>
#include "base/MtsLog.h"
#include "base/enum_ext.h"

class PythonUtil {
public:
		template<class _Ty>
		static QVariant pyExtract(_Ty obj) {
			boost::python::extract<int> intType(obj);
			if (intType.check()) {
				return (int)intType;
			}
			boost::python::extract<double> doubleType(obj);
			if (doubleType.check()) {
				return (double)doubleType;
			}
			boost::python::extract<std::string> strType(obj);
			if (strType.check()) {
				return QString::fromStdString((std::string)strType);
			}
			MTS_ERROR("MTS just support int/double/string of params dict\n");
			return QVariant();
		}


		static void dict2vmap(const boost::python::dict& params, QVariantMap* paramMap);

		static void printDict(const boost::python::dict& params);
		static boost::python::list toObject(const QStringList & strs);

		static boost::python::dict globals();

		template<typename F>
		static boost::python::object addFunction(const QString& fn, F f) {
			return addObject(qPrintable(fn), boost::python::make_function(f));
		}

		template<typename Class>
		boost::python::object addClass(const QString& fn, Class cls) {
			return addObject(fn, cls);
		}


		static boost::python::object addObject(const QString& objName, const boost::python::object& obj);
		template<typename Class>
		static boost::python::object addObject(const QString& objName, Class cls) {
			return addObject(objName, boost::python::object(cls));
		}

		static bool hasObject(const QString& objName);
		static boost::python::object getObject(const QString & objName);
		template<class _Ty>
		static _Ty& newObject(const QString& className) {
			auto globals = globals();
			return boost::python::extract<_Ty&>(boost::python::eval(qPrintable(className + "()"), globals, globals));
		}

		static int attrSize(const boost::python::object & obj);
		static bool hasAttr(const boost::python::object & obj, const QString & attrName);
		template<class _Ty>
		static void addAttr(boost::python::object& obj, const QString& attrName, const _Ty& value) {
			obj.attr(qPrintable(attrName)) = value;
		}
};

