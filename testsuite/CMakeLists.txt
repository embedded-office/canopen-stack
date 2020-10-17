#---
# define the test application executable
#
add_executable(CanopenTest)

#---
# test application with test runner and supporting functions
#
target_sources(CanopenTest
  PRIVATE
    app/app_dict.c
    app/app_dom.c
    app/app_emcy.c
    app/app_env.c
    app/app_hooks.c
    app/app.c
)
target_include_directories(CanopenTest
  PRIVATE
    app
)

#---
# add the test framework
#
target_sources(CanopenTest
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
target_include_directories(CanopenTest
  PRIVATE
    testfrm
)

#---
# select the test-suite files
#
target_sources(CanopenTest
  PRIVATE
    tests/get_ticks.c
    tests/get_min_time.c
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
target_include_directories(CanopenTest
  PRIVATE
    tests
)

#---
# specify the dependencies for this application
#
target_link_libraries(CanopenTest Canopen CanopenDriver)

#---
# enable warnings and tread as errors
#
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # tread all files as C files 
    target_compile_options(CanopenTest PRIVATE /TC)
  endif()
  # enable all warnings
  # target_compile_options(CanopenTest PRIVATE -Wall
  #                                            -Wextra
  #                                            -Wpedantic
  #                                            -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(CanopenTest PRIVATE /W4
                                             /WX)
endif()
