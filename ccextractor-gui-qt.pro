#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T14:32:13
#
#-------------------------------------------------

QT	   += core gui widgets

TARGET = ccextractor-gui-qt
TEMPLATE = app

SOURCES += main.cpp\
	ccx-options.cpp \
	ccx-main-window.cpp \
    ccx-about.cpp

HEADERS  += \
	ccx-options.h \
	ccx-main-window.h \
    ccx-about.h

FORMS	+= \
	ccx-main-window.ui \
	ccx-options.ui \
    ccx-about.ui

RESOURCES += \
	ccx-gui-res.qrc
