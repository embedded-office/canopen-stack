#/*****************************************************************************
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

add_executable(co_domain main.c)
target_link_libraries(co_domain Canopen TestEnv)
target_code_coverage(co_domain ALL EXTERNAL)

#--- type function interface tests ---

add_test(NAME co_domain/size/unknown   COMMAND co_domain size_unknown  )
add_test(NAME co_domain/size/known     COMMAND co_domain size_known    )
add_test(NAME co_domain/size/bad_size  COMMAND co_domain bad_size      )
add_test(NAME co_domain/read/ref       COMMAND co_domain read_ref      )
add_test(NAME co_domain/read/bad_node  COMMAND co_domain read_bad_node )
add_test(NAME co_domain/write/ref      COMMAND co_domain write_ref     )
add_test(NAME co_domain/write/bad_node COMMAND co_domain write_bad_node)
add_test(NAME co_domain/init/offset    COMMAND co_domain init_offset   )
add_test(NAME co_domain/reset/offset   COMMAND co_domain reset_offset  )
