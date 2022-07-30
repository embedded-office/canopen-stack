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

add_executable(co_hb_cons main.c)
target_link_libraries(co_hb_cons Canopen TestEnv)

#--- type function interface tests ---

add_test(NAME co_hb_cons/size/sub_0       COMMAND co_hb_cons size_sub_0      )
add_test(NAME co_hb_cons/size/consumer    COMMAND co_hb_cons size_consumer   )
add_test(NAME co_hb_cons/size/bad_size    COMMAND co_hb_cons size_bad_size   )
add_test(NAME co_hb_cons/read/sub_0       COMMAND co_hb_cons read_sub_0      )
add_test(NAME co_hb_cons/read/consumer    COMMAND co_hb_cons read_consumer   )
add_test(NAME co_hb_cons/read/bad_node    COMMAND co_hb_cons read_bad_node   )
add_test(NAME co_hb_cons/write/sub_0      COMMAND co_hb_cons write_sub_0     )
add_test(NAME co_hb_cons/write/consumer   COMMAND co_hb_cons write_consumer  )
add_test(NAME co_hb_cons/write/bad_node   COMMAND co_hb_cons write_bad_node  )
add_test(NAME co_hb_cons/init/single      COMMAND co_hb_cons init_single     )
add_test(NAME co_hb_cons/init/multiple    COMMAND co_hb_cons init_multiple   )
add_test(NAME co_hb_cons/init/bad_index   COMMAND co_hb_cons init_bad_index  )
add_test(NAME co_hb_cons/reset/unused     COMMAND co_hb_cons reset_unused    )
