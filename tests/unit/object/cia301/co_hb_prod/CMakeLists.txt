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

add_executable(ut-hb-prod main.c)
target_link_libraries(ut-hb-prod canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/hb-prod/size/unknown    COMMAND ut-hb-prod size_unknown    )
add_test(NAME unit/object/hb-prod/size/known      COMMAND ut-hb-prod size_known      )
add_test(NAME unit/object/hb-prod/size/bad_size   COMMAND ut-hb-prod size_bad_size   )
add_test(NAME unit/object/hb-prod/read/time       COMMAND ut-hb-prod read_time       )
add_test(NAME unit/object/hb-prod/read/bad_node   COMMAND ut-hb-prod read_bad_node   )
add_test(NAME unit/object/hb-prod/write/start     COMMAND ut-hb-prod write_start     )
add_test(NAME unit/object/hb-prod/write/restart   COMMAND ut-hb-prod write_restart   )
add_test(NAME unit/object/hb-prod/write/bad_index COMMAND ut-hb-prod write_bad_index )
add_test(NAME unit/object/hb-prod/write/bad_node  COMMAND ut-hb-prod write_bad_node  )
add_test(NAME unit/object/hb-prod/init/check      COMMAND ut-hb-prod init_check      )
add_test(NAME unit/object/hb-prod/init/start      COMMAND ut-hb-prod init_start      )
add_test(NAME unit/object/hb-prod/init/bad_index  COMMAND ut-hb-prod init_bad_index  )
add_test(NAME unit/object/hb-prod/reset/unused    COMMAND ut-hb-prod reset_unused    )
