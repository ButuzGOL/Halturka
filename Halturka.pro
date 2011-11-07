#-------------------------------------------------
#
# Project created by QtCreator 2010-03-16T21:21:30
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Halturka
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    models/customermodel.cpp \
    models/workermodel.cpp \
    models/customerform.cpp \
    models/workerform.cpp \
    models/workmodel.cpp \
    models/workform.cpp \
    models/paymentmodel.cpp \
    models/paymentform.cpp \
    models/paymentworkerform.cpp \
    preferencedialog.cpp \
    models/statistikmodel.cpp

HEADERS  += mainwindow.h\
        models/customermodel.h \
    connection.h \
    models/workermodel.h \
    models/customerform.h \
    models/workerform.h \
    models/workmodel.h \
    models/workform.h \
    models/materialmodel.h \
    models/paymentmodel.h \
    models/paymentform.h \
    models/paymentworkerform.h \
    preferencedialog.h \
    models/statistikmodel.h

FORMS    += mainwindow.ui \
    models/customerform.ui \
    models/workerform.ui \
    models/workform.ui \
    models/paymentform.ui \
    models/paymentworkerform.ui \
    models/statistikmodel.ui

RESOURCES += \
    halturka.qrc
