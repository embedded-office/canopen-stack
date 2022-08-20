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

#---
# define the test application executable
#
add_executable(it-canopen-stack)

#---
# test application with test runner and supporting functions
#
target_sources(it-canopen-stack
  PRIVATE
    app/app_dict.c
    app/app_dom.c
    app/app_emcy.c
    app/app_env.c
    app/app_hooks.c
    app/app.c
)
target_include_directories(it-canopen-stack
  PRIVATE
    app
)

#---
# add the host drivers
#
target_sources(it-canopen-stack
  PRIVATE
    driver/drv_can_sim.c
    driver/drv_nvm_sim.c
    driver/drv_timer_swcycle.c
)
target_include_directories(it-canopen-stack
  PRIVATE
    driver
)

#---
# add the test framework
#
target_sources(it-canopen-stack
  PRIVATE
    testfrm/ts_context.c
    testfrm/ts_env.c
    testfrm/ts_list.c
    testfrm/ts_lock.c
    testfrm/ts_mem.c
    testfrm/ts_output.c
    testfrm/ts_pipe.c
    testfrm/ts_printf.c
    testfrm/ts_version.c
)
target_include_directories(it-canopen-stack
  PRIVATE
    testfrm
)

#---
# select the test-suite files
#
target_sources(it-canopen-stack
  PRIVATE
    tests/core_tmr.c
    tests/emcy_api.c
    tests/emcy_err.c
    tests/emcy_hist.c
    tests/emcy_state.c
    tests/nmt_hbc.c
    tests/nmt_hbp.c
    tests/nmt_lss.c
    tests/nmt_mgr.c
    tests/od_api.c
    tests/pdo_dyn.c
    tests/pdo_rx.c
    tests/pdo_tx.c
    tests/sdoc_exp_down.c
    tests/sdoc_exp_up.c
    tests/sdoc_seg_down.c
    tests/sdoc_seg_up.c
    tests/sdos_blk_down.c
    tests/sdos_blk_up.c
    tests/sdos_exp_down.c
    tests/sdos_exp_up.c
    tests/sdos_seg_down.c
    tests/sdos_seg_up.c
    tests/sync_prod.c
)
target_include_directories(it-canopen-stack
  PRIVATE
    tests
)

#---
# specify the dependencies for this application
#
target_link_libraries(it-canopen-stack canopen-stack)

#--- integration tests ---

add_test(NAME integration/all COMMAND it-canopen-stack)
