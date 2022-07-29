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

add_executable(co_hb_prod main.c)
target_link_libraries(co_hb_prod Canopen TestEnv)

#--- type function interface tests ---

add_test(NAME co_hb_prod/size/unknown    COMMAND co_hb_prod size_unknown    )
add_test(NAME co_hb_prod/size/known      COMMAND co_hb_prod size_known      )
add_test(NAME co_hb_prod/size/bad_size   COMMAND co_hb_prod size_bad_size   )
add_test(NAME co_hb_prod/read/time       COMMAND co_hb_prod read_time       )
add_test(NAME co_hb_prod/read/bad_node   COMMAND co_hb_prod read_bad_node   )
add_test(NAME co_hb_prod/write/start     COMMAND co_hb_prod write_start     )
add_test(NAME co_hb_prod/write/restart   COMMAND co_hb_prod write_restart   )
add_test(NAME co_hb_prod/write/bad_index COMMAND co_hb_prod write_bad_index )
add_test(NAME co_hb_prod/write/bad_node  COMMAND co_hb_prod write_bad_node  )
add_test(NAME co_hb_prod/init/check      COMMAND co_hb_prod init_check      )
add_test(NAME co_hb_prod/init/start      COMMAND co_hb_prod init_start      )
add_test(NAME co_hb_prod/init/bad_index  COMMAND co_hb_prod init_bad_index  )
add_test(NAME co_hb_prod/reset/unused    COMMAND co_hb_prod reset_unused    )
