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

add_executable(ut-hb-cons main.c)
target_link_libraries(ut-hb-cons canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/hb-cons/size/sub_0       COMMAND ut-hb-cons size_sub_0      )
add_test(NAME unit/object/hb-cons/size/consumer    COMMAND ut-hb-cons size_consumer   )
add_test(NAME unit/object/hb-cons/size/bad_size    COMMAND ut-hb-cons size_bad_size   )
add_test(NAME unit/object/hb-cons/read/sub_0       COMMAND ut-hb-cons read_sub_0      )
add_test(NAME unit/object/hb-cons/read/consumer    COMMAND ut-hb-cons read_consumer   )
add_test(NAME unit/object/hb-cons/read/bad_node    COMMAND ut-hb-cons read_bad_node   )
add_test(NAME unit/object/hb-cons/write/sub_0      COMMAND ut-hb-cons write_sub_0     )
add_test(NAME unit/object/hb-cons/write/consumer   COMMAND ut-hb-cons write_consumer  )
add_test(NAME unit/object/hb-cons/write/bad_node   COMMAND ut-hb-cons write_bad_node  )
add_test(NAME unit/object/hb-cons/init/single      COMMAND ut-hb-cons init_single     )
add_test(NAME unit/object/hb-cons/init/multiple    COMMAND ut-hb-cons init_multiple   )
add_test(NAME unit/object/hb-cons/init/bad_index   COMMAND ut-hb-cons init_bad_index  )
add_test(NAME unit/object/hb-cons/reset/unused     COMMAND ut-hb-cons reset_unused    )
