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

add_executable(ut-domain main.c)
target_link_libraries(ut-domain canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/domain/size/unknown   COMMAND ut-domain size_unknown  )
add_test(NAME unit/object/domain/size/known     COMMAND ut-domain size_known    )
add_test(NAME unit/object/domain/size/small     COMMAND ut-domain size_small    )
add_test(NAME unit/object/domain/size/large     COMMAND ut-domain size_large    )
add_test(NAME unit/object/domain/size/bad_size  COMMAND ut-domain bad_size      )
add_test(NAME unit/object/domain/read/all       COMMAND ut-domain read_all      )
add_test(NAME unit/object/domain/read/part      COMMAND ut-domain read_part     )
add_test(NAME unit/object/domain/read/bad_node  COMMAND ut-domain read_bad_node )
add_test(NAME unit/object/domain/write/all      COMMAND ut-domain write_all     )
add_test(NAME unit/object/domain/write/part     COMMAND ut-domain write_part    )
add_test(NAME unit/object/domain/write/bad_node COMMAND ut-domain write_bad_node)
add_test(NAME unit/object/domain/init/offset    COMMAND ut-domain init_offset   )
add_test(NAME unit/object/domain/reset/offset   COMMAND ut-domain reset_offset  )
