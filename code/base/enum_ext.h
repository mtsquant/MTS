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