##-*-makefile-*-########################################################################################################
# Copyright 2016 - 2022 Inesonic, LLC
#
# This file is licensed under two licenses.
#
# Inesonic Commercial License, Version 1:
#   All rights reserved.  Inesonic, LLC retains all rights to this software, including the right to relicense the
#   software in source or binary formats under different terms.  Unauthorized use under the terms of this license is
#   strictly prohibited.
#
# GNU Public License, Version 2:
#   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later
#   version.
#
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
#   details.
#
#   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
#   Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
########################################################################################################################

########################################################################################################################
# Basic build characteristics
#

TEMPLATE = app
QT += core testlib
CONFIG += testcase c++14

DEFINES += INEM_USE_OS_ESCAPE

unix:!macx {
    # GCC produces rather stupid warnings related to blackslashes at the EOL in comments.  Since we use backslashes to
    # describe operations on R-B trees in m_ordered_set.h, we end up with lots of these spurious and rather meaningless
    # warnings.  We disable these warnings below.

    QMAKE_CXXFLAGS_WARN_ON += -Wno-comment
}

HEADERS = test_reference_counter.h \
          test_variant_operators.h \
          test_variant_functions.h \
          test_file_functions.h \
          test_per_thread.h \
          test_boolean.h \
          test_complex.h \
          test_complex_operators.h \
          test_variant.h \
          test_list.h \
          test_unordered_set.h \
          test_ordered_set.h \
          test_model_range.h \
          test_m_range.h \
          test_model_tuple.h \
          test_m_tuple.h \
          test_set.h \
          test_matrix_base.h \
          test_matrix_boolean.h \
          test_matrix_integer.h \
          test_matrix_real.h \
          test_matrix_complex.h \
          test_implicit_ordering.h \
          test_matrix_random_functions.h \
          test_basic_functions.h \
          test_trigonometric_functions.h \
          test_hyperbolic_functions.h \
          test_set_functions.h \
          test_boolean_functions.h \
          test_statistical_functions.h \
          test_summation_operator.h \
          test_identifier_database.h \
          test_model_api.h \
          test_regressions.h \

SOURCES = test_inem.cpp \
          test_reference_counter.cpp \
          test_variant_operators.cpp \
          test_variant_functions.cpp \
          test_file_functions.cpp \
          test_per_thread.cpp \
          test_boolean.cpp \
          test_complex.cpp \
          test_complex_operators.cpp \
          test_variant.cpp \
          test_list.cpp \
          test_unordered_set.cpp \
          test_ordered_set.cpp \
          test_model_range.cpp \
          test_m_range.cpp \
          test_model_tuple.cpp \
          test_m_tuple.cpp \
          test_set.cpp \
          test_matrix_base.cpp \
          test_matrix_boolean.cpp \
          test_matrix_integer.cpp \
          test_matrix_real.cpp \
          test_matrix_complex.cpp \
          test_implicit_ordering.cpp \
          test_matrix_random_functions.cpp \
          test_basic_functions.cpp \
          test_trigonometric_functions.cpp \
          test_hyperbolic_functions.cpp \
          test_set_functions.cpp \
          test_boolean_functions.cpp \
          test_statistical_functions.cpp \
          test_summation_operator.cpp \
          test_identifier_database.cpp \
          test_model_api.cpp \
          test_regressions.cpp \

########################################################################################################################
# inem library:
#

INEM_BASE = $${OUT_PWD}/../inem/
INCLUDEPATH = $${PWD}/../inem/include/ $${PWD}/../inem/customer_include/

unix {
    CONFIG(debug, debug|release) {
        LIBS += -L$${INEM_BASE}/build/debug/ -linem

        macx {
            PRE_TARGETDEPS += $${INEM_BASE}/build/debug/libinem.dylib
        } else {
            PRE_TARGETDEPS += $${INEM_BASE}/build/debug/libinem.so
        }
    } else {
        LIBS += -L$${INEM_BASE}/build/release/ -linem

        macx {
            PRE_TARGETDEPS += $${INEM_BASE}/build/release/libinem.so
        } else {
            PRE_TARGETDEPS += $${INEM_BASE}/build/release/libinem.dylib
        }
    }
}

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $${INEM_BASE}/build/Debug/inem.lib
        PRE_TARGETDEPS += $${INEM_BASE}/build/Debug/inem.lib
    } else {
        LIBS += $${INEM_BASE}/build/Release/inem.lib
        PRE_TARGETDEPS += $${INEM_BASE}/build/Release/inem.lib
    }
}

########################################################################################################################
# Libraries
#

defined(SETTINGS_PRI, var) {
    include($${SETTINGS_PRI})
}

INCLUDEPATH += $${INEUTIL_INCLUDE}
INCLUDEPATH += $${INEMAT_INCLUDE}
INCLUDEPATH += $${BOOST_INCLUDE}

LIBS += -L$${INEUTIL_LIBDIR} -lineutil

defined(INEMAT_PRI, var) {
    include($${INEMAT_PRI})
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = test_inem

CONFIG(debug, debug|release) {
    unix:DESTDIR = build/debug
    win32:DESTDIR = build/Debug
} else {
    unix:DESTDIR = build/release
    win32:DESTDIR = build/Release
}

OBJECTS_DIR = $${DESTDIR}/objects
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui
