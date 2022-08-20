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

add_executable(ut-emcy-hist main.c)
target_link_libraries(ut-emcy-hist canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/emcy-hist/size/sub_0       COMMAND ut-emcy-hist size_sub_0      )
add_test(NAME unit/object/emcy-hist/size/sub_1       COMMAND ut-emcy-hist size_sub_1      )
add_test(NAME unit/object/emcy-hist/size/bad_size    COMMAND ut-emcy-hist bad_size        )
add_test(NAME unit/object/emcy-hist/read/sub_0       COMMAND ut-emcy-hist read_sub_0      )
add_test(NAME unit/object/emcy-hist/read/sub_1       COMMAND ut-emcy-hist read_sub_1      )
add_test(NAME unit/object/emcy-hist/read/bad_node    COMMAND ut-emcy-hist read_bad_node   )
add_test(NAME unit/object/emcy-hist/write/reset      COMMAND ut-emcy-hist write_reset     )
add_test(NAME unit/object/emcy-hist/write/no_reset   COMMAND ut-emcy-hist write_no_reset  )
add_test(NAME unit/object/emcy-hist/write/sub_1      COMMAND ut-emcy-hist write_sub_1     )
add_test(NAME unit/object/emcy-hist/write/bad_node   COMMAND ut-emcy-hist write_bad_node  )
add_test(NAME unit/object/emcy-hist/init/hist_min    COMMAND ut-emcy-hist init_hist_min   )
add_test(NAME unit/object/emcy-hist/init/hist_8      COMMAND ut-emcy-hist init_hist_8     )
add_test(NAME unit/object/emcy-hist/init/bad_index   COMMAND ut-emcy-hist init_bad_index  )
add_test(NAME unit/object/emcy-hist/init/bad_hist    COMMAND ut-emcy-hist init_bad_hist   )
add_test(NAME unit/object/emcy-hist/reset/unused     COMMAND ut-emcy-hist reset_unused    )
