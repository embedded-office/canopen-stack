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

add_executable(ut-pdo-map main.c)
target_link_libraries(ut-pdo-map canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/pdo-map/size/unknown   COMMAND ut-pdo-map size_unknown   )
add_test(NAME unit/object/pdo-map/size/known     COMMAND ut-pdo-map size_known     )
add_test(NAME unit/object/pdo-map/size/bad_size  COMMAND ut-pdo-map size_bad_size  )
add_test(NAME unit/object/pdo-map/read/ref       COMMAND ut-pdo-map read_ref       )
add_test(NAME unit/object/pdo-map/read/direct    COMMAND ut-pdo-map read_direct    )
add_test(NAME unit/object/pdo-map/read/bad_node  COMMAND ut-pdo-map read_bad_node  )
add_test(NAME unit/object/pdo-map/write/mapping  COMMAND ut-pdo-map write_mapping  )
add_test(NAME unit/object/pdo-map/write/bad_node COMMAND ut-pdo-map write_bad_node )
add_test(NAME unit/object/pdo-map/init/check     COMMAND ut-pdo-map init_check     )
add_test(NAME unit/object/pdo-map/init/bad_index COMMAND ut-pdo-map init_bad_index )
add_test(NAME unit/object/pdo-map/reset/unused   COMMAND ut-pdo-map reset_unused   )
