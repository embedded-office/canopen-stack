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

add_executable(ut-para-store main.c)
target_link_libraries(ut-para-store canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/para-store/size/sub_0           COMMAND ut-para-store size_sub_0          )
add_test(NAME unit/object/para-store/size/sub_1           COMMAND ut-para-store size_sub_1          )
add_test(NAME unit/object/para-store/size/bad_size        COMMAND ut-para-store size_bad_size       )
add_test(NAME unit/object/para-store/read/enabled         COMMAND ut-para-store read_enabled        )
add_test(NAME unit/object/para-store/read/autonomous      COMMAND ut-para-store read_autonomous     )
add_test(NAME unit/object/para-store/read/bad_node        COMMAND ut-para-store read_bad_node       )
add_test(NAME unit/object/para-store/write/single         COMMAND ut-para-store write_single        )
add_test(NAME unit/object/para-store/write/all            COMMAND ut-para-store write_all           )
add_test(NAME unit/object/para-store/write/bad_signature  COMMAND ut-para-store write_bad_signature )
add_test(NAME unit/object/para-store/write/bad_index      COMMAND ut-para-store write_bad_index     )
add_test(NAME unit/object/para-store/write/bad_node       COMMAND ut-para-store write_bad_node      )
add_test(NAME unit/object/para-store/init/check           COMMAND ut-para-store init_check          )
add_test(NAME unit/object/para-store/init/bad_index       COMMAND ut-para-store init_bad_index      )
add_test(NAME unit/object/para-store/reset/com            COMMAND ut-para-store reset_com           )
add_test(NAME unit/object/para-store/reset/node           COMMAND ut-para-store reset_node          )
