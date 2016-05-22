#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T19:29:42
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = DadaNAS
TEMPLATE = app


SOURCES += main.cpp\
        core.cpp \
    bash.cpp \
    toolsdialog.cpp

HEADERS  += core.h \
    bash.h \
    toolsdialog.h

FORMS    += core.ui \
    toolsdialog.ui

RESOURCES += \
    pictures.qrc
