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

add_executable(co_sync_cycle main.c)
target_link_libraries(co_sync_cycle Canopen TestEnv)

#--- type function interface tests ---

add_test(NAME co_sync_cycle/size/unknown   COMMAND co_sync_cycle size_unknown   )
add_test(NAME co_sync_cycle/size/known     COMMAND co_sync_cycle size_known     )
add_test(NAME co_sync_cycle/size/bad_size  COMMAND co_sync_cycle size_bad_size  )
add_test(NAME co_sync_cycle/read/time      COMMAND co_sync_cycle read_time      )
add_test(NAME co_sync_cycle/read/bad_node  COMMAND co_sync_cycle read_bad_node  )
add_test(NAME co_sync_cycle/write/time     COMMAND co_sync_cycle write_time     )
add_test(NAME co_sync_cycle/write/bad_node COMMAND co_sync_cycle write_bad_node )
add_test(NAME co_sync_cycle/init/check     COMMAND co_sync_cycle init_check     )
add_test(NAME co_sync_cycle/init/bad_index COMMAND co_sync_cycle init_bad_index )
add_test(NAME co_sync_cycle/init/bad_sub   COMMAND co_sync_cycle init_bad_sub   )
add_test(NAME co_sync_cycle/reset/unused   COMMAND co_sync_cycle reset_unused   )
