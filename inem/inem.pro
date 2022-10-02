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

TEMPLATE = lib

CONFIG -= qt
STATIC_INEM {
    CONFIG += static
} else {
    CONFIG += shared
}

CONFIG += c++14

DEFINES += INEM_USE_OS_ESCAPE
DEFINES += MODEL_BUILD

unix {
    QMAKE_CFLAGS_DEBUG -= -O
    QMAKE_CFLAGS_DEBUG -= -O1
    QMAKE_CFLAGS_DEBUG -= -O2

    QMAKE_CFLAGS_DEBUG *= -O3

    QMAKE_CXXFLAGS_DEBUG -= -O
    QMAKE_CXXFLAGS_DEBUG -= -O1
    QMAKE_CXXFLAGS_DEBUG -= -O2

    QMAKE_CXXFLAGS_DEBUG *= -O3

    QMAKE_CFLAGS_RELEASE -= -O
    QMAKE_CFLAGS_RELEASE -= -O1
    QMAKE_CFLAGS_RELEASE -= -O2

    QMAKE_CFLAGS_RELEASE *= -O3

    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2

    QMAKE_CXXFLAGS_RELEASE *= -O3
}

unix:!macx {
    # GCC produces rather stupid warnings related to blackslashes at the EOL in comments.  Since we use backslashes to
    # describe operations on R-B trees in m_ordered_set.h, we end up with lots of these spurious and rather meaningless
    # warnings.
    #
    # For our sanity, we disable these warnings below.

    QMAKE_CXXFLAGS_WARN_OFF += -Wno-comment
    QMAKE_CXXFLAGS += -msse2
}

win32 {
    # Microsoft CL throws a number of warnings related to integer sizes that other compilers don't report.  For
    # consistency, we disable the warnings here.

    QMAKE_CXXFLAGS_WARN_ON  += -wd4244
    QMAKE_CXXFLAGS += /GR-
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG_BUILD
} else {
    DEFINES += RELEASE_BUILD
}

########################################################################################################################
# Third-Party Visible Public includes
#

INCLUDEPATH += customer_include
CUSTOMER_HEADERS = customer_include/model_common.h \
                   customer_include/model_intrinsic_types.h \
                   customer_include/model_api_types.h \
                   customer_include/model_exceptions.h \
                   customer_include/model_complex.h \
                   customer_include/model_range_iterator.h \
                   customer_include/model_range.h \
                   customer_include/model_tuple_iterator_base.h \
                   customer_include/model_tuple_const_iterator.h \
                   customer_include/model_tuple_iterator.h \
                   customer_include/model_tuple.h \
                   customer_include/model_set_iterator.h \
                   customer_include/model_set.h \
                   customer_include/model_matrix.h \
                   customer_include/model_matrix_boolean.h \
                   customer_include/model_matrix_integer.h \
                   customer_include/model_matrix_real.h \
                   customer_include/model_matrix_complex.h \
                   customer_include/model_variant.h \
                   customer_include/model_identifier_data.h \
                   customer_include/model_identifier_database_iterator.h \
                   customer_include/model_identifier_database.h \
                   customer_include/model_api.h \
                   customer_include/model_status.h \
                   customer_include/model_rng.h \

########################################################################################################################
# Public includes
#

INCLUDEPATH += include
API_HEADERS = include/m_common.h \
              include/m_os_escape.h \
              include/m_pragmas.h \
              include/m_intrinsics.h \
              include/m_intrinsic_types.h \
              include/m_complex_operators.h \
              include/m_api_types.h \
              include/m_type_conversion.h \
              include/m_type_conversion_template.h \
              include/m_exceptions.h \
              include/m_reference_counter.h \
              include/m_variant.h \
              include/m_variant_operators.h \
              include/m_variant_functions.h \
              include/m_unordered_set_base.h \
              include/m_unordered_set_entry.h \
              include/m_unordered_set_iterator.h \
              include/m_unordered_set.h \
              include/m_ordered_set_compare_function.h \
              include/m_ordered_set_entry.h \
              include/m_ordered_set_iterator.h \
              include/m_ordered_set.h \
              include/m_hash_functions.h \
              include/m_list_iterator.h \
              include/m_list_const_iterator.h \
              include/m_list.h \
              include/m_tuple.h \
              include/m_range.h \
              include/m_variant_ordered_set.h \
              include/m_set_iterator.h \
              include/m_set.h \
              include/m_matrix_iterator.h \
              include/m_matrix_boolean.h \
              include/m_matrix_integer.h \
              include/m_matrix_real.h \
              include/m_matrix_complex.h \
              include/m_implicit_ordering.h \
              include/m_special_values.h \
              include/m_assignment_functions.h \
              include/m_basic_functions.h \
              include/m_set_functions.h \
              include/m_boolean_functions.h \
              include/m_matrix_functions.h \
              include/m_statistical_functions.h \
              include/m_identifier_data.h \
              include/m_identifier_database.h \
              include/m_api.h \
              include/m_model_base.h \
              include/m_per_thread.h \
              include/m_trigonometric_functions.h \
              include/m_hyperbolic_functions.h \
              include/m_file_functions.h \
              include/m_summation_operator.h \
              include/m_console.h \

########################################################################################################################
# Source files
#

SOURCES = source/model_exceptions.cpp \
          source/model_complex.cpp \
          source/model_range_iterator.cpp \
          source/model_range_iterator_private.cpp \
          source/model_range.cpp \
          source/model_tuple_iterator_base.cpp \
          source/model_tuple_const_iterator.cpp \
          source/model_tuple_iterator.cpp \
          source/model_tuple_iterator_base_private.cpp \
          source/model_tuple.cpp \
          source/model_tuple_private.cpp \
          source/model_range_private.cpp \
          source/model_set_iterator.cpp \
          source/model_set_iterator_private.cpp \
          source/model_set.cpp \
          source/model_set_private.cpp \
          source/model_matrix_complex.cpp \
          source/model_matrix_real.cpp \
          source/model_matrix_integer.cpp \
          source/model_matrix_boolean.cpp \
          source/model_variant.cpp \
          source/model_identifier_data.cpp \
          source/model_identifier_data_private.cpp \
          source/model_identifier_database_iterator.cpp \
          source/model_identifier_database_iterator_private.cpp \
          source/model_identifier_database.cpp \
          source/model_identifier_database_private.cpp \
          source/model_status.cpp \
          source/m_type_conversion.cpp \
          source/m_exceptions.cpp \
          source/m_reference_counter.cpp \
          source/m_complex_operators.cpp \
          source/m_range.cpp \
          source/m_tuple.cpp \
          source/m_set_iterator.cpp \
          source/m_set.cpp \
          source/m_matrix_private_base.cpp \
          source/m_matrix_dense_private_base.cpp \
          source/m_matrix_complex_dense_data.cpp \
          source/m_matrix_complex.cpp \
          source/m_matrix_real_dense_data.cpp \
          source/m_matrix_real.cpp \
          source/m_matrix_integer_dense_data.cpp \
          source/m_matrix_integer.cpp \
          source/m_matrix_boolean_dense_data.cpp \
          source/m_matrix_boolean.cpp \
          source/m_implicit_ordering.cpp \
          source/m_matrix_functions.cpp \
          source/m_statistical_functions.cpp \
          source/m_special_values.cpp \
          source/m_basic_functions.cpp \
          source/m_set_functions.cpp \
          source/m_boolean_functions.cpp \
          source/m_assignment_functions.cpp \
          source/m_variant.cpp \
          source/m_variant_operators.cpp \
          source/m_variant_functions.cpp \
          source/m_unordered_set_base.cpp \
          source/m_hash_functions.cpp \
          source/m_identifier_data.cpp \
          source/m_identifier_database.cpp \
          source/m_api.cpp \
          source/m_api_private.cpp \
          source/m_model_base.cpp \
          source/m_model_base_private.cpp \
          source/m_per_thread.cpp \
          source/m_per_thread_rng_base.cpp \
          source/m_per_thread_mt19937.cpp \
          source/m_per_thread_mt216091.cpp \
          source/m_per_thread_xorshiro256_base.cpp \
          source/m_per_thread_xorshiro256_plus.cpp \
          source/m_per_thread_xorshiro256_stars.cpp \
          source/m_per_thread_trng.cpp \
          source/m_trigonometric_functions.cpp \
          source/m_hyperbolic_functions.cpp \
          source/m_file_functions.cpp \
          source/m_summation_operator.cpp \
          source/m_console.cpp \

########################################################################################################################
# Private includes
#

INCLUDEPATH += source
PRIVATE_HEADERS = source/m_compiler_abstraction.h \
                  source/mkl_include.h \
                  source/model_range_private.h \
                  source/model_range_iterator_private.h \
                  source/model_tuple_private.h \
                  source/model_tuple_iterator_base_private.h \
                  source/model_set_iterator_private.h \
                  source/model_set_private.h \
                  source/model_matrix_private.h \
                  source/model_matrix_boolean_private.h \
                  source/model_matrix_integer_private.h \
                  source/model_matrix_real_private.h \
                  source/model_matrix_complex_private.h \
                  source/model_variant_private.h \
                  source/model_identifier_data_private.h \
                  source/model_identifier_database_iterator_private.h \
                  source/model_identifier_database_private.h \
                  source/m_api_private.h \
                  source/m_matrix_helpers.h \
                  source/m_matrix_private_base.h \
                  source/m_matrix_dense_private_base.h \
                  source/m_matrix_dense_private.h \
                  source/m_matrix_sparse_private.h \
                  source/m_matrix_complex_data.h \
                  source/m_matrix_complex_dense_data.h \
                  source/m_matrix_complex_sparse_data.h \
                  source/m_matrix_real_data.h \
                  source/m_matrix_real_dense_data.h \
                  source/m_matrix_real_sparse_data.h \
                  source/m_matrix_integer_data.h \
                  source/m_matrix_integer_dense_data.h \
                  source/m_matrix_integer_sparse_data.h \
                  source/m_matrix_boolean_data.h \
                  source/m_matrix_boolean_dense_data.h \
                  source/m_matrix_boolean_sparse_data.h \
                  source/m_model_base_private.h \
                  source/m_per_thread_rng_base.h \
                  source/m_per_thread_mt19937.h \
                  source/m_per_thread_mt216091.h \
                  source/m_per_thread_xorshiro256_base.h \
                  source/m_per_thread_xorshiro256_plus.h \
                  source/m_per_thread_xorshiro256_stars.h \
                  source/m_per_thread_trng.h \

########################################################################################################################
# Windows runtime hooks.
#

defined(INEM_WINDOWS_RUNTIME_PRI, var) {
    include($${INEM_WINDOWS_RUNTIME_PRI})
}

########################################################################################################################
# Setup headers and installation
#

HEADERS = $$API_HEADERS $$CUSTOMER_HEADERS $$PRIVATE_HEADERS $$RUNTIME_HEADERS

########################################################################################################################
# Libraries
#

INCLUDEPATH += $${INEMAT_INCLUDE}
INCLUDEPATH += $${BOOST_INCLUDE}

########################################################################################################################
# Setup our symbol export capability. if needed.
#

defined(SYMBOL_EXPORT, var) {
    include($${SYMBOL_EXPORT})
}

########################################################################################################################
# Locate build intermediate and output products
#

TARGET = inem

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
