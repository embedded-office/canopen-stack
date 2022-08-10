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

add_executable(ut-pdo-type main.c)
target_link_libraries(ut-pdo-type canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/pdo-type/size/unknown   COMMAND ut-pdo-type size_unknown   )
add_test(NAME unit/object/pdo-type/size/known     COMMAND ut-pdo-type size_known     )
add_test(NAME unit/object/pdo-type/size/bad_size  COMMAND ut-pdo-type size_bad_size  )
add_test(NAME unit/object/pdo-type/read/ref       COMMAND ut-pdo-type read_ref       )
add_test(NAME unit/object/pdo-type/read/direct    COMMAND ut-pdo-type read_direct    )
add_test(NAME unit/object/pdo-type/read/bad_node  COMMAND ut-pdo-type read_bad_node  )
add_test(NAME unit/object/pdo-type/write/num      COMMAND ut-pdo-type write_num      )
add_test(NAME unit/object/pdo-type/write/bad_node COMMAND ut-pdo-type write_bad_node )
add_test(NAME unit/object/pdo-type/init/check     COMMAND ut-pdo-type init_check     )
add_test(NAME unit/object/pdo-type/init/bad_index COMMAND ut-pdo-type init_bad_index )
add_test(NAME unit/object/pdo-type/init/bad_sub   COMMAND ut-pdo-type init_bad_sub   )
add_test(NAME unit/object/pdo-type/reset/unused   COMMAND ut-pdo-type reset_unused   )
