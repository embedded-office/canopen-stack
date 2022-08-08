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

add_executable(co_emcy_hist main.c)
target_link_libraries(co_emcy_hist Canopen TestEnv)


#--- type function interface tests ---

add_test(NAME unit/co_emcy_hist/size/sub_0       COMMAND co_emcy_hist size_sub_0      )
add_test(NAME unit/co_emcy_hist/size/sub_1       COMMAND co_emcy_hist size_sub_1      )
add_test(NAME unit/co_emcy_hist/size/bad_size    COMMAND co_emcy_hist bad_size        )
add_test(NAME unit/co_emcy_hist/read/sub_0       COMMAND co_emcy_hist read_sub_0      )
add_test(NAME unit/co_emcy_hist/read/sub_1       COMMAND co_emcy_hist read_sub_1      )
add_test(NAME unit/co_emcy_hist/read/bad_node    COMMAND co_emcy_hist read_bad_node   )
add_test(NAME unit/co_emcy_hist/write/reset      COMMAND co_emcy_hist write_reset     )
add_test(NAME unit/co_emcy_hist/write/no_reset   COMMAND co_emcy_hist write_no_reset  )
add_test(NAME unit/co_emcy_hist/write/sub_1      COMMAND co_emcy_hist write_sub_1     )
add_test(NAME unit/co_emcy_hist/write/bad_node   COMMAND co_emcy_hist write_bad_node  )
add_test(NAME unit/co_emcy_hist/init/hist_min    COMMAND co_emcy_hist init_hist_min   )
add_test(NAME unit/co_emcy_hist/init/hist_8      COMMAND co_emcy_hist init_hist_8     )
add_test(NAME unit/co_emcy_hist/init/bad_index   COMMAND co_emcy_hist init_bad_index  )
add_test(NAME unit/co_emcy_hist/init/bad_hist    COMMAND co_emcy_hist init_bad_hist   )
add_test(NAME unit/co_emcy_hist/reset/unused     COMMAND co_emcy_hist reset_unused    )
