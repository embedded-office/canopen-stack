/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, http://www.embedded-office.com
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see < http ://www.can-cia.org/>.
*
* CANopenStack is free and open source software: you can redistribute
* it and / or modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* GLOBAL CONSTANTS
******************************************************************************/

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

const uint32_t CO_LssBaudTbl[CO_LSS_MAX_BAUD] = {
    CO_STD_BAUD_1M,              /* 1MBit                                    */
    CO_STD_BAUD_800K,            /* 800kBit                                  */
    CO_STD_BAUD_500K,            /* 500kBit                                  */
    CO_STD_BAUD_250K,            /* 250kBit                                  */
    CO_STD_BAUD_125K,            /* 125kBit                                  */
    0,                           /* reserved                                 */
    CO_STD_BAUD_50K,             /* 50kBit                                   */
    CO_STD_BAUD_20K,             /* 20kBit                                   */
    CO_STD_BAUD_10K,             /* 10kBit                                   */
    0                            /* auto detection (not supported)           */
};

/******************************************************************************
* FUNCTIONS
******************************************************************************/

void CO_LssInit (CO_LSS *lss, CO_NODE *node)
{
    CO_OBJ   *obj;
    uint32_t  size;
    uint8_t   subidx;

    if ((lss == 0) || (node == 0)) {
        CONodeFatalError();
        return;
    }

    lss->Node        = node;
    lss->Mode        = CO_LSS_WAIT;
    lss->Tmr         = -1;
    lss->Flags       = 0;
    lss->CfgBaudrate = 0;
    lss->CfgNodeId   = 0;
    lss->Step        = CO_LSS_SEL_VENDOR;

    for (subidx = 1; subidx <= 4; subidx++) {
        obj = CODirFind(&node->Dir, CO_DEV(0x1018, subidx));
        if (obj == 0) {
            lss->Mode   = CO_LSS_EXIT;
            node->Error = CO_ERR_CFG_1018;
            break;
        } else {
            size = COObjGetSize(obj, 0);
            if (size != CO_LONG) {
                lss->Mode = CO_LSS_EXIT;
                node->Error = CO_ERR_CFG_1018;
                break;
            }
        }
    }
}

int16_t CO_LssCheck (CO_LSS *lss, CO_IF_FRM *frm)
{
    int16_t           result = 0;
    const CO_LSS_MAP *map;
    uint8_t           sid;
    uint8_t           cmd;

    if (CO_GET_COBID(frm) == CO_LSS_RX_ID) {
        result = -1;
        cmd    = CO_GET_BYTE(frm, 0);
        for (sid = 0; sid < CO_LSS_MAX_SID; sid++) {
            map = &COLssServices[sid];
            if (map->Request == cmd) {
                if ((map->Allowed & lss->Mode) > 0) {
                    result = map->Service(lss, frm);
                }
                break;
            }
        }
    }

    return (result);
}

int16_t CO_LssSwitchStateGlobal(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint8_t mode;

    mode = CO_GET_BYTE(frm, 1);
    if (mode == CO_LSS_CMD_CONF) {
        lss->Mode = CO_LSS_CONF;
    } else {
        lss->Mode = CO_LSS_WAIT;
    }

    return -1;
}

int16_t CO_LssSwitchStateSelective_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    lss->Step = CO_LSS_SEL_VENDOR;

    select    = CO_GET_LONG(frm, 1);
    err       = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 1), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        lss->Step = CO_LSS_SEL_PRODUCT;
    }

    return -1;
}

int16_t CO_LssSwitchStateSelective_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_SEL_PRODUCT) {
        lss->Step = CO_LSS_SEL_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 2), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        lss->Step = CO_LSS_SEL_REVISION;
    }

    return -1;
}

int16_t CO_LssSwitchStateSelective_Revision(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_SEL_REVISION) {
        lss->Step = CO_LSS_SEL_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        lss->Step = CO_LSS_SEL_SERIAL;
    }

    return -1;
}

int16_t CO_LssSwitchStateSelective_Serial(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  result = -1;
    int16_t  err;

    if (lss->Step != CO_LSS_SEL_SERIAL) {
        lss->Step = CO_LSS_SEL_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        CO_SET_LONG (frm, 0L, 0);
        CO_SET_LONG (frm, 0L, 4);
        CO_SET_BYTE (frm, CO_LSS_RES_SEL_OK, 0);
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        lss->Mode = CO_LSS_CONF;
        result    = 1;
    }

    return result;
}

void CO_LssActivateBitTiming_SwitchDelay (void *arg)
{
    CO_LSS *lss;

    lss = (CO_LSS *)arg;

    if (lss->Step == 1) {
        COIfInit(&lss->Node->If, lss->Node);
        COIfEnable(&lss->Node->If, lss->CfgBaudrate);
        lss->Step = 2;
    } else {
        CONmtSetMode(&lss->Node->Nmt, CO_PREOP);
        COTmrDelete(&lss->Node->Tmr, lss->Tmr);
        lss->Tmr  = -1;
        lss->Step = 0;
    }
}

int16_t CO_LssActivateBitTiming(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint16_t delay;

    delay = CO_GET_WORD(frm, 1);

    CONmtSetMode(&lss->Node->Nmt, CO_INIT);
    COIfClose(&lss->Node->If);

    lss->Step = 1;
    lss->Tmr  = COTmrCreate(&lss->Node->Tmr,
                0,
                CO_TMR_TICKS(delay),
                CO_LssActivateBitTiming_SwitchDelay,
                (void*)lss);
    return -1;
}

int16_t CO_LssConfigureBitTiming(CO_LSS *lss, CO_IF_FRM *frm)
{
    int16_t result = 1;
    uint8_t table;
    uint8_t baudId;

    table  = CO_GET_BYTE(frm, 1);
    baudId = CO_GET_BYTE(frm, 2);
    if (table == 0) {
        if (baudId < CO_LSS_MAX_BAUD) {
            lss->CfgBaudrate = CO_LssBaudTbl[baudId];
            if (lss->CfgBaudrate != 0) {
                CO_SET_BYTE(frm, 0, 1);
            } else {
                CO_SET_BYTE(frm, 1, 1);
            }
        } else {
            CO_SET_BYTE(frm, 1, 1);
        }
    } else {
        CO_SET_BYTE(frm, 1, 1);
    }
    CO_SET_BYTE (frm, 0, 2);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return result;
}

int16_t CO_LssConfigureNodeId(CO_LSS *lss, CO_IF_FRM *frm)
{
    int16_t result = 1;
    uint8_t nodeId;

    nodeId = CO_GET_BYTE(frm, 1);
    if ( ((nodeId >= 1) && (nodeId <= 127)) || (nodeId == 0xFF) ) {
        lss->CfgNodeId = nodeId;
        CO_SET_BYTE(frm, 0, 1);
    } else {
        CO_SET_BYTE(frm, 1, 1);
    }
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return result;
}

int16_t CO_LssStoreConfiguration(CO_LSS *lss, CO_IF_FRM *frm)
{
    int16_t err;

    err = CO_LssStore(lss->CfgBaudrate, lss->CfgNodeId);
    if (err == CO_ERR_NONE) {
        lss->Flags |= CO_LSS_STORED;
        CO_SET_BYTE(frm, 0, 1);
    } else {
        CO_SET_BYTE(frm, 2, 1);
    }

    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssInquireAddress_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t ident = 0;

    (void)CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 1), &ident);
    CO_SET_LONG (frm, ident, 1);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssInquireAddress_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t ident = 0;

    (void)CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 2), &ident);
    CO_SET_LONG(frm, ident, 1);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssInquireAddress_Revision(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t ident = 0;

    (void)CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 3), &ident);
    CO_SET_LONG(frm, ident, 1);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssInquireAddress_Serial(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t ident = 0;

    (void)CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 4), &ident);
    CO_SET_LONG(frm, ident, 1);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssInquireNodeId(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t nodeId;

    nodeId = CONmtGetNodeId(&lss->Node->Nmt);
    CO_SET_BYTE (frm, nodeId, 1);
    CO_SET_COBID(frm, CO_LSS_TX_ID);

    return 1;
}

int16_t CO_LssIdentifyRemoteSlave_Vendor(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    lss->Step = CO_LSS_REM_VENDOR;

    select    = CO_GET_LONG(frm, 1);
    err       = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 1), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        lss->Step = CO_LSS_REM_PRODUCT;
    }

    return -1;
}

int16_t CO_LssIdentifyRemoteSlave_Product(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_REM_PRODUCT) {
        lss->Step = CO_LSS_REM_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 2), &ident);
    if ((err == CO_ERR_NONE) && (select == ident)) {
        lss->Step = CO_LSS_REM_REVISION_MIN;
    }
    return -1;
}

int16_t CO_LssIdentifyRemoteSlave_RevMin(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_REM_REVISION_MIN) {
        lss->Step = CO_LSS_REM_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select <= ident)) {
        lss->Step = CO_LSS_REM_REVISION_MAX;
    }
    return -1;
}

int16_t CO_LssIdentifyRemoteSlave_RevMax(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_REM_REVISION_MAX) {
        lss->Step = CO_LSS_REM_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 3), &ident);
    if ((err == CO_ERR_NONE) && (select >= ident)) {
        lss->Step = CO_LSS_REM_SERIAL_MIN;
    }
    return -1;
}

int16_t CO_LssIdentifyRemoteSlave_SerMin(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;

    if (lss->Step != CO_LSS_REM_SERIAL_MIN) {
        lss->Step = CO_LSS_REM_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select <= ident)) {
        lss->Step = CO_LSS_REM_SERIAL_MAX;
    }
    return -1;
}

int16_t CO_LssIdentifyRemoteSlave_SerMax(CO_LSS *lss, CO_IF_FRM *frm)
{
    uint32_t select;
    uint32_t ident;
    int16_t  err;
    int16_t  result = -1;

    if (lss->Step != CO_LSS_REM_SERIAL_MAX) {
        lss->Step = CO_LSS_REM_VENDOR;
        return -1;
    }

    select = CO_GET_LONG(frm, 1);
    err    = CODirRdLong(&lss->Node->Dir, CO_DEV(0x1018, 4), &ident);
    if ((err == CO_ERR_NONE) && (select >= ident)) {
        CO_SET_LONG(frm, 0L, 0);
        CO_SET_LONG(frm, 0L, 4);
        CO_SET_BYTE(frm, CO_LSS_RES_SLAVE, 0);
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        result = 1;
    }
    return result;
}

int16_t CO_LssNonConfiguredRemoteSlave(CO_LSS *lss, CO_IF_FRM *frm)
{
    CO_NODE *node;
    int16_t  result = -1;

    node = lss->Node;
    if ( (node->NodeId == (uint8_t)0xff) &&
         ((lss->Flags & CO_LSS_STORED) != 0) ) {
        CO_SET_LONG(frm, 0L, 0);
        CO_SET_LONG(frm, 0L, 4);
        CO_SET_BYTE(frm, CO_LSS_RES_UNCONF, 0);
        CO_SET_COBID(frm, CO_LSS_TX_ID);
        result = 1;
    }
    return result;
}


uint32_t RamStorage_Baudrate = 0xFFFFFFFF;
uint8_t RamStorage_NodeId   = 0xFF;
int16_t CO_LssStore(uint32_t baudrate, uint8_t nodeId)
{
    if (baudrate != 0) {
        RamStorage_Baudrate = baudrate;
    }
    if (nodeId != 0) {
        RamStorage_NodeId   = nodeId;
    }
    return (CO_ERR_NONE);
}

int16_t CO_LssLoad(uint32_t *baudrate, uint8_t *nodeId)
{
    if (RamStorage_Baudrate != 0xFFFFFFFFL) {
        *baudrate = RamStorage_Baudrate;
    }
    if (RamStorage_NodeId != 0xFF) {
        *nodeId = RamStorage_NodeId;                  
    }
    return (CO_ERR_NONE);
}
