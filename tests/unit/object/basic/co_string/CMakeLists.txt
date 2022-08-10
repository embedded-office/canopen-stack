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

add_executable(ut-string main.c)
target_link_libraries(ut-string canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/string/size/unknown   COMMAND ut-string size_unknown  )
add_test(NAME unit/object/string/size/known     COMMAND ut-string size_known    )
add_test(NAME unit/object/string/size/bad_size  COMMAND ut-string bad_size      )
add_test(NAME unit/object/string/read/ref       COMMAND ut-string read_ref      )
add_test(NAME unit/object/string/read/bad_node  COMMAND ut-string read_bad_node )
add_test(NAME unit/object/string/write/ref      COMMAND ut-string write_ref     )
add_test(NAME unit/object/string/write/bad_node COMMAND ut-string write_bad_node)
add_test(NAME unit/object/string/init/offset    COMMAND ut-string init_offset   )
add_test(NAME unit/object/string/reset/offset   COMMAND ut-string reset_offset  )
