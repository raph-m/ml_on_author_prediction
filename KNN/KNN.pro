TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    knn.cpp \
    distances.cpp \
    dictionary.cpp \
    myknn_algo.cpp \
    main.cpp

HEADERS += \
    knn.h \
    distances.h \
    dictionary.h \
    myknn_algo.h
