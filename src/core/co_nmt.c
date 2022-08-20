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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_nmt.h"

#include "co_core.h"

/******************************************************************************
* PRIVATE CONSTANTS
******************************************************************************/

/*! \brief NMT MODE ALLOWED OBJECTS
*
*    This constant codes are used to identify the allowed objects within the
*    specific NMT state machine mode.
*/
static const uint8_t CONmtModeObj[CO_MODE_NUM] = {
    ( 0 ),                       /*!< objects in invalid mode                */

    ( CO_BOOT_ALLOWED ),         /*!< objects in initialization mode         */

    (  CO_SDO_ALLOWED |          /*!< objects in pre-operational mode        */
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_PDO_ALLOWED |          /*!< objects in operational mode            */
       CO_SDO_ALLOWED |
      CO_SYNC_ALLOWED |
      CO_TIME_ALLOWED |
      CO_EMCY_ALLOWED |
       CO_NMT_ALLOWED ),

    (  CO_NMT_ALLOWED )          /*!< objects in stop mode                   */
};

/*! \brief NMT MODE ENCODING
*
*    This constant codes are used to encode the NMT state machine mode within
*    heartbeat acc. the standard.
*/
static const uint8_t CONmtModeCode[CO_MODE_NUM] = {
    255,                         /*!< encoding for invalid mode              */
    0,                           /*!< encoding for initialization mode       */
    127,                         /*!< encoding for pre-operational mode      */
    5,                           /*!< encoding for operational mode          */
    4                            /*!< encoding for stopped mode              */
};

/******************************************************************************
* PROTECTED API FUNCTIONS
******************************************************************************/

void CONmtReset(CO_NMT *nmt, CO_NMT_RESET type)
{
    CO_OBJ *store;
    uint8_t nobootup = 1;
    CO_ERR  err;

    ASSERT_PTR_FATAL(nmt);

    if (nmt->Mode != CO_INIT) {
        CONmtSetMode(nmt, CO_INIT);
        nobootup = 0;
    }

    /* check for parameter storage */
    store = CODictFind(&(nmt->Node->Dict), CO_DEV(0x1010, 0));

    if (type == CO_RESET_NODE) {
        if (store != NULL) {
            err = COObjReset(store, nmt->Node, CO_RESET_NODE);
            if (err != CO_ERR_NONE) {
                nmt->Node->Error = err;
            }
        }
    }

    if (type <= CO_RESET_COM) {
        if (store != NULL) {
            err = COObjReset(store, nmt->Node, CO_RESET_COM);
            if (err != CO_ERR_NONE) {
                nmt->Node->Error = err;
            }
        }

#if USE_LSS
        err = COLssLoad(&nmt->Node->Baudrate, &nmt->Node->NodeId);
        if (err != CO_ERR_NONE) {
            nmt->Node->Error = CO_ERR_LSS_LOAD;
        }
        COLssInit(&nmt->Node->Lss, nmt->Node);
#endif //USE_LSS
        COTmrClear(&nmt->Node->Tmr);
        CONmtInit(nmt, nmt->Node);
        COSdoInit(nmt->Node->Sdo, nmt->Node);
        COIfCanReset(&nmt->Node->If);
        COEmcyReset(&nmt->Node->Emcy, 1);
        COSyncInit(&nmt->Node->Sync, nmt->Node);
        if (nobootup == 0) {
            CONmtBootup(nmt);
        }
    }
}

void CONmtInit(CO_NMT *nmt, CO_NODE *node)
{
    ASSERT_PTR_FATAL(nmt);
    ASSERT_PTR_FATAL(node);

    nmt->Node = node;
    nmt->HbCons = NULL;
    CONmtSetMode(nmt, CO_INIT);
}

void CONmtBootup(CO_NMT *nmt)
{
    CO_IF_FRM frm;

    if (nmt->Mode == CO_INIT) {
        CONmtSetMode(nmt, CO_PREOP);

        CO_SET_ID  (&frm, 1792 + nmt->Node->NodeId);
        CO_SET_DLC (&frm, 1);
        CO_SET_BYTE(&frm, 0, 0);

        (void)COIfCanSend(&nmt->Node->If, &frm);
    }
}

int16_t CONmtCheck(CO_NMT *nmt, CO_IF_FRM *frm)
{
    int16_t result = -1;

    if (frm->Identifier == 0) {
        result = 0;
        if ((frm->Data[1] == nmt->Node->NodeId) ||
            (frm->Data[1] == 0u               )) {
            switch(frm->Data[0u]) {
                case 1:
                    CONmtSetMode(nmt, CO_OPERATIONAL);
                    break;
                case 2:
                    CONmtSetMode(nmt, CO_STOP);
                    break;
                case 128:
                    CONmtSetMode(nmt, CO_PREOP);
                    break;
                case 129:
                    CONmtReset(nmt, CO_RESET_NODE);
                    CONmtResetRequest(nmt, CO_RESET_NODE);
                    break;
                case 130:
                    CONmtReset(nmt, CO_RESET_COM);
                    CONmtResetRequest(nmt, CO_RESET_COM);
                    break;
                default:
                    break;
            }
        }
    }
    return(result);
}

/******************************************************************************
* PUBLIC API FUNCTIONS
******************************************************************************/

void CONmtSetMode(CO_NMT *nmt, CO_MODE mode)
{
    ASSERT_PTR_FATAL(nmt);

    if (nmt->Mode != mode) {
        if (mode == CO_OPERATIONAL) {
            
            COTPdoInit(nmt->Node->TPdo, nmt->Node);
            CORPdoInit(nmt->Node->RPdo, nmt->Node);
        }
        CONmtModeChange(nmt, mode);
    }
    nmt->Mode    = mode;
    nmt->Allowed = CONmtModeObj[mode];
}

CO_MODE CONmtGetMode(CO_NMT *nmt)
{
    ASSERT_PTR_FATAL_ERR(nmt, CO_INVALID);

    return (nmt->Mode);
}

void CONmtSetNodeId(CO_NMT *nmt, uint8_t nodeId)
{
    CO_MODE mode;

    ASSERT_PTR_FATAL(nmt);
    ASSERT_PTR(nmt->Node);
    ASSERT_NOT(nodeId, 0);

    mode = nmt->Mode;
    if (mode != CO_INIT) {
        nmt->Node->Error = CO_ERR_NMT_MODE;
    } else {
        nmt->Node->NodeId = nodeId;
    }
}

uint8_t CONmtGetNodeId(CO_NMT *nmt)
{
    uint8_t result = 0;

    ASSERT_PTR_FATAL_ERR(nmt, 0);

    if (nmt->Node != NULL) {
        result = nmt->Node->NodeId;
    }
    return result;
}

CO_MODE CONmtModeDecode(uint8_t code)
{
    CO_MODE    result = CO_INVALID;
    uint8_t id;

    for (id = 0; id < CO_MODE_NUM; id++) {
        if (CONmtModeCode[id] == code) {
            result = (CO_MODE)id;
        }
    }
    return (result);
}

uint8_t CONmtModeEncode(CO_MODE mode)
{
    uint8_t result = CONmtModeCode[CO_INVALID];

    if (mode < CO_MODE_NUM) {
        result = CONmtModeCode[mode];
    }
    return (result);
}
