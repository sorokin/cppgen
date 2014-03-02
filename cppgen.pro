TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    context.cpp \
    gram.basic.cpp \
    gram.dcl.cpp \
    gram.decl.cpp \
    gram.class.cpp \
    gram.lex.cpp \
    gram.stmt.cpp \
    gram.expr.cpp

LIBS += -lboost_program_options -lboost_random

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    random_utils.h \
    context.h \
    gram.basic.h \
    gram.dcl.h \
    gram.decl.h \
    gram.class.h \
    gram.lex.h \
    gram.stmt.h \
    gram.expr.h

OTHER_FILES +=
