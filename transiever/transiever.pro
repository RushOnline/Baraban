QT += core
QT += gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = baraban
TEMPLATE = app

SOURCES += \
    main.cpp\
    mainwindow.cpp

HEADERS  += \
    mainwindow.h

FORMS += \
    mainwindow.ui


RESOURCES +=

OTHER_FILES += 

include(../pods.pri)
