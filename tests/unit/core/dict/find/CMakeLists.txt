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

add_executable(ut-dict-find main.c)
target_link_libraries(ut-dict-find canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/dict/find/no_init   COMMAND ut-dict-find no_init   )
add_test(NAME unit/dict/find/empty     COMMAND ut-dict-find empty     )
add_test(NAME unit/dict/find/single    COMMAND ut-dict-find single    )
add_test(NAME unit/dict/find/first     COMMAND ut-dict-find first     )
add_test(NAME unit/dict/find/middle    COMMAND ut-dict-find middle    )
add_test(NAME unit/dict/find/last      COMMAND ut-dict-find last      )
add_test(NAME unit/dict/find/not_found COMMAND ut-dict-find not_found )
