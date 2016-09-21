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
* \file     co_lss.h
*
* \brief    LAYER SETTING SERVICE SLAVE DEFINITIONS
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_lss.h#3 $
*
*           This include file defines the symbolic constants and data types for the CANopen LSS
*           slave handling.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_LSS_H_
#define CO_LSS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_obj.h"
#include "co_if.h"

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_LSS_MAX_SID           21                   /*!< number of LSS services                 */
#define CO_LSS_MAX_BAUD          10                   /*!< number of standard baudrates           */

#define CO_LSS_RX_ID           2021                   /*!< LSS request identifier                 */
#define CO_LSS_TX_ID           2020                   /*!< LSS response identifier                */

#define CO_LSS_WAIT            0x01                   /*!< waiting mode                           */
#define CO_LSS_CONF            0x02                   /*!< configuration mode                     */
#define CO_LSS_EXIT            0x04                   /*!< exit LSS slave state machine           */

#define CO_LSS_STORED          0x10                   /*!< flag: configuration is stored          */

#define CO_LSS_CMD_WAIT           0
#define CO_LSS_CMD_CONF           1

#define CO_LSS_RES_SEL_OK        68
#define CO_LSS_RES_SLAVE         79
#define CO_LSS_RES_UNCONF        80

#define CO_LSS_SEL_VENDOR         0
#define CO_LSS_SEL_PRODUCT        1
#define CO_LSS_SEL_REVISION       2
#define CO_LSS_SEL_SERIAL         3
#define CO_LSS_SEL_ACTIVE         4

#define CO_LSS_REM_VENDOR        10
#define CO_LSS_REM_PRODUCT       11
#define CO_LSS_REM_REVISION_MIN  12
#define CO_LSS_REM_REVISION_MAX  13
#define CO_LSS_REM_SERIAL_MIN    14
#define CO_LSS_REM_SERIAL_MAX    15

/*
****************************************************************************************************
*                                       TYPE DEFINITIONS
****************************************************************************************************
*/

#if CO_LSS_EN > 0

struct CO_NODE_T;

/*------------------------------------------------------------------------------------------------*/
/*! \brief LSS SLAVE HANDLING
*
* \ingroup LSS
*
*          This structure contains all needed data to handle the CANOpen LSS slave.
*/
/*------------------------------------------------------------------------------------------------*/
typedef struct CO_LSS_T {
    struct CO_NODE_T *Node;                           /* link to parent CANopen node              */
    uint32_t        CfgBaudrate;                    /* buffered baudrate config for storage     */
    int16_t        Tmr;                            /* timer identifier for baudrate activation */
    uint8_t        CfgNodeId;                      /* buffered node ID config for storage      */
    uint8_t        Mode;                           /* mode of layer setting service slave      */
    uint8_t        Step;                           /* LSS address selection step               */
    uint8_t        Flags;                          /* event flags                              */

} CO_LSS;

typedef int16_t(*CO_LSS_SERVICE)(CO_LSS *, CO_IF_FRM *);

typedef struct CO_LSS_MAP_T {
    uint8_t        Request;                        /* received LSS command specifier           */
    uint8_t        Allowed;                        /* allowed service in bitmasked LSS modes   */
    CO_LSS_SERVICE    Service;                        /* service function                         */

} CO_LSS_MAP;
#endif

/*
****************************************************************************************************
*                                   INTERNAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_LSS_EN > 0
void       CO_LssInit     (CO_LSS *lss, struct CO_NODE_T *node);
int16_t CO_LssCheck    (CO_LSS *lss, CO_IF_FRM *frm);
#endif

/*
****************************************************************************************************
*                                   CALLBACK FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_LSS_EN > 0
int16_t CO_LssStore    (uint32_t  baudrate, uint8_t  nodeId);
int16_t CO_LssLoad     (uint32_t *baudrate, uint8_t *nodeId);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CO_LSS_H_ */
