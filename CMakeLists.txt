#/*****************************************************************************
#   Copyright 2020 Embedded Office GmbH & Co. KG
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#******************************************************************************

# Preamble
cmake_minimum_required(VERSION 3.15)
project(CanopenStack C ASM)
enable_testing()

# Project wide setup
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(c-standards)
include(code-coverage)
# require standard C99
c_99()
# enable coverage measurement over all test executables
add_code_coverage_all_targets(EXCLUDE config/* include/* tests/*)

# Main targets built by this project
add_subdirectory(canopen)
add_subdirectory(driver)

# Typically needed only if we are the top level project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
add_subdirectory(tests)      # unit tests (object type modules)
add_subdirectory(testsuite)  # integration tests (module interactions)
add_subdirectory(example)    # validation tests (demo apps)
endif()
