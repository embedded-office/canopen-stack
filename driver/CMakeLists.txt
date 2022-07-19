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
# define the canopen stack library
#
add_library(CanopenDriver)

#---
# define interface
#
target_include_directories(CanopenDriver
  PUBLIC
    include
  )

#---
# specify the implementation files
#
target_sources(CanopenDriver
  PRIVATE
    source/co_can_dummy.c       # dummy CAN driver
    source/co_can_sim.c         # simulated CAN for testsuite
    #---
    source/co_timer_dummy.c     # dummy timer driver
    source/co_timer_swcycle.c   # timer based on cyclic service call
    #---
    source/co_nvm_dummy.c       # dummy nvm driver
    source/co_nvm_sim.c         # simulated nvm for testsuite
)

#---
# specify the dependencies for this application
#
target_link_libraries(CanopenDriver Canopen)
