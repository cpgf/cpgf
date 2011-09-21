#-------------------------------------------------
#
# Project created by QtCreator 2010-12-24T21:08:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = RunTestCase
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../../../../src/testcase/reflection/property.cpp \
    ../../../../src/testcase/reflection/operator_unary.cpp \
    ../../../../src/testcase/reflection/operator_other.cpp \
    ../../../../src/testcase/reflection/operator_logic.cpp \
    ../../../../src/testcase/reflection/operator_bitwise_assign.cpp \
    ../../../../src/testcase/reflection/operator_bitwise.cpp \
    ../../../../src/testcase/reflection/operator_arithmetic_assign.cpp \
    ../../../../src/testcase/reflection/operator_arithmetic.cpp \
    ../../../../src/testcase/reflection/method.cpp \
    ../../../../src/testcase/reflection/inheritance_single.cpp \
    ../../../../src/testcase/reflection/field.cpp \
    ../../../../src/testcase/reflection/enum.cpp \
    ../../../../src/testcase/reflection/constructor.cpp \
    ../../../../src/testcase/reflection/annotation.cpp \
    ../../../../src/greflection.cpp \
    ../../../../src/gtype.cpp
INCLUDEPATH += d:/cxxtest;d:/projects/cpgf/include;d:/projects/cpgf/test/runtestcase
