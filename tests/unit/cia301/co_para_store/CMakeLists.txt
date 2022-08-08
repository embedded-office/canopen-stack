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

add_executable(co_para_store main.c)
target_link_libraries(co_para_store Canopen TestEnv)


#--- type function interface tests ---

add_test(NAME unit/co_para_store/size/sub_0           COMMAND co_para_store size_sub_0          )
add_test(NAME unit/co_para_store/size/sub_1           COMMAND co_para_store size_sub_1          )
add_test(NAME unit/co_para_store/size/bad_size        COMMAND co_para_store size_bad_size       )
add_test(NAME unit/co_para_store/read/enabled         COMMAND co_para_store read_enabled        )
add_test(NAME unit/co_para_store/read/autonomous      COMMAND co_para_store read_autonomous     )
add_test(NAME unit/co_para_store/read/bad_node        COMMAND co_para_store read_bad_node       )
add_test(NAME unit/co_para_store/write/single         COMMAND co_para_store write_single        )
add_test(NAME unit/co_para_store/write/all            COMMAND co_para_store write_all           )
add_test(NAME unit/co_para_store/write/bad_signature  COMMAND co_para_store write_bad_signature )
add_test(NAME unit/co_para_store/write/bad_index      COMMAND co_para_store write_bad_index     )
add_test(NAME unit/co_para_store/write/bad_node       COMMAND co_para_store write_bad_node      )
add_test(NAME unit/co_para_store/init/check           COMMAND co_para_store init_check          )
add_test(NAME unit/co_para_store/init/bad_index       COMMAND co_para_store init_bad_index      )
add_test(NAME unit/co_para_store/reset/com            COMMAND co_para_store reset_com           )
add_test(NAME unit/co_para_store/reset/node           COMMAND co_para_store reset_node          )
