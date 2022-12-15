#******************************************************************************
#   Copyright 2022 Generac Holdings Inc.
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

add_executable(ut-unsigned64 main.c)
target_link_libraries(ut-unsigned64 canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/unsigned64/size/unknown    COMMAND ut-unsigned64 size_unknown    )
add_test(NAME unit/object/unsigned64/size/known      COMMAND ut-unsigned64 size_known      )
add_test(NAME unit/object/unsigned64/size/bad_size   COMMAND ut-unsigned64 bad_size        )
add_test(NAME unit/object/unsigned64/read/ref        COMMAND ut-unsigned64 read_ref        )
add_test(NAME unit/object/unsigned64/read/direct     COMMAND ut-unsigned64 read_direct     )
add_test(NAME unit/object/unsigned64/read/nodeid     COMMAND ut-unsigned64 read_nodeid     )
add_test(NAME unit/object/unsigned64/read/bad_node   COMMAND ut-unsigned64 read_bad_node   )
add_test(NAME unit/object/unsigned64/write/ref       COMMAND ut-unsigned64 write_ref       )
add_test(NAME unit/object/unsigned64/write/direct    COMMAND ut-unsigned64 write_direct    )
add_test(NAME unit/object/unsigned64/write/nodeid    COMMAND ut-unsigned64 write_nodeid    )
add_test(NAME unit/object/unsigned64/write/pdo_async COMMAND ut-unsigned64 write_pdo_async )
add_test(NAME unit/object/unsigned64/write/pdo       COMMAND ut-unsigned64 write_pdo       )
add_test(NAME unit/object/unsigned64/write/bad_node  COMMAND ut-unsigned64 write_bad_node  )
add_test(NAME unit/object/unsigned64/init/unused     COMMAND ut-unsigned64 init_unused     )
add_test(NAME unit/object/unsigned64/reset/unused    COMMAND ut-unsigned64 reset_unused    )
