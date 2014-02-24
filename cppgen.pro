TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    context.cpp

LIBS += -lboost_program_options

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    random_utils.h \
    context.h
