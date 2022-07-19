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
