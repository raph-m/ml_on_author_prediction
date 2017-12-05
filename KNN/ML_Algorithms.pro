TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    knn.cpp \
    distances.cpp

HEADERS += \
    knn.h \
    distances.h
