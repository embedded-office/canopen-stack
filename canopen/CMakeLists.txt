#---
# define the canopen stack library
#
add_library(Canopen)

#---
# define interface
#
target_include_directories(Canopen
  PUBLIC
    config
    include
  PRIVATE
    source
)

#---
# specify the implementation files
#
target_sources(Canopen
  PRIVATE
    source/co_core.c
    source/co_dict.c
    source/co_emcy.c
    source/co_if_can.c
    source/co_if_nvm.c
    source/co_if_timer.c
    source/co_if.c
    source/co_lss.c
    source/co_nmt.c
    source/co_obj.c
    source/co_para.c
    source/co_pdo.c
    source/co_ssdo.c
    source/co_csdo.c
    source/co_sync.c
    source/co_tmr.c
    source/co_ver.c
)
