TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += \
    dataset.cpp \
    randomtree.cpp \
    randomforest.cpp \
    main2.cpp \
    testDataPrep.cpp \
    testRandom.cpp

HEADERS += \
    dataset.h \
    matrix.h \
    probamap.h \
    column.h \
    node.h \
    randomtree.h \
    leafnode.h \
    splitnode.h \
    randomforest.h \
    testDataPrep.h \
    testRandom.h
