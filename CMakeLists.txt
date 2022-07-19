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

cmake_minimum_required(VERSION 3.15)
project(CanopenStack)
enable_language(C ASM)

# Make CANopen library
add_subdirectory(canopen)
add_subdirectory(driver)

# Select Executable with CMAKE variable: `APP`
# (1) no -DAPP in commandline  => testsuite (default)
# (2) -DAPP:STRING=quickstart  => example/quickstart
# (3) -DAPP:STRING=dynamic-od  => example/dynamic-od
if (DEFINED APP)
add_subdirectory(example)
else()
add_subdirectory(testsuite)
endif()
