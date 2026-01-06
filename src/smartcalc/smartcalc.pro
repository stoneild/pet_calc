#TEMPLATE = app
#CONFIG += console c++17
#CONFIG -= app_bundle
#CONFIG -= qt
QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
        main.cpp \
        mbutton.cpp \
        plotwindow.cpp \
        qcustomplot.cpp \
        smartcalc.cpp


INCLUDEPATH += $$PWD/lib
LIBS += -L$$PWD/lib -ls21_calc

HEADERS += \
        mbutton.h \
        plotwindow.h \
        qcustomplot.h \
        smartcalc.h
