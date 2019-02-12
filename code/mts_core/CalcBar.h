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
#include "Bar.h"
#include "mts_core/Quote.h"
#include "mts_core/InstrumentObject.h"

#define MemberCalcBarCopyMethodDefine(Type,name,setName)	\
                                Type name() const{return _bar->name();}\
				void setName(const Type& t){_bar->setName(t);}


#define MemberCalcBarCopyReadOnlyMethodDefine(Type,name)	\
                                Type name() const{return _bar->name();}

namespace mts
{

	class MTS_CORE_API CalcBar:public InstrumentObject
	{
	public:
		CalcBar(const InstrumentId&,int interval,qint64 endTicks); // ǰ�պ�
		CalcBar(BarPtr,const InstrumentId&,int interval);
		CalcBar(int intervalSec=0);
		~CalcBar();

		qint64 beginTicksSinceEpoch() const;

		bool updateQuote(QuoteSnapshot*);  //return true, if the bar finished
		bool updateTime(qint64);  

		MemberCalcBarCopyMethodDefine(qint64, endTicksSinceEpoch, setEndTicksSinceEpoch); //����ʱ��
		MemberCalcBarCopyMethodDefine(double, openPrice, setOpenPrice);//���̼�
		MemberCalcBarCopyMethodDefine(double, highPrice, setHighPrice);//��߼۸�
		MemberCalcBarCopyMethodDefine(double, lowPrice, setLowPrice);//��ͼ۸�
		MemberCalcBarCopyMethodDefine(double, closePrice, setClosePrice);//���̼�

		MemberCalcBarCopyMethodDefine(double, volume, setVolume);//�ɽ����� //TODO check double
		MemberCalcBarCopyMethodDefine(double, turnover, setTurnover);
		double  vwap() const;
		MemberCalcBarCopyMethodDefine(double, openInterest, setOpenInterest);//�ֲ���
		MemberCalcBarCopyMethodDefine(double, fairPrice, setFairPrice);
		MemberCalcBarCopyMethodDefine(double, midPrice, setMidPrice);
		MemberCalcBarCopyMethodDefine(double, spread,setSpread);
		MemberCalcBarCopyMethodDefine(int, bidAskDepth,setBidAskDepth);


		bool fromJsonString(const QString&);
		QString toJsonString() const;

		bool fromCsvString(const QString& line, QChar sep = ',');
		//std::string toCsvString() const;

		BarPtr coreBar() const;

		MemberCopyMethodDefine(int, interval, setInterval);//barʱ���ȣ�60�룬300���
		void mergeFrom ( const CalcBar& );
	public:
		CalcBar* clone() const;
		//bool isUpdating() const {
		//	return _isUpdating;
		//}
		//void setUpdating(bool u) {
		//	_isUpdating = u;
		//}

		static void registerMetaType();
	private:
		BarPtr  _bar;
		//bool _isUpdating;
	};

	typedef QSharedPointer<CalcBar> CalcBarPtr;

}

Q_DECLARE_METATYPE(mts::CalcBarPtr);
