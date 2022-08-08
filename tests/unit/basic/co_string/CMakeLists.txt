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

add_executable(co_string main.c)
target_link_libraries(co_string Canopen TestEnv)


#--- type function interface tests ---

add_test(NAME unit/co_string/size/unknown   COMMAND co_string size_unknown  )
add_test(NAME unit/co_string/size/known     COMMAND co_string size_known    )
add_test(NAME unit/co_string/size/bad_size  COMMAND co_string bad_size      )
add_test(NAME unit/co_string/read/ref       COMMAND co_string read_ref      )
add_test(NAME unit/co_string/read/bad_node  COMMAND co_string read_bad_node )
add_test(NAME unit/co_string/write/ref      COMMAND co_string write_ref     )
add_test(NAME unit/co_string/write/bad_node COMMAND co_string write_bad_node)
add_test(NAME unit/co_string/init/offset    COMMAND co_string init_offset   )
add_test(NAME unit/co_string/reset/offset   COMMAND co_string reset_offset  )
