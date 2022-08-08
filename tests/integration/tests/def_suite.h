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

#ifndef DEF_SUITE_H_
#define DEF_SUITE_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_env.h"
#include "app_env.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

typedef enum DEF_TEST_GROUPS_E {                      /*---- Test Groups -------------------------*/
    DEF_G_CORE,                                       /*!< Group: Core Components                 */

    DEF_G_SDOS,                                       /*!< Group: SDO Server                      */
    DEF_G_PDO,                                        /*!< Group: PDO Communication               */
    DEF_G_NMT,                                        /*!< Group: NMT Management                  */
    DEF_G_EMCY,                                       /*!< Group: EMCY Management                 */
    DEF_G_OD,                                         /*!< Group: Object Dictionary               */
    DEF_G_SYNC,                                       /*!< Group: SYNC Communication              */
    DEF_G_CSDO,                                       /*!< Group: SDO Client                      */

    DEF_G_NUM                                         /*!< Number of Groups                       */
} DEF_TEST_GROUPS;

typedef enum DEF_OD_SUITES_E {                        /*---- Object Dictionary Test Suites -------*/
    DEF_S_OD_API,                                     /*!< Group: Object read/write API           */

    DEF_S_OD_NUM                                      /*!< Number of Suites in Group              */
} DEF_OD_SUITES;

typedef enum DEF_CORE_SUITES_E {                      /*---- Core Component Test Suites ----------*/
    DEF_S_CORE_TMR,                                   /*!< Suite: Highspeed Timer                 */
    DEF_S_MIN_TIME,                                   /*!< Suite: COTmrGetMinTime()               */

    DEF_S_CORE_NUM                                    /*!< Number of Suites in Group              */
} DEF_CORE_SUITES;

typedef enum DEF_SDOS_SUITES_E {                      /*---- SDO Server Test Suites --------------*/
    DEF_S_EXP_UP,                                     /*!< Suite: SDO Expedited Upload            */
    DEF_S_EXP_DOWN,                                   /*!< Suite: SDO Expedited Download          */
    DEF_S_SEG_UP,                                     /*!< Suite: SDO Segmented Upload            */
    DEF_S_SEG_DOWN,                                   /*!< Suite: SDO Segmented Download          */
    DEF_S_BLK_UP,                                     /*!< Suite: SDO Block Upload                */
    DEF_S_BLK_DOWN,                                   /*!< Suite: SDO Block Download              */

    DEF_S_SDOS_NUM                                    /*!< Number of Suites in Group              */
} DEF_SDOS_SUITES;

typedef enum DEF_PDO_SUITES_E {                       /*---- PDO Communication Test Suites -------*/
    DEF_S_PDO_TX,                                     /*!< Suite: PDO Transmit                    */
    DEF_S_PDO_RX,                                     /*!< Suite: PDO Receive                     */
    DEF_S_PDO_DYN,                                    /*!< Suite: Dynamic PDO Configuration       */

    DEF_S_PDO_NUM                                     /*!< Number of Suites in Group              */
} DEF_PDO_SUITES;

typedef enum DEF_NMT_SUITES_E {                       /*---- NMT Management Test Suites ----------*/
    DEF_S_NMT_MGR,                                    /*!< Suite: NMT Management                  */
    DEF_S_NMT_HBP,                                    /*!< Suite: NMT Heartbeat Producer          */
    DEF_S_NMT_HBC,                                    /*!< Suite: NMT Heartbeat Consumer          */
    DEF_S_NMT_LSS,                                    /*!< Suite: NMT Layer Setting Service       */

    DEF_S_NMT_NUM                                     /*!< Number of Suites in Group              */
} DEF_NMT_SUITES;

typedef enum DEF_EMCY_SUITES_E {                      /*---- EMCY Management Test Suites ---------*/
    DEF_S_EMCY_STATE,                                 /*!< Suite: EMCY Error States               */
    DEF_S_EMCY_ERR,                                   /*!< Suite: EMCY Error Register Object      */
    DEF_S_EMCY_HIST,                                  /*!< Suite: EMCY Error History Object       */
    DEF_S_EMCY_API,                                   /*!< Suite: EMCY Application Interface Test */

    DEF_S_EMCY_NUM                                    /*!< Number of Suites in Group              */
} DEF_EMCY_SUITES;

typedef enum DEF_SYNC_SUITES_E {                      /*---- SYNC Communication Test Suites ------*/
    DEF_S_SYNC_PROD,                                  /*!< Suite: SYNC Producer                   */

    DEF_S_SYNC_NUM                                    /*!< Number of Suites in Group              */
} DEF_SYNC_SUITES;

typedef enum DEF_CSDO_SUITES_E {                      /*---- SDO Client Test Suites --------------*/
    DEF_S_CSDO_EXP_UP,                                /*!< Suite: SDO Upload Expedited            */
    DEF_S_CSDO_EXP_DOWN,                              /*!< Suite: SDO Download Expedited          */
    DEF_S_CSDO_SEG_UP,                                /*!< Suite: SDO Upload Segmented            */
    DEF_S_CSDO_SEG_DOWN,                              /*!< Suite: SDO Download Segmented          */

    DEF_S_CSDO_NUM                                    /*!< Number of Suites in Group              */
} DEF_CSDO_SUITES;

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define SUITE_CORE_TMR()   TS_DEF_SUITE(DEF_G_CORE, DEF_S_CORE_TMR)  /*!< \addtogroup core_tmr    Core Timer Test     */

#define SUITE_OD_API()     TS_DEF_SUITE(DEF_G_OD, DEF_S_OD_API)      /*!< \addtogroup od_api  Object Dictionary API Test */

#define SUITE_EXP_UP()     TS_DEF_SUITE(DEF_G_SDOS, DEF_S_EXP_UP)    /*!< \addtogroup sdos_exp_up   SDO Server Test: Expedited Upload   */
#define SUITE_EXP_DOWN()   TS_DEF_SUITE(DEF_G_SDOS, DEF_S_EXP_DOWN)  /*!< \addtogroup sdos_exp_down SDO Server Test: Expedited Download */
#define SUITE_SEG_UP()     TS_DEF_SUITE(DEF_G_SDOS, DEF_S_SEG_UP)    /*!< \addtogroup sdos_seg_up   SDO Server Test: Segmented Upload   */
#define SUITE_SEG_DOWN()   TS_DEF_SUITE(DEF_G_SDOS, DEF_S_SEG_DOWN)  /*!< \addtogroup sdos_seg_down SDO Server Test: Segmented Download */
#define SUITE_BLK_UP()     TS_DEF_SUITE(DEF_G_SDOS, DEF_S_BLK_UP)    /*!< \addtogroup sdos_blk_up   SDO Server Test: Block Upload       */
#define SUITE_BLK_DOWN()   TS_DEF_SUITE(DEF_G_SDOS, DEF_S_BLK_DOWN)  /*!< \addtogroup sdos_blk_down SDO Server Test: Block Download     */

#define SUITE_PDO_TX()     TS_DEF_SUITE(DEF_G_PDO, DEF_S_PDO_TX)     /*!< \addtogroup pdo_tx  PDO Communication Test: PDO Transmit */
#define SUITE_PDO_RX()     TS_DEF_SUITE(DEF_G_PDO, DEF_S_PDO_RX)     /*!< \addtogroup pdo_rx  PDO Communication Test: PDO Receive  */
#define SUITE_PDO_DYN()    TS_DEF_SUITE(DEF_G_PDO, DEF_S_PDO_DYN)    /*!< \addtogroup pdo_dyn Dynamic PDO Configuration Test       */

#define SUITE_NMT_MGR()    TS_DEF_SUITE(DEF_G_NMT, DEF_S_NMT_MGR)    /*!< \addtogroup nmt_mgr NMT Management            */
#define SUITE_NMT_HBP()    TS_DEF_SUITE(DEF_G_NMT, DEF_S_NMT_HBP)    /*!< \addtogroup nmt_hbp NMT Heartbeat Producer    */
#define SUITE_NMT_HBC()    TS_DEF_SUITE(DEF_G_NMT, DEF_S_NMT_HBC)    /*!< \addtogroup nmt_hbc NMT Heartbeat Consumer    */
#define SUITE_NMT_LSS()    TS_DEF_SUITE(DEF_G_NMT, DEF_S_NMT_LSS)    /*!< \addtogroup nmt_lss NMT Layer Setting Service */

#define SUITE_EMCY_STATE() TS_DEF_SUITE(DEF_G_EMCY, DEF_S_EMCY_STATE) /*!< \addtogroup emcy_state EMCY Error State Test           */
#define SUITE_EMCY_ERR()   TS_DEF_SUITE(DEF_G_EMCY, DEF_S_EMCY_ERR)   /*!< \addtogroup emcy_err   EMCY Error Register Test        */
#define SUITE_EMCY_HIST()  TS_DEF_SUITE(DEF_G_EMCY, DEF_S_EMCY_HIST)  /*!< \addtogroup emcy_hist  EMCY Error History Test         */
#define SUITE_EMCY_API()   TS_DEF_SUITE(DEF_G_EMCY, DEF_S_EMCY_API)   /*!< \addtogroup emcy_api   EMCY Application Interface Test */

#define SUITE_SYNC_PROD()  TS_DEF_SUITE(DEF_G_SYNC, DEF_S_SYNC_PROD)  /*!< \addtogroup sync_prod  SYNC Producer Test */

#define SUITE_CSDO_EXP_UP()   TS_DEF_SUITE(DEF_G_CSDO, DEF_S_CSDO_EXP_UP)    /*!< \addtogroup csdo_exp_up    SDO Client Expedited Upload Test   */
#define SUITE_CSDO_EXP_DOWN() TS_DEF_SUITE(DEF_G_CSDO, DEF_S_CSDO_EXP_DOWN)  /*!< \addtogroup csdo_exp_down  SDO Client Expedited Download Test */
#define SUITE_CSDO_SEG_UP()   TS_DEF_SUITE(DEF_G_CSDO, DEF_S_CSDO_SEG_UP)    /*!< \addtogroup csdo_seg_down  SDO Client Segmented Upload Test   */
#define SUITE_CSDO_SEG_DOWN() TS_DEF_SUITE(DEF_G_CSDO, DEF_S_CSDO_SEG_DOWN)  /*!< \addtogroup csdo_seg_down  SDO Client Segmented Download Test */

#endif /* DEF_SUITE_H_ */
