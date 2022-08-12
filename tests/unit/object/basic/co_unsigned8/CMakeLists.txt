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

add_executable(ut-unsigned8 main.c)
target_link_libraries(ut-unsigned8 canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/unsigned8/size/unknown    COMMAND ut-unsigned8 size_unknown    )
add_test(NAME unit/object/unsigned8/size/known      COMMAND ut-unsigned8 size_known      )
add_test(NAME unit/object/unsigned8/size/bad_size   COMMAND ut-unsigned8 bad_size        )
add_test(NAME unit/object/unsigned8/read/ref        COMMAND ut-unsigned8 read_ref        )
add_test(NAME unit/object/unsigned8/read/direct     COMMAND ut-unsigned8 read_direct     )
add_test(NAME unit/object/unsigned8/read/nodeid     COMMAND ut-unsigned8 read_nodeid     )
add_test(NAME unit/object/unsigned8/read/bad_node   COMMAND ut-unsigned8 read_bad_node   )
add_test(NAME unit/object/unsigned8/write/ref       COMMAND ut-unsigned8 write_ref       )
add_test(NAME unit/object/unsigned8/write/direct    COMMAND ut-unsigned8 write_direct    )
add_test(NAME unit/object/unsigned8/write/nodeid    COMMAND ut-unsigned8 write_nodeid    )
add_test(NAME unit/object/unsigned8/write/pdo_async COMMAND ut-unsigned8 write_pdo_async )
add_test(NAME unit/object/unsigned8/write/pdo       COMMAND ut-unsigned8 write_pdo       )
add_test(NAME unit/object/unsigned8/write/bad_node  COMMAND ut-unsigned8 write_bad_node  )
add_test(NAME unit/object/unsigned8/init/unused     COMMAND ut-unsigned8 init_unused     )
add_test(NAME unit/object/unsigned8/reset/unused    COMMAND ut-unsigned8 reset_unused    )
