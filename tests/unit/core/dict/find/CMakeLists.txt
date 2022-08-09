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

add_executable(dict_find main.c)
target_link_libraries(dict_find Canopen TestEnv)


#--- type function interface tests ---

add_test(NAME unit/dict/find/no_init   COMMAND dict_find no_init   )
add_test(NAME unit/dict/find/empty     COMMAND dict_find empty     )
add_test(NAME unit/dict/find/single    COMMAND dict_find single    )
add_test(NAME unit/dict/find/first     COMMAND dict_find first     )
add_test(NAME unit/dict/find/middle    COMMAND dict_find middle    )
add_test(NAME unit/dict/find/last      COMMAND dict_find last      )
add_test(NAME unit/dict/find/not_found COMMAND dict_find not_found )
