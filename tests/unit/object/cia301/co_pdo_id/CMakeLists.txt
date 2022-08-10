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

add_executable(ut-pdo-id main.c)
target_link_libraries(ut-pdo-id canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/pdo-id/size/unknown     COMMAND ut-pdo-id size_unknown     )
add_test(NAME unit/object/pdo-id/size/known       COMMAND ut-pdo-id size_known       )
add_test(NAME unit/object/pdo-id/size/size        COMMAND ut-pdo-id bad_size         )
add_test(NAME unit/object/pdo-id/read/ref         COMMAND ut-pdo-id read_ref         )
add_test(NAME unit/object/pdo-id/read/direct      COMMAND ut-pdo-id read_direct      )
add_test(NAME unit/object/pdo-id/read/bad_node    COMMAND ut-pdo-id read_bad_node    )
add_test(NAME unit/object/pdo-id/write/activate   COMMAND ut-pdo-id write_activate   )
add_test(NAME unit/object/pdo-id/write/deactivate COMMAND ut-pdo-id write_deactivate )
add_test(NAME unit/object/pdo-id/write/active     COMMAND ut-pdo-id write_active     )
add_test(NAME unit/object/pdo-id/write/bad_node   COMMAND ut-pdo-id write_bad_node   )
add_test(NAME unit/object/pdo-id/init/check       COMMAND ut-pdo-id init_check       )
add_test(NAME unit/object/pdo-id/init/bad_index   COMMAND ut-pdo-id init_bad_index   )
add_test(NAME unit/object/pdo-id/init/bad_sub     COMMAND ut-pdo-id init_bad_sub     )
add_test(NAME unit/object/pdo-id/reset/unused     COMMAND ut-pdo-id reset_unused     )
