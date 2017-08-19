TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    network.cpp \
    linear.cpp \
    graph.cpp \
    operations.cpp

HEADERS += \
    network.h \
    linear.h \
    operations.h \
    graph.h \
    vector_operations.h \
    function.h \
    activations.h
