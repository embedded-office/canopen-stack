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

add_executable(ut-pdo-event main.c)
target_link_libraries(ut-pdo-event canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/pdo-event/size/unknown   COMMAND ut-pdo-event size_unknown   )
add_test(NAME unit/object/pdo-event/size/known     COMMAND ut-pdo-event size_known     )
add_test(NAME unit/object/pdo-event/size/bad_size  COMMAND ut-pdo-event size_bad_size  )
add_test(NAME unit/object/pdo-event/read/ref       COMMAND ut-pdo-event read_ref       )
add_test(NAME unit/object/pdo-event/read/direct    COMMAND ut-pdo-event read_direct    )
add_test(NAME unit/object/pdo-event/read/bad_node  COMMAND ut-pdo-event read_bad_node  )
add_test(NAME unit/object/pdo-event/write/start    COMMAND ut-pdo-event write_start    )
add_test(NAME unit/object/pdo-event/write/change   COMMAND ut-pdo-event write_change   )
add_test(NAME unit/object/pdo-event/write/stop     COMMAND ut-pdo-event write_stop     )
add_test(NAME unit/object/pdo-event/write/bad_node COMMAND ut-pdo-event write_bad_node )
add_test(NAME unit/object/pdo-event/init/check     COMMAND ut-pdo-event init_check     )
add_test(NAME unit/object/pdo-event/init/bad_index COMMAND ut-pdo-event init_bad_index )
add_test(NAME unit/object/pdo-event/init/bad_sub   COMMAND ut-pdo-event init_bad_sub   )
add_test(NAME unit/object/pdo-event/reset/unused   COMMAND ut-pdo-event reset_unused   )
