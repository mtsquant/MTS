
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
#include "Library.h"
#ifdef _WIN32
#include "MtsUtils.h"
#include <QtCore/QDir>

//for windows 7 and higher ,the dll serach order has been changed, the order of current dir  is the lowest priority insteads of highest.
// so we use the param LOAD_WITH_ALTERED_SEARCH_PATH 
//for linux , there are not the issue

Library::Library ( QObject * parent )
{
}

Library::Library(const QString &fileName, QObject *parent)
	:_fileName(fileName)
{
}


Library::~Library() {
	unload ();
}

QString Library::errorString () const{
	return _errorString;
}

QString Library::fileName() const {
	return _fileName;
}

bool Library::isLoaded () const{
	return _dllHandle!=NULL;
}

bool Library::load() {
	const QString nativeFilePath ( QDir::toNativeSeparators ( _fileName ) + ".dll" );
	_dllHandle=LoadLibraryEx( nativeFilePath.toStdWString().c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (_dllHandle == NULL) {
		_errorString = MtsUtils::getLastErrorMessage ();
		return false;
	} else {
		_errorString.clear ();
		return true;
	}

}

void Library::setFileName(const QString & fn) {
	_fileName = fn;
}

bool Library::unload (){
	if ( _dllHandle ==NULL){
		return false;
	}
	bool ret= FreeLibrary(_dllHandle);
	_dllHandle = NULL;
	return ret;
}


QFunctionPointer Library::resolve ( const char * symbol ){
	return  (QFunctionPointer)GetProcAddress ( _dllHandle , symbol );
}


#endif // 
