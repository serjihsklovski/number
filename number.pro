TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=c11

SOURCES += main.c \
    lib/cexception/CException.c \
    number.c \
    list_charptr.c

DISTFILES += \
    lib/cexception/LICENSE.txt \
    .gitignore

HEADERS += \
    lib/cclasses/cclasses.h \
    lib/cexception/CException.h \
    lib/template_list.h \
    error_codes.h \
    number.h \
    list_charptr.h
