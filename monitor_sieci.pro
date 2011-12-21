# -------------------------------------------------
# Project created by QtCreator 2010-06-30T21:09:54
# -------------------------------------------------
TARGET = monitor_sieci
TEMPLATE = app
SOURCES += Mutex.cpp \
    MessageFlow.cpp \
    MessageObject.cpp \
    main.cpp \
    mainwindow.cpp \
    ping.cpp \
    host.cpp
HEADERS += Mutex.h \
    MessageFlow.h \
    MessageObject.h \
    mainwindow.h \
    ping.h \
    host.h
FORMS += mainwindow.ui
RESOURCES += zasoby.qrc
QT += network
