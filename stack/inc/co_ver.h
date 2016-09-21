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
* \file     co_ver.h
*
* \brief    CANOPEN IDENTIFICATION DEFINITION
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_ver.h#2 $
*
*           This include file defines the symbolic constants and data types for the CANopen
*           version identifivation.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

#ifndef CO_VER_H_
#define CO_VER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "cpu.h"

/*
****************************************************************************************************
*                                             DEFINES
****************************************************************************************************
*/

#define CO_VER_MAJOR          3
#define CO_VER_MINOR          3
#define CO_VER_BUILD          0

/*
****************************************************************************************************
*                                        FUNCTION PROTOTYPE
****************************************************************************************************
*/

CPU_INT08U COVerMajor   (void);
CPU_INT08U COVerMinor   (void);
CPU_INT08U COVerBuild   (void);
CPU_INT32U COVerChange  (void);
CPU_INT32U COVersion    (void);

#ifdef __cplusplus
}
#endif

#endif /* ifndef CO_VER_H_ */
