#-------------------------------------------------
#
# Project created by QtCreator 2017-06-14T21:46:07
#
#-------------------------------------------------
DEFINES += RIGHTS_END_DATE=20190601


QT       -= gui

TARGET = base
TEMPLATE = lib

DEFINES += BASE_LIB

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Currency.cpp \
    Timezone.cpp \
    DateTime.cpp \
    Event.cpp \
    EventDispatcher.cpp \
    EventQueue.cpp \
    MtsLog.cpp \
    MtsUtils.cpp \
    UrlPath.cpp \
    LogFixtureMgr.cpp \
    MtsPath.cpp \
    Library.cpp \
    MtsType.cpp \

HEADERS += \
    base_api.h \
    Currency.h \
    enum_ext.h \
    Timezone.h \
    DateTime.h \
    NotifiableObserver.h \
    Event.h \
    EventDispatcher.h \
    EventQueue.h \
    EventType.h \
    MtsLog.h \
    MtsUtils.h \
    number.h \
    PerfTest.h \
    UrlPath.h \
    LogFixtureMgr.h \
    MtsPath.h \
    Library.h \
    MtsType.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    qt_res.qrc

include (../global.pri)
LIBS += -lz
