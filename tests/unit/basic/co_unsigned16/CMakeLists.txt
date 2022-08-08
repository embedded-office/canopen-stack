#******************************************************************************
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

add_executable(co_unsigned16 main.c)
target_link_libraries(co_unsigned16 Canopen TestEnv)
install(TARGETS co_unsigned16 RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/unit)

#--- type function interface tests ---

add_test(NAME unit/co_unsigned16/size/unknown   COMMAND co_unsigned16 size_unknown  )
add_test(NAME unit/co_unsigned16/size/known     COMMAND co_unsigned16 size_known    )
add_test(NAME unit/co_unsigned16/size/bad_size  COMMAND co_unsigned16 bad_size      )
add_test(NAME unit/co_unsigned16/read/ref       COMMAND co_unsigned16 read_ref      )
add_test(NAME unit/co_unsigned16/read/direct    COMMAND co_unsigned16 read_direct   )
add_test(NAME unit/co_unsigned16/read/nodeid    COMMAND co_unsigned16 read_nodeid   )
add_test(NAME unit/co_unsigned16/read/bad_node  COMMAND co_unsigned16 read_bad_node )
add_test(NAME unit/co_unsigned16/write/ref      COMMAND co_unsigned16 write_ref     )
add_test(NAME unit/co_unsigned16/write/direct   COMMAND co_unsigned16 write_direct  )
add_test(NAME unit/co_unsigned16/write/nodeid   COMMAND co_unsigned16 write_nodeid  )
add_test(NAME unit/co_unsigned16/write/bad_node COMMAND co_unsigned16 write_bad_node)
add_test(NAME unit/co_unsigned16/init/unused    COMMAND co_unsigned16 init_unused   )
add_test(NAME unit/co_unsigned16/reset/unused   COMMAND co_unsigned16 reset_unused  )
