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

#---
# define the test application executable
#
add_executable(Quickstart)

#---
# test application with test runner and supporting functions
#
target_sources(Quickstart
  PRIVATE
    app/clock_app.c
    app/clock_callbacks.c
    app/clock_spec.c
  )
target_include_directories(Quickstart
  PRIVATE
    app
)

#---
# specify the dependencies for this application
#
target_link_libraries(Quickstart Canopen CanopenDriver)
