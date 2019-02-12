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
#include<QtGlobal>
#include <QtCore/QSharedPointer>
#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>

#include "base/DateTime.h"
#include "mts_core/InstrumentId.h"
#include "mts_core/MethodDefine.h"

#include "mts_core/Quote.h"


#define MemberBarCopyMethodDefine(Type,name,setName)	public:\
				Type name() const;\
				void setName(const Type& t);\
				const char* name##FieldName() const {return #name;} \
				void name##ToJson(QJsonObject& jsonObj) const ; 


namespace mts {
	struct CoreBarT;
	class MTS_CORE_API Bar
	{
	public:
		Bar();
		~Bar();

		MemberBarCopyMethodDefine ( qint64 , endTicksSinceEpoch , setEndTicksSinceEpoch ); //����ʱ��
		MemberBarCopyMethodDefine (double, openPrice, setOpenPrice);//���̼�
		MemberBarCopyMethodDefine (double, highPrice, setHighPrice);//��߼۸�
		MemberBarCopyMethodDefine (double, lowPrice, setLowPrice);//��ͼ۸�
		MemberBarCopyMethodDefine (double, closePrice, setClosePrice);//���̼�

		MemberBarCopyMethodDefine (double, volume, setVolume);//�ɽ����� //TODO check double
		MemberBarCopyMethodDefine(double, turnover, setTurnover);//�ɽ����

		MemberBarCopyMethodDefine(double, openInterest, setOpenInterest);//�ֲ���
		MemberBarCopyMethodDefine ( double , fairPrice , setFairPrice );

		MemberBarCopyMethodDefine(double, midPrice, setMidPrice);
		MemberBarCopyMethodDefine(double, spread, setSpread);
		MemberBarCopyMethodDefine(int, bidAskDepth, setBidAskDepth);

		double vwap (double volumeMultiple) const;

		void mergeFrom ( const Bar& );
		Bar operator+( const Bar& ) const;
	public:
		Bar* clone() const;
		bool fromJson(const QJsonObject&);
		QJsonObject toJson() const;

		bool fromJsonString(const QString& jsonStr);
		QString toJsonString() const;

		QByteArray toByteArray() const;
		bool fromByteArray(const QByteArray&);

		bool updateQuote(QuoteSnapshot*, int barInterval);  //return true, if the bar finished

		static void registerMetaType();

		bool fromCsvString(const QString& line, QChar sep = ',');
		QString toCsvString() const;

		Bar* generateNextEmptyBar(qint64 timestamp) const;

		bool isEmpty() const;
	protected:

	private:
		QSharedPointer<CoreBarT> _coreBar;

		double _openInterest;
		double _openTotalVolume; //TODO check double
		double _openTrunover;

		double _currentTotalVolume; //TODO check double

	};

	typedef QSharedPointer<Bar> BarPtr;
}
Q_DECLARE_METATYPE(mts::BarPtr);
