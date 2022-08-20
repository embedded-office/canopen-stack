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

add_executable(ut-sync-id main.c)
target_link_libraries(ut-sync-id canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/sync-id/size/unknown     COMMAND ut-sync-id size_unknown     )
add_test(NAME unit/object/sync-id/size/known       COMMAND ut-sync-id size_known       )
add_test(NAME unit/object/sync-id/size/size        COMMAND ut-sync-id bad_size         )
add_test(NAME unit/object/sync-id/read/ref         COMMAND ut-sync-id read_ref         )
add_test(NAME unit/object/sync-id/read/direct      COMMAND ut-sync-id read_direct      )
add_test(NAME unit/object/sync-id/read/bad_node    COMMAND ut-sync-id read_bad_node    )
add_test(NAME unit/object/sync-id/write/activate   COMMAND ut-sync-id write_activate   )
add_test(NAME unit/object/sync-id/write/deactivate COMMAND ut-sync-id write_deactivate )
add_test(NAME unit/object/sync-id/write/active     COMMAND ut-sync-id write_active     )
add_test(NAME unit/object/sync-id/write/bad_node   COMMAND ut-sync-id write_bad_node   )
add_test(NAME unit/object/sync-id/init/check       COMMAND ut-sync-id init_check       )
add_test(NAME unit/object/sync-id/init/bad_index   COMMAND ut-sync-id init_bad_index   )
add_test(NAME unit/object/sync-id/init/bad_sub     COMMAND ut-sync-id init_bad_sub     )
add_test(NAME unit/object/sync-id/reset/unused     COMMAND ut-sync-id reset_unused     )
