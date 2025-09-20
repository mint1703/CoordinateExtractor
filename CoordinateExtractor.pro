QT += core network
CONFIG += c++17 cmdline
TARGET = CoordinateExtractor
TEMPLATE = app

# Добавляем поддержку UTF-8
QMAKE_CXXFLAGS += -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8
DEFINES += QT_USE_UTF8

SOURCES += \
    extractionresult.cpp \
    main.cpp \
    httpserver.cpp \
    coordinateparser.cpp \
    textprocessor.cpp \
    geometryanalyzer.cpp \
    coordinatevalidator.cpp \
    coordinate.cpp

HEADERS += \
    httpserver.h \
    coordinateparser.h \
    textprocessor.h \
    geometryanalyzer.h \
    coordinatevalidator.h \
    coordinate.h \
    extractionresult.h
