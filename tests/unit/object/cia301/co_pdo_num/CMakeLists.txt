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

add_executable(ut-pdo-num main.c)
target_link_libraries(ut-pdo-num canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/pdo-num/size/unknown   COMMAND ut-pdo-num size_unknown   )
add_test(NAME unit/object/pdo-num/size/known     COMMAND ut-pdo-num size_known     )
add_test(NAME unit/object/pdo-num/size/bad_size  COMMAND ut-pdo-num size_bad_size  )
add_test(NAME unit/object/pdo-num/read/ref       COMMAND ut-pdo-num read_ref       )
add_test(NAME unit/object/pdo-num/read/direct    COMMAND ut-pdo-num read_direct    )
add_test(NAME unit/object/pdo-num/read/bad_node  COMMAND ut-pdo-num read_bad_node  )
add_test(NAME unit/object/pdo-num/write/num      COMMAND ut-pdo-num write_num      )
add_test(NAME unit/object/pdo-num/write/bad_node COMMAND ut-pdo-num write_bad_node )
add_test(NAME unit/object/pdo-num/init/check     COMMAND ut-pdo-num init_check     )
add_test(NAME unit/object/pdo-num/init/bad_index COMMAND ut-pdo-num init_bad_index )
add_test(NAME unit/object/pdo-num/init/bad_sub   COMMAND ut-pdo-num init_bad_sub   )
add_test(NAME unit/object/pdo-num/reset/unused   COMMAND ut-pdo-num reset_unused   )
