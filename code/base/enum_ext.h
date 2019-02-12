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

#define SELECT_1_IN_2(p1,p2)		p1
#define SELECT_1_AND_COMMA_IN_2(p1,p2)	p1,

#define SELECT_2_IN_2(p1,p2)		p2
#define SELECT_2_AND_COMMA_IN_2(p1,p2)	p2,



#define SELECT_1_IN_3(p1,p2,p3)		p1
#define SELECT_1_AND_COMMA_IN_3(p1,p2,p3)	p1,

#define SELECT_2_IN_3(p1,p2,p3)		p2
#define SELECT_2_AND_COMMA_IN_3(p1,p2,p3)	p2,

#define SELECT_3_IN_3(p1,p2,p3)		p3
#define SELECT_3_AND_COMMA_IN_3(p1,p2,p3)	p3,



#define SELECT_1_IN_4(p1,p2,p3,p4)		p1
#define SELECT_1_AND_COMMA_IN_4(p1,p2,p3,p4)	p1,

#define SELECT_2_IN_4(p1,p2,p3,p4)		p2
#define SELECT_2_AND_COMMA_IN_4(p1,p2,p3,p4)	p2,

#define SELECT_3_IN_4(p1,p2,p3,p4)		p3
#define SELECT_3_AND_COMMA_IN_4(p1,p2,p3,p4)	p3,

#define SELECT_4_IN_4(p1,p2,p3,p4)		p4
#define SELECT_4_AND_COMMA_IN_4(p1,p2,p3,p4)	p4,

#define SELECT_1_IN_4_DEFAULT(p1,p2,p3,p4)		p1=p4
#define SELECT_1_AND_COMMA_IN_4_DEFAULT(p1,p2,p3,p4)	p1=p4,


#define SELECT_1_2_AND_COMMA_IN_4(p1,p2,p3,p4)	p1=p2,
#define SELECT_3_2_AND_COMMA_IN_4(p1,p2,p3,p4)	p3,
#define SELECT_4_2_AND_COMMA_IN_4(p1,p2,p3,p4)	p4,

#define ARRAY_SIZE(a)							(sizeof(a)/sizeof(a[0]))

#define DEFINE_NAME_ENUM_MAP3_KEY_2(p1,p2,p3)  map[p2]=p1;
#define DEFINE_NAME_ENUM_MAP3_KEY_3(p1,p2,p3)  map[p3]=p1;
#define DEFINE_NAME_ENUM_MAP4_KEY_2(p1,p2,p3,p4)  map[p2]=p1;
#define DEFINE_NAME_ENUM_MAP4_KEY_3(p1,p2,p3,p4)  map[p3]=p1;
#define DEFINE_NAME_ENUM_MAP4_KEY_4(p1,p2,p3,p4)  map[p4]=p1;

#define DEFINE_NAME_ENUM_MAP4_KEY_1_3(p1,p2,p3,p4)  map[p1]=p3;
#define DEFINE_NAME_ENUM_MAP4_KEY_1_4(p1,p2,p3,p4)  map[p1]=p4;
#define DEFINE_NAME_ENUM_MAP4_KEY_3_1(p1,p2,p3,p4)  map[p3]=p1;

#define LS(s) QString::fromLocal8Bit(s)
#define UTFS(s) QString::fromUtf8(s)
#define TO_UTF8_STD_STR(s) s.toUtf8().toStdString()