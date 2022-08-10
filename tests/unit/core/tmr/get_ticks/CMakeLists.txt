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

add_executable(ut-tmr-get-ticks main.c)
target_link_libraries(ut-tmr-get-ticks canopen-stack ut-test-env)


#--- type function interface tests ---

add_test(NAME unit/tmr/get_ticks/1MHz_0ms       COMMAND ut-tmr-get-ticks 1MHz_0ms       )
add_test(NAME unit/tmr/get_ticks/1MHz_23ms      COMMAND ut-tmr-get-ticks 1MHz_23ms      )
add_test(NAME unit/tmr/get_ticks/1MHz_456ms     COMMAND ut-tmr-get-ticks 1MHz_456ms     )
add_test(NAME unit/tmr/get_ticks/1MHz_1000ms    COMMAND ut-tmr-get-ticks 1MHz_1000ms    )
add_test(NAME unit/tmr/get_ticks/1MHz_65535ms   COMMAND ut-tmr-get-ticks 1MHz_65535ms   )
add_test(NAME unit/tmr/get_ticks/1MHz_100us     COMMAND ut-tmr-get-ticks 1MHz_100us     )
add_test(NAME unit/tmr/get_ticks/1MHz_2300us    COMMAND ut-tmr-get-ticks 1MHz_2300us    )
add_test(NAME unit/tmr/get_ticks/1MHz_72500us   COMMAND ut-tmr-get-ticks 1MHz_72500us   )
add_test(NAME unit/tmr/get_ticks/1MHz_6553500us COMMAND ut-tmr-get-ticks 1MHz_6553500us )
add_test(NAME unit/tmr/get_ticks/1kHz_0ms       COMMAND ut-tmr-get-ticks 1kHz_0ms       )
add_test(NAME unit/tmr/get_ticks/1kHz_23ms      COMMAND ut-tmr-get-ticks 1kHz_23ms      )
add_test(NAME unit/tmr/get_ticks/1kHz_456ms     COMMAND ut-tmr-get-ticks 1kHz_456ms     )
add_test(NAME unit/tmr/get_ticks/1kHz_1000ms    COMMAND ut-tmr-get-ticks 1kHz_1000ms    )
add_test(NAME unit/tmr/get_ticks/1kHz_65535ms   COMMAND ut-tmr-get-ticks 1kHz_65535ms   )
add_test(NAME unit/tmr/get_ticks/1kHz_100us     COMMAND ut-tmr-get-ticks 1kHz_100us     )
add_test(NAME unit/tmr/get_ticks/100Hz_6ms      COMMAND ut-tmr-get-ticks 100Hz_6ms      )
add_test(NAME unit/tmr/get_ticks/100Hz_42ms     COMMAND ut-tmr-get-ticks 100Hz_42ms     )
add_test(NAME unit/tmr/get_ticks/100Hz_789ms    COMMAND ut-tmr-get-ticks 100Hz_789ms    )
add_test(NAME unit/tmr/get_ticks/100Hz_5000ms   COMMAND ut-tmr-get-ticks 100Hz_5000ms   )
add_test(NAME unit/tmr/get_ticks/100Hz_65535ms  COMMAND ut-tmr-get-ticks 100Hz_65535ms  )
add_test(NAME unit/tmr/get_ticks/100Hz_100us    COMMAND ut-tmr-get-ticks 100Hz_100us    )
