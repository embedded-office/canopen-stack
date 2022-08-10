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

add_executable(ut-sync-cycle main.c)
target_link_libraries(ut-sync-cycle canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/sync-cycle/size/unknown   COMMAND ut-sync-cycle size_unknown   )
add_test(NAME unit/object/sync-cycle/size/known     COMMAND ut-sync-cycle size_known     )
add_test(NAME unit/object/sync-cycle/size/bad_size  COMMAND ut-sync-cycle size_bad_size  )
add_test(NAME unit/object/sync-cycle/read/time      COMMAND ut-sync-cycle read_time      )
add_test(NAME unit/object/sync-cycle/read/bad_node  COMMAND ut-sync-cycle read_bad_node  )
add_test(NAME unit/object/sync-cycle/write/active   COMMAND ut-sync-cycle write_active   )
add_test(NAME unit/object/sync-cycle/write/inactive COMMAND ut-sync-cycle write_inactive )
add_test(NAME unit/object/sync-cycle/write/bad_node COMMAND ut-sync-cycle write_bad_node )
add_test(NAME unit/object/sync-cycle/init/check     COMMAND ut-sync-cycle init_check     )
add_test(NAME unit/object/sync-cycle/init/bad_index COMMAND ut-sync-cycle init_bad_index )
add_test(NAME unit/object/sync-cycle/init/bad_sub   COMMAND ut-sync-cycle init_bad_sub   )
add_test(NAME unit/object/sync-cycle/reset/unused   COMMAND ut-sync-cycle reset_unused   )
