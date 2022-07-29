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

add_executable(co_pdo_type main.c)
target_link_libraries(co_pdo_type Canopen TestEnv)

#--- type function interface tests ---

add_test(NAME co_pdo_type/size/unknown   COMMAND co_pdo_type size_unknown   )
add_test(NAME co_pdo_type/size/known     COMMAND co_pdo_type size_known     )
add_test(NAME co_pdo_type/size/bad_size  COMMAND co_pdo_type size_bad_size  )
add_test(NAME co_pdo_type/read/ref       COMMAND co_pdo_type read_ref       )
add_test(NAME co_pdo_type/read/direct    COMMAND co_pdo_type read_direct    )
add_test(NAME co_pdo_type/read/bad_node  COMMAND co_pdo_type read_bad_node  )
add_test(NAME co_pdo_type/write/num      COMMAND co_pdo_type write_num      )
add_test(NAME co_pdo_type/write/bad_node COMMAND co_pdo_type write_bad_node )
add_test(NAME co_pdo_type/init/check     COMMAND co_pdo_type init_check     )
add_test(NAME co_pdo_type/init/bad_index COMMAND co_pdo_type init_bad_index )
add_test(NAME co_pdo_type/init/bad_sub   COMMAND co_pdo_type init_bad_sub   )
add_test(NAME co_pdo_type/reset/unused   COMMAND co_pdo_type reset_unused   )
