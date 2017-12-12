TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += \
    dataset.cpp \
    randomtree.cpp \
    testDataPrep.cpp \
    randomforest.cpp \
    testRandom.cpp \
    multiclass_rs.cpp \
    main.cpp

HEADERS += \
    dataset.h \
    matrix.h \
    probamap.h \
    column.h \
    node.h \
    randomtree.h \
    leafnode.h \
    splitnode.h \
    testDataPrep.h \
    testRandom.h \
    randomforest.h \
    multiclass_rs.h
