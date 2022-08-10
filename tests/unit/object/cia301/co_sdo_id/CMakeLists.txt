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

add_executable(ut-sdo-id main.c)
target_link_libraries(ut-sdo-id canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/sdo-id/size/unknown     COMMAND ut-sdo-id size_unknown     )
add_test(NAME unit/object/sdo-id/size/known       COMMAND ut-sdo-id size_known       )
add_test(NAME unit/object/sdo-id/size/size        COMMAND ut-sdo-id bad_size         )
add_test(NAME unit/object/sdo-id/read/ref         COMMAND ut-sdo-id read_ref         )
add_test(NAME unit/object/sdo-id/read/direct      COMMAND ut-sdo-id read_direct      )
add_test(NAME unit/object/sdo-id/read/bad_node    COMMAND ut-sdo-id read_bad_node    )
add_test(NAME unit/object/sdo-id/write/activate   COMMAND ut-sdo-id write_activate   )
add_test(NAME unit/object/sdo-id/write/deactivate COMMAND ut-sdo-id write_deactivate )
add_test(NAME unit/object/sdo-id/write/active     COMMAND ut-sdo-id write_active     )
add_test(NAME unit/object/sdo-id/write/bad_node   COMMAND ut-sdo-id write_bad_node   )
add_test(NAME unit/object/sdo-id/init/check       COMMAND ut-sdo-id init_check       )
add_test(NAME unit/object/sdo-id/init/bad_index   COMMAND ut-sdo-id init_bad_index   )
add_test(NAME unit/object/sdo-id/init/bad_sub     COMMAND ut-sdo-id init_bad_sub     )
add_test(NAME unit/object/sdo-id/reset/unused     COMMAND ut-sdo-id reset_unused     )
