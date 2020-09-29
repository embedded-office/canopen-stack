#---
# define the test application executable
#
add_executable(CanopenTests)

#---
# test application with test runner and supporting functions
#
target_sources(CanopenTests
  PRIVATE
    app/app_dict.c
    app/app_dom.c
    app/app_emcy.c
    app/app_env.c
    app/app_hooks.c
    app/app.c
)
target_include_directories(CanopenTests
  PRIVATE
    app
)

#---
# interface driver for test
#
target_sources(CanopenTests
  PRIVATE
    driver/drv_can.c
)
target_include_directories(CanopenTests
  PRIVATE
    driver
)

#---
# add the test framework
#
target_sources(CanopenTests
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
target_include_directories(CanopenTests
  PRIVATE
    testfrm
)

#---
# select the test-suite files
#
target_sources(CanopenTests
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
    tests/pdo_dyn.c
    tests/pdo_rx.c
    tests/pdo_tx.c
    tests/sdos_blk_down.c
    tests/sdos_blk_up.c
    tests/sdos_exp_down.c
    tests/sdos_exp_up.c
    tests/sdos_seg_down.c
    tests/sdos_seg_up.c
)
target_include_directories(CanopenTests
  PRIVATE
    tests
)

#---
# specify the dependencies for this application
#
target_link_libraries(CanopenTests Canopen)
