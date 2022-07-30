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

add_executable(co_pdo_map main.c)
target_link_libraries(co_pdo_map Canopen TestEnv)

#--- type function interface tests ---

add_test(NAME co_pdo_map/size/unknown   COMMAND co_pdo_map size_unknown   )
add_test(NAME co_pdo_map/size/known     COMMAND co_pdo_map size_known     )
add_test(NAME co_pdo_map/size/bad_size  COMMAND co_pdo_map size_bad_size  )
add_test(NAME co_pdo_map/read/ref       COMMAND co_pdo_map read_ref       )
add_test(NAME co_pdo_map/read/direct    COMMAND co_pdo_map read_direct    )
add_test(NAME co_pdo_map/read/bad_node  COMMAND co_pdo_map read_bad_node  )
add_test(NAME co_pdo_map/write/mapping  COMMAND co_pdo_map write_mapping  )
add_test(NAME co_pdo_map/write/bad_node COMMAND co_pdo_map write_bad_node )
add_test(NAME co_pdo_map/init/check     COMMAND co_pdo_map init_check     )
add_test(NAME co_pdo_map/init/bad_index COMMAND co_pdo_map init_bad_index )
add_test(NAME co_pdo_map/reset/unused   COMMAND co_pdo_map reset_unused   )
