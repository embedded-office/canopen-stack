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

add_executable(ut-para-restore main.c)
target_link_libraries(ut-para-restore canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/object/para-restore/size/sub_0          COMMAND ut-para-restore size_sub_0          )
add_test(NAME unit/object/para-restore/size/sub_1          COMMAND ut-para-restore size_sub_1          )
add_test(NAME unit/object/para-restore/size/bad_size       COMMAND ut-para-restore size_bad_size       )
add_test(NAME unit/object/para-restore/read/enabled        COMMAND ut-para-restore read_enabled        )
add_test(NAME unit/object/para-restore/read/disabled       COMMAND ut-para-restore read_disabled       )
add_test(NAME unit/object/para-restore/read/bad_node       COMMAND ut-para-restore read_bad_node       )
add_test(NAME unit/object/para-restore/write/single        COMMAND ut-para-restore write_single        )
add_test(NAME unit/object/para-restore/write/all           COMMAND ut-para-restore write_all           )
add_test(NAME unit/object/para-restore/write/bad_signature COMMAND ut-para-restore write_bad_signature )
add_test(NAME unit/object/para-restore/write/bad_index     COMMAND ut-para-restore write_bad_index     )
add_test(NAME unit/object/para-restore/write/bad_node      COMMAND ut-para-restore write_bad_node      )
add_test(NAME unit/object/para-restore/init/check          COMMAND ut-para-restore init_check          )
add_test(NAME unit/object/para-restore/init/bad_index      COMMAND ut-para-restore init_bad_index      )
add_test(NAME unit/object/para-restore/reset/unused        COMMAND ut-para-restore reset_unused        )
