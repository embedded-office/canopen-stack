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

add_executable(ut-signed32 main.c)
target_link_libraries(ut-signed32 canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/signed32/size/unknown    COMMAND ut-signed32 size_unknown    )
add_test(NAME unit/object/signed32/size/known      COMMAND ut-signed32 size_known      )
add_test(NAME unit/object/signed32/size/bad_size   COMMAND ut-signed32 bad_size        )
add_test(NAME unit/object/signed32/read/ref        COMMAND ut-signed32 read_ref        )
add_test(NAME unit/object/signed32/read/direct     COMMAND ut-signed32 read_direct     )
add_test(NAME unit/object/signed32/read/nodeid     COMMAND ut-signed32 read_nodeid     )
add_test(NAME unit/object/signed32/read/bad_node   COMMAND ut-signed32 read_bad_node   )
add_test(NAME unit/object/signed32/write/ref       COMMAND ut-signed32 write_ref       )
add_test(NAME unit/object/signed32/write/direct    COMMAND ut-signed32 write_direct    )
add_test(NAME unit/object/signed32/write/nodeid    COMMAND ut-signed32 write_nodeid    )
add_test(NAME unit/object/signed32/write/pdo_async COMMAND ut-signed32 write_pdo_async )
add_test(NAME unit/object/signed32/write/pdo       COMMAND ut-signed32 write_pdo       )
add_test(NAME unit/object/signed32/write/bad_node  COMMAND ut-signed32 write_bad_node  )
add_test(NAME unit/object/signed32/init/unused     COMMAND ut-signed32 init_unused     )
add_test(NAME unit/object/signed32/reset/unused    COMMAND ut-signed32 reset_unused    )
