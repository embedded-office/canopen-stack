/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef APP_HOOKS_H_
#define APP_HOOKS_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CHK_CB_IF_RECEIVE(s,n)        TS_ASSERT((n) == (s)->IfCanReceive_Called)

#define CHK_CB_TPDO_TRANSMIT(s,n)     TS_ASSERT((n) == (s)->PdoTransmit_Called)

#define CHK_CB_RPDO_RECEIVE(s,n)      TS_ASSERT((n) == (s)->PdoReceive_Called)

#define CHK_CB_NMT_MODE_CHANGE(s,n)   TS_ASSERT((n) == (s)->NmtModeChange_Called)
#define CHK_CB_NMT_RESET_REQUEST(s,n) TS_ASSERT((n) == (s)->NmtResetRequest_Called)

#define CHK_CB_HBC_EVENT(s,n)         TS_ASSERT((n) == (s)->NmtHbConsEvent_Called)

#define CHK_CB_HBC_CHANGE(s,n)        TS_ASSERT((n) == (s)->NmtHbConsChange_Called)

#define CHK_CB_LSS_STORE(s,n)         TS_ASSERT((n) == (s)->LssStore_Called)
#define CHK_CB_LSS_ARG_BAUDRATE(s,n)  TS_ASSERT((n) == (s)->LssStore_ArgBaudrate)
#define CHK_CB_LSS_ARG_NODE_ID(s,n)   TS_ASSERT((n) == (s)->LssStore_ArgNodeId)
#define SET_CB_LSS_STORE_RETURN(s,v)  (s)->LssStore_Return=(int16_t)(v)

#define CHK_CB_CSDO_FINISHED(s,n)     TS_ASSERT((n) == (s)->AppCSdoCallback_Called)
#define CHK_CB_CSDO_CODE(s,c)         TS_ASSERT((c) == (s)->AppCSdoCallback_ArgCode)

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef struct TS_CALLBACK_T {
    uint32_t    NodeFatalError_Called;

    uint32_t    TmrLock_Called;
    uint32_t    TmrUnlock_Called;

    CO_IF_FRM  *IfCanReceive_ArgFrm;
    uint32_t    IfCanReceive_Called;

    CO_IF_FRM  *PdoTransmit_ArgFrm;
    uint32_t    PdoTransmit_Called;

    CO_IF_FRM  *PdoReceive_ArgFrm;
    uint32_t    PdoReceive_Called;
    int16_t     PdoReceive_Return;

    CO_NMT     *NmtModeChange_ArgNmt;
    CO_MODE     NmtModeChange_ArgMode;
    uint32_t    NmtModeChange_Called;

    CO_NMT       *NmtResetRequest_ArgNmt;
    CO_NMT_RESET  NmtResetRequest_ArgReset;
    uint32_t      NmtResetRequest_Called;

    CO_NMT     *NmtHbConsEvent_ArgNmt;
    uint8_t     NmtHbConsEvent_ArgNodeId;
    uint32_t    NmtHbConsEvent_Called;

    CO_NMT     *NmtHbConsChange_ArgNmt;
    uint8_t     NmtHbConsChange_ArgNodeId;
    CO_MODE     NmtHbConsChange_ArgState;
    uint32_t    NmtHbConsChange_Called;

    uint32_t    LssStore_ArgBaudrate;
    uint8_t     LssStore_ArgNodeId;
    uint32_t    LssStore_Called;
    int16_t     LssStore_Return;

    uint32_t   *LssLoad_ArgBaudrate;
    uint8_t    *LssLoad_ArgNodeId;
    uint32_t    LssLoad_Called;
    int16_t     LssLoad_Return;

    CO_PARA    *ParaDefault_ArgParaGrp;
    uint32_t    ParaDefault_Called;
    int16_t     ParaDefault_Return;

    CO_RPDO    *PdoSyncUpdate_ArgPdo;
    uint32_t    PdoSyncUpdate_Called;

    CO_CSDO    *AppCSdoCallback_ArgCSdo;
    uint16_t    AppCSdoCallback_ArgIndex;
    uint8_t     AppCSdoCallback_ArgSub;
    uint32_t    AppCSdoCallback_ArgCode;
    uint32_t    AppCSdoCallback_Called;

    CO_IF_FRM  *CORpdoWriteData_ArgFrm;
    uint8_t     CORpdoWriteData_ArgPos;
    uint8_t     CORpdoWriteData_ArgSize;
    CO_OBJ     *CORpdoWriteData_ArgObj;
    uint32_t    CORpdoWriteData_Called;

    CO_IF_FRM  *COTpdoReadData_ArgFrm;
    uint8_t     COTpdoReadData_ArgPos;
    uint8_t     COTpdoReadData_ArgSize;
    CO_OBJ     *COTpdoReadData_ArgObj;
    uint32_t    COTpdoReadData_Called;
} TS_CALLBACK;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_CallbackInit  (TS_CALLBACK *cb);
void TS_CallbackDeInit(void);

void COTmrLock  (void);
void COTmrUnlock(void);

/* Application callbacks provided via function argument: */
void TS_AppCSdoCallback(CO_CSDO *csdo, uint16_t index, uint8_t sub, uint32_t code);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
