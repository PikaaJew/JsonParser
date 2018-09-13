TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    error.c \
    methods.c \
    parse.c \
    freejson.c

HEADERS += \
    error.h \
    methods.h \
    parse.h \
    freejson.h \
    tree.h
