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

add_executable(ut-emcy-id main.c)
target_link_libraries(ut-emcy-id canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/emcy-id/size/unknown     COMMAND ut-emcy-id size_unknown     )
add_test(NAME unit/object/emcy-id/size/known       COMMAND ut-emcy-id size_known       )
add_test(NAME unit/object/emcy-id/size/size        COMMAND ut-emcy-id bad_size         )
add_test(NAME unit/object/emcy-id/read/ref         COMMAND ut-emcy-id read_ref         )
add_test(NAME unit/object/emcy-id/read/direct      COMMAND ut-emcy-id read_direct      )
add_test(NAME unit/object/emcy-id/read/bad_node    COMMAND ut-emcy-id read_bad_node    )
add_test(NAME unit/object/emcy-id/write/activate   COMMAND ut-emcy-id write_activate   )
add_test(NAME unit/object/emcy-id/write/deactivate COMMAND ut-emcy-id write_deactivate )
add_test(NAME unit/object/emcy-id/write/active     COMMAND ut-emcy-id write_active     )
add_test(NAME unit/object/emcy-id/write/bad_node   COMMAND ut-emcy-id write_bad_node   )
add_test(NAME unit/object/emcy-id/init/check       COMMAND ut-emcy-id init_check       )
add_test(NAME unit/object/emcy-id/init/bad_index   COMMAND ut-emcy-id init_bad_index   )
add_test(NAME unit/object/emcy-id/init/bad_sub     COMMAND ut-emcy-id init_bad_sub     )
add_test(NAME unit/object/emcy-id/reset/unused     COMMAND ut-emcy-id reset_unused     )
