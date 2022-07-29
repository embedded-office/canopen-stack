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

add_executable(co_pdo_id main.c)
target_link_libraries(co_pdo_id Canopen TestEnv)
target_code_coverage(co_pdo_id ALL EXTERNAL)

#--- type function interface tests ---

add_test(NAME co_pdo_id/size/unknown     COMMAND co_pdo_id size_unknown     )
add_test(NAME co_pdo_id/size/known       COMMAND co_pdo_id size_known       )
add_test(NAME co_pdo_id/size/size        COMMAND co_pdo_id bad_size         )
add_test(NAME co_pdo_id/read/ref         COMMAND co_pdo_id read_ref         )
add_test(NAME co_pdo_id/read/direct      COMMAND co_pdo_id read_direct      )
add_test(NAME co_pdo_id/read/bad_node    COMMAND co_pdo_id read_bad_node    )
add_test(NAME co_pdo_id/write/activate   COMMAND co_pdo_id write_activate   )
add_test(NAME co_pdo_id/write/deactivate COMMAND co_pdo_id write_deactivate )
add_test(NAME co_pdo_id/write/active     COMMAND co_pdo_id write_active     )
add_test(NAME co_pdo_id/write/bad_node   COMMAND co_pdo_id write_bad_node   )
add_test(NAME co_pdo_id/init/check       COMMAND co_pdo_id init_check       )
add_test(NAME co_pdo_id/init/bad_index   COMMAND co_pdo_id init_bad_index   )
add_test(NAME co_pdo_id/init/bad_sub     COMMAND co_pdo_id init_bad_sub     )
add_test(NAME co_pdo_id/reset/unused     COMMAND co_pdo_id reset_unused     )
