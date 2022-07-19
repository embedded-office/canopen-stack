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
    include/types/basic
    include/types/cia301
  PRIVATE
    source
)

#---
# specify the implementation files
#
target_sources(Canopen
  PRIVATE
    source/co_core.c
    source/co_csdo.c
    source/co_dict.c
    source/co_emcy.c
    source/co_if_can.c
    source/co_if_nvm.c
    source/co_if_timer.c
    source/co_if.c
    source/co_lss.c
    source/co_nmt.c
    source/co_obj.c
    source/co_pdo.c
    source/co_ssdo.c
    source/co_csdo.c
    source/co_sync.c
    source/co_tmr.c
    source/co_ver.c
    # basic type modules
    source/types/basic/co_domain.c
    source/types/basic/co_string.c
    source/types/basic/co_unsigned8.c
    source/types/basic/co_unsigned16.c
    source/types/basic/co_unsigned32.c
    # cia301 type modules
    source/types/cia301/co_emcy_hist.c
    source/types/cia301/co_emcy_id.c
    source/types/cia301/co_hb_cons.c
    source/types/cia301/co_hb_prod.c
    source/types/cia301/co_para_ctrl.c
    source/types/cia301/co_pdo_event.c
    source/types/cia301/co_pdo_id.c
    source/types/cia301/co_pdo_map.c
    source/types/cia301/co_pdo_num.c
    source/types/cia301/co_pdo_type.c
    source/types/cia301/co_sdo_id.c
    source/types/cia301/co_sync_cycle.c
    source/types/cia301/co_sync_id.c
)
