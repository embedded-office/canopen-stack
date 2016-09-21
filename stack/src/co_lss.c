/*
****************************************************************************************************
* (c) copyright by
*     Embedded Office GmbH & Co. KG       Tel : (07522) 97 00 08-0
*     Friedrich-Ebert-Str. 20/1           Fax : (07522) 97 00 08-99
*     D-88239 Wangen                      Mail: info@embedded-office.de
*                                         Web : http://www.embedded-office.de
*
* All rights reserved. Confidential and Proprietary. Protected by international copyright laws.
* Knowledge of the source code may not be used to write a similar product.
* This file may only be used in accordance with a license and should not be
* redistributed in any way.
****************************************************************************************************
*/
/*!
****************************************************************************************************
* \file     co_lss.c
*
* \brief    LAYER SETTING SERVICE
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_lss.c#3 $
*
*           This source file implements the CANopen layer setting service handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                            INCLUDES
****************************************************************************************************
*/

#include "co_core.h"

/*
****************************************************************************************************
*                                   LOCAL HELP FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_LSS_EN > 0
static CPU_INT16S CO_LssSwitchStateGlobal             (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssSwitchStateSelective_Vendor   (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssSwitchStateSelective_Product  (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssSwitchStateSelective_Revision (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssSwitchStateSelective_Serial   (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssActivateBitTiming             (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssConfigureBitTiming            (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssConfigureNodeId               (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssStoreConfiguration            (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssInquireAddress_Vendor         (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssInquireAddress_Product        (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssInquireAddress_Revision       (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssInquireAddress_Serial         (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssInquireNodeId                 (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_Vendor    (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_Product   (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_RevMin    (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_RevMax    (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_SerMin    (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssIdentifyRemoteSlave_SerMax    (CO_LSS *lss, CO_IF_FRM *frm);
static CPU_INT16S CO_LssNonConfiguredRemoteSlave      (CO_LSS *lss, CO_IF_FRM *frm);
#endif

/*
****************************************************************************************************
*                                        GLOBAL CONSTANTS
****************************************************************************************************
*/

#if CO_LSS_EN > 0
const CO_LSS_MAP COLssServices[CO_LSS_MAX_SID] = {
    {   4 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssSwitchStateGlobal },
    {  64 , CO_LSS_WAIT              , CO_LssSwitchStateSelective_Vendor },
    {  65 , CO_LSS_WAIT              , CO_LssSwitchStateSelective_Product },
    {  66 , CO_LSS_WAIT              , CO_LssSwitchStateSelective_Revision },
    {  67 , CO_LSS_WAIT              , CO_LssSwitchStateSelective_Serial },
    {  21 ,               CO_LSS_CONF, CO_LssActivateBitTiming },
    {  19 ,               CO_LSS_CONF, CO_LssConfigureBitTiming },
    {  17 ,               CO_LSS_CONF, CO_LssConfigureNodeId },
    {  23 ,               CO_LSS_CONF, CO_LssStoreConfiguration },
    {  90 ,               CO_LSS_CONF, CO_LssInquireAddress_Vendor },
    {  91 ,               CO_LSS_CONF, CO_LssInquireAddress_Product },
    {  92 ,               CO_LSS_CONF, CO_LssInquireAddress_Revision },
    {  93 ,               CO_LSS_CONF, CO_LssInquireAddress_Serial },
    {  94 ,               CO_LSS_CONF, CO_LssInquireNodeId },
    {  70 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_Vendor },
    {  71 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_Product },
    {  72 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_RevMin },
    {  73 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_RevMax },
    {  74 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_SerMin },
    {  75 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssIdentifyRemoteSlave_SerMax },
    {  76 , CO_LSS_WAIT | CO_LSS_CONF, CO_LssNonConfiguredRemoteSlave }
};

const CPU_INT32U CO_LssBaudTbl[CO_LSS_MAX_BAUD] = {
    CO_STD_BAUD_1M,                                   /* 1MBit                                    */
    CO_STD_BAUD_800K,                                 /* 800kBit                                  */
    CO_STD_BAUD_500K,                                 /* 500kBit                                  */
    CO_STD_BAUD_250K,                                 /* 250kBit                                  */
    CO_STD_BAUD_125K,                                 /* 125kBit                                  */
    0,                                                /* reserved                                 */
    CO_STD_BAUD_50K,                                  /* 50kBit                                   */
    CO_STD_BAUD_20K,                                  /* 20kBit                                   */
    CO_STD_BAUD_10K,                                  /* 10kBit                                   */
    0                                                 /* auto detection (not supported)           */
};
#endif

/*
****************************************************************************************************
*                                           FUNCTIONS
****************************************************************************************************
*/

#if CO_LSS_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief LSS INITIALIZATION
*
* \ingroup CANOPEN
*
*          This function initializes the LSS object.
*/
/*------------------------------------------------------------------------------------------------*/
void CO_LssInit (CO_LSS *lss, CO_NODE *node)
{
    CO_OBJ     *obj;
    CPU_INT32U  size;
    CPU_INT08U  subidx;

    if ((lss == 0) || (node == 0)) {                  /* see, if argument pointers are invalid    */
        CO_NodeFatalError();                          /* inform user                              */
        return;                                       /* abort initialization                     */
    }

    lss->Node        = node;                          /* link CANopen node information            */
    lss->Mode        = CO_LSS_WAIT;                   /* set LSS state to waiting state           */
    lss->Tmr         = -1;                            /* clear timer identifier                   */
    lss->Flags       = 0;                             /* clear flags                              */
    lss->CfgBaudrate = 0;                             /* clear buffer for bit timing              */
    lss->CfgNodeId   = 0;                             /* clear buffer for node id                 */
    lss->Step        = CO_LSS_SEL_VENDOR;             /* reset selection state machine            */

    for (subidx = 1; subidx <= 4; subidx++) {         /* loop through identity object             */
        obj = CODirFind(&node->Dir,                   /* check existence of identity subindex     */
                  CO_DEV(0x1018, subidx));
        if (obj == 0) {                               /* see, if subidx is not existing           */
            lss->Mode   = CO_LSS_EXIT;                /* exit LSS slave state machine             */
            node->Error = CO_ERR_CFG_1018;            /* indicate configuration error             */
            break;                                    /* abort after detecting first error        */
        } else {
            size = COObjGetSize(obj, 0);              /* get size of subidx entry                 */
            if (size != CO_LONG) {                    /* see, if size is not correct              */
                lss->Mode = CO_LSS_EXIT;              /* exit LSS slave state machine             */
                node->Error = CO_ERR_CFG_1018;        /* indicate configuration error             */
                break;                                /* abort after detecting first error        */
            }
        }
    }
}
#endif

#if CO_LSS_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief LSS MESSAGE CHECK
*
* \ingroup LSS
*
*          This functions checks a received frame to be a LSS message. If this is a valid LSS
*          frame, the frame will be processed and replaced by the corresponding response frame.
*
* \param   frm      The CAN frame
*
* \retval  =0:      no LSS request
* \retval  <0:      ignored LSS request
* \retval  >0:      successful processed LSS request
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_LssCheck (CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT16S  result = 0;                           /* Local: function result: 'no LSS request' */
    const CO_LSS_MAP *map;                            /* Local: LSS service map                   */
    CPU_INT08U  sid;                                  /* Local: loop variable (service index)     */
    CPU_INT08U  cmd;                                  /* Local: LSS request command specifier     */
                                                      /*------------------------------------------*/
    if (CO_GET_COBID(frm) == CO_LSS_RX_ID) {          /* see if frame is a LSS request            */
        result = -1;                                  /* set return to 'ignore' LSS request       */
        cmd    = CO_GET_BYTE(frm, 0);                 /* extract command specifier                */
        for (sid = 0; sid < CO_LSS_MAX_SID; sid++) {  /* loop through all possible services       */
            map = &COLssServices[sid];                /* set service map configuration            */
            if (map->Request == cmd) {                /* see, if command is found in map          */
                if ((map->Allowed & lss->Mode) > 0) { /* see, if this command is allowed          */
                    result = map->Service(lss, frm);  /* execute the service command              */
                }
                break;                                /* break loop, because command was found    */
            }
        }
    }
    return (result);
}
#endif

/*
****************************************************************************************************
*                                        LOCAL FUNCTIONS
****************************************************************************************************
*/

#if CO_LSS_EN > 0
static CPU_INT16S CO_LssSwitchStateGlobal(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT08U mode;                                  /* Local: commanded mode                    */

    mode = CO_GET_BYTE(frm, 1);                       /* extract switching mode                   */
    if (mode == CO_LSS_CMD_CONF) {                    /* see, if switching mode is 'configuration'*/
        lss->Mode = CO_LSS_CONF;                      /* switch to configuration mode             */
    } else {
        lss->Mode = CO_LSS_WAIT;                      /* switch to waiting mode                   */
    }

    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssSwitchStateSelective_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    lss->Step = CO_LSS_SEL_VENDOR;                    /* this service always restarts selection   */

    select    = CO_GET_LONG(frm, 1);                  /* extract selection from LSS request       */
    err       = CODirRdLong(&lss->Node->Dir,          /* read value from identity object          */
                    CO_DEV(0x1018, 1), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of vendor-id         */
        lss->Step = CO_LSS_SEL_PRODUCT;               /* ok, switch to next step                  */
    }

    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssSwitchStateSelective_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_SEL_PRODUCT) {            /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_SEL_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
                 CO_DEV(0x1018, 2), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of product-code      */
        lss->Step = CO_LSS_SEL_REVISION;              /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssSwitchStateSelective_Revision(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_SEL_REVISION) {           /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_SEL_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
                 CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of revision          */
        lss->Step = CO_LSS_SEL_SERIAL;                /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssSwitchStateSelective_Serial(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S result = -1;                           /* Local: indicate: no LSS response         */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_SEL_SERIAL) {             /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_SEL_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
                 CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of serial number     */
        CO_SET_LONG (frm, 0L, 0);                     /* clear data in frame                      */
        CO_SET_LONG (frm, 0L, 4);
        CO_SET_BYTE (frm, CO_LSS_RES_SEL_OK, 0);      /* generate response frame                  */
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        lss->Mode = CO_LSS_CONF;                      /* switch to configuration mode             */
        result    = 1;                                /* indicate success with response           */
    }
    return result;
}

void CO_LssActivateBitTiming_SwitchDelay (void *arg)
{
	CO_LSS *lss;

	lss = (CO_LSS *)arg;

    if (lss->Step == 1) {                             /* First step: switching baudrate           */
    	CO_IfInit(&lss->Node->If, lss->Node);         /* re-initialize CAN controller             */
        CO_IfEnable(&lss->Node->If, lss->CfgBaudrate);/* enable CAN bus with configured baudrate  */
        lss->Step = 2;                                /* set timer delay - step 2                 */
    } else {
        CONmtSetMode(&lss->Node->Nmt, CO_PREOP);      /* switch to pre-operational                */
        COTmrDelete(&lss->Node->Tmr, lss->Tmr);       /* delete high speed timer                  */
        lss->Tmr  = -1;                               /* remove timer identifier                  */
        lss->Step = 0;
    }
}

static CPU_INT16S CO_LssActivateBitTiming(CO_LSS *lss, CO_IF_FRM *frm)
{
	CPU_INT16U delay;

    delay = CO_GET_WORD(frm, 1);                      /* extract switch delay from LSS request    */

    CONmtSetMode(&lss->Node->Nmt, CO_INIT);           /* set device to INIT mode                  */
    COIfClose(&lss->Node->If);                        /* de-activate and flush CAN bus            */

    lss->Step = 1;                                    /* set timer delay - step 1                 */
    lss->Tmr  = COTmrCreate(&lss->Node->Tmr,          /* create high speed timer                  */
    		    0,
    		    CO_TMR_TICKS(delay),
                CO_LssActivateBitTiming_SwitchDelay,
                (void*)lss);
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssConfigureBitTiming(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT16S result = 1;                            /* Local: indicate: LSS response            */
    CPU_INT08U table;                                 /* Local: baudrate table selector           */
    CPU_INT08U baudId;                                /* Local: selected baudrate index           */

    table  = CO_GET_BYTE(frm, 1);                     /* extract table selector from LSS request  */
    baudId = CO_GET_BYTE(frm, 2);                     /* extract baudrate index from LSS request  */
    if (table == 0) {                                 /* see, if CiA standard baudrate table      */
        if (baudId < CO_LSS_MAX_BAUD) {               /* see, if baudrate index is in range       */
            lss->CfgBaudrate = CO_LssBaudTbl[baudId]; /* set selected standard baudrate           */
            if (lss->CfgBaudrate != 0) {
                CO_SET_BYTE(frm, 0, 1);               /* set success in response                  */
            } else {                                  /* baudrate is not supported                */
                CO_SET_BYTE(frm, 1, 1);               /* set error code in response               */
            }
        } else {                                      /* baudrate index out of range              */
            CO_SET_BYTE(frm, 1, 1);                   /* set error code in response               */
        }
    } else {                                          /* manufacturer tables are not supported    */
        CO_SET_BYTE(frm, 1, 1);                       /* set error code in response               */
    }
    CO_SET_BYTE (frm, 0, 2);                          /* clear byte 2                             */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return result;
}

static CPU_INT16S CO_LssConfigureNodeId(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT16S result = 1;                            /* Local: indicate: LSS response            */
    CPU_INT08U nodeId;

    nodeId = CO_GET_BYTE(frm, 1);                     /* extract node-ID from LSS request         */
    if ( ((nodeId >= 1) && (nodeId <= 127)) ||        /* see, if node-Id is in valid range        */
          (nodeId == 0xFF) ) {                        /* or if node-ID indicates invalid          */
        lss->CfgNodeId = nodeId;                      /* buffer newly configured node-ID          */
        CO_SET_BYTE(frm, 0, 1);                       /* set success in response                  */
    } else {                                          /* otherwise: node-ID has illegal value     */
        CO_SET_BYTE(frm, 1, 1);                       /* set error code in response               */
    }
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return result;
}

static CPU_INT16S CO_LssStoreConfiguration(CO_LSS *lss, CO_IF_FRM *frm)
{
#if CO_CB_LSS_STORE_EN > 0
    CPU_INT16S err;                                   /* Local: error code                        */

    err = CO_LssStore(lss->CfgBaudrate,               /* store configured values                  */
                      lss->CfgNodeId);
    if (err == CO_ERR_NONE) {                         /* see, if storage is successful            */
        lss->Flags |= CO_LSS_STORED;                  /* set flag, successful storage             */
        CO_SET_BYTE(frm, 0, 1);                       /* set success                              */
    } else {
        CO_SET_BYTE(frm, 2, 1);                       /* set error: error in storage medium       */
    }
#else
    CO_SET_BYTE(frm, 1, 1);                           /* set error: storage not supported         */
#endif
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssInquireAddress_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U ident = 0;                             /* Local: read identity value out of object */

    (void)CODirRdLong(&lss->Node->Dir,                /* read value from identity object          */
        CO_DEV(0x1018, 1), &ident);
    CO_SET_LONG (frm, ident, 1);                      /* set identity value in response           */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssInquireAddress_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U ident = 0;                             /* Local: read identity value out of object */

    (void)CODirRdLong(&lss->Node->Dir,                /* read value from identity object          */
        CO_DEV(0x1018, 2), &ident);
    CO_SET_LONG(frm, ident, 1);                       /* set identity value in response           */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssInquireAddress_Revision(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U ident = 0;                             /* Local: read identity value out of object */

    (void)CODirRdLong(&lss->Node->Dir,                /* read value from identity object          */
        CO_DEV(0x1018, 3), &ident);
    CO_SET_LONG(frm, ident, 1);                       /* set identity value in response           */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssInquireAddress_Serial(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U ident = 0;                             /* Local: read identity value out of object */

    (void)CODirRdLong(&lss->Node->Dir,                /* read value from identity object          */
        CO_DEV(0x1018, 4), &ident);
    CO_SET_LONG(frm, ident, 1);                       /* set identity value in response           */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssInquireNodeId(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U nodeId;                                /* Local: read node ID of device            */

    nodeId = CONmtGetNodeId(&lss->Node->Nmt);         /* get current node-ID                      */
    CO_SET_BYTE (frm, nodeId, 1);                     /* set node-ID value in response            */
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;                                         /* indicate: LSS response                   */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    lss->Step = CO_LSS_REM_VENDOR;                    /* this service always restarts identify    */

    select    = CO_GET_LONG(frm, 1);                  /* extract selection from LSS request       */
    err       = CODirRdLong(&lss->Node->Dir,          /* read value from identity object          */
        CO_DEV(0x1018, 1), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of vendor-id         */
        lss->Step = CO_LSS_REM_PRODUCT;               /* ok, switch to next step                  */
    }

    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_REM_PRODUCT) {            /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_REM_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err = CODirRdLong(&lss->Node->Dir,                /* read value from identity object          */
        CO_DEV(0x1018, 2), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {  /* see, if exact match of product-code      */
        lss->Step = CO_LSS_REM_REVISION_MIN;          /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_RevMin(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_REM_REVISION_MIN) {       /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_REM_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
        CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select <= ident)) {  /* see, if identity is greater or equal     */
        lss->Step = CO_LSS_REM_REVISION_MAX;          /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_RevMax(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_REM_REVISION_MAX) {       /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_REM_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
        CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select >= ident)) {  /* see, if identity is lower or equal       */
        lss->Step = CO_LSS_REM_SERIAL_MIN;            /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_SerMin(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */

    if (lss->Step != CO_LSS_REM_SERIAL_MIN) {         /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_REM_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
        CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select <= ident)) {  /* see, if identity is greater or equal     */
        lss->Step = CO_LSS_REM_SERIAL_MAX;            /* ok, switch to next step                  */
    }
    return -1;                                        /* indicate: no response                    */
}

static CPU_INT16S CO_LssIdentifyRemoteSlave_SerMax(CO_LSS *lss, CO_IF_FRM *frm)
{
    CPU_INT32U select;                                /* Local: received selection value          */
    CPU_INT32U ident;                                 /* Local: value from device identity object */
    CPU_INT16S err;                                   /* Local: error code                        */
    CPU_INT16S result = -1;                           /* Local: indicate no response              */

    if (lss->Step != CO_LSS_REM_SERIAL_MAX) {         /* see, if order of selection is not correct*/
        lss->Step = CO_LSS_REM_VENDOR;                /* restarts selection                       */
        return -1;                                    /* indicate: no response                    */
    }

    select = CO_GET_LONG(frm, 1);                     /* extract selection from LSS request       */
    err    = CODirRdLong(&lss->Node->Dir,             /* read value from identity object          */
        CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select >= ident)) {  /* see, if identity is lower or equal       */
        CO_SET_LONG(frm, 0L, 0);                      /* clear data in frame                      */
        CO_SET_LONG(frm, 0L, 4);
        CO_SET_BYTE(frm, CO_LSS_RES_SLAVE, 0);        /* generate response frame                  */
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        result = 1;                                   /* indicate success with response           */
    }
    return result;
}

static CPU_INT16S CO_LssNonConfiguredRemoteSlave(CO_LSS *lss, CO_IF_FRM *frm)
{
    CO_NODE    *node;
    CPU_INT16S  result = -1;                          /* Local: indicate no response              */

    node = lss->Node;
    if ( (node->NodeId == (CPU_INT08U)0xff) &&        /* see, if nodeID is invalid and            */
         ((lss->Flags & CO_LSS_STORED) != 0) ) {      /* no configured nodeID is stored           */
        CO_SET_LONG(frm, 0L, 0);                      /* clear data in frame                      */
        CO_SET_LONG(frm, 0L, 4);
        CO_SET_BYTE(frm, CO_LSS_RES_UNCONF, 0);       /* generate response frame                  */
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        result = 1;                                   /* indicate: LSS response                   */
    }
    return result;
}
#endif

#if CO_LSS_EN > 0
#if CO_CB_LSS_STORE_EN == 0
CPU_INT32U RamStorage_Baudrate = 0xFFFFFFFF;
CPU_INT08U RamStorage_NodeId   = 0xFF;

/*------------------------------------------------------------------------------------------------*/
/*! \brief LSS CONFIGURATION STORE CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called with new configuration data, which is set by the LSS service.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_LSS_STORE_EN.
*
* \note    When disabling the application callback function, this example implementation is
*          enabled, but not called. In fact: disabling the application function will remove the
*          callback function call in the interface receive processing.
*
* \note    The CAN frame pointer is checked to be valid before calling this function.
*
* \param[in]       frm               The received CAN frame
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_LssStore(CPU_INT32U baudrate, CPU_INT08U nodeId)
{
	if (baudrate != 0) {
        RamStorage_Baudrate = baudrate;
	}
	if (nodeId != 0) {
        RamStorage_NodeId   = nodeId;
	}
    return (CO_ERR_NONE);
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief LSS CONFIGURATION LOAD CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called during reset communication to load the stored LSS configuration.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_LSS_STORE_EN.
*
* \note    When disabling the application callback function, this example implementation is
*          enabled, but not called. In fact: disabling the application function will remove the
*          callback function call in the interface receive processing.
*
* \note    The CAN frame pointer is checked to be valid before calling this function.
*
* \param[in]       frm               The received CAN frame
*/
/*------------------------------------------------------------------------------------------------*/
CPU_INT16S CO_LssLoad(CPU_INT32U *baudrate, CPU_INT08U *nodeId)
{
    if (RamStorage_Baudrate != 0xFFFFFFFFL) {
        *baudrate = RamStorage_Baudrate;
    }
    if (RamStorage_NodeId != 0xFF) {
        *nodeId = RamStorage_NodeId;                  
    }
    return (CO_ERR_NONE);
}
#endif
#endif
