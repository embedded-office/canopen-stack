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

#ifndef CO_CFG_H_
#define CO_CFG_H_

/*! \brief DEFAULT SDO SERVER
*
*    This configuration define specifies how many SDO servers the library
*    will support.
*/
#ifndef CO_SSDO_N
#define CO_SSDO_N               1
#endif

/*! \brief DEFAULT SDO CLIENT
*
*    This configuration define specifies how many SDO clients the library
*    will support.
*/
#ifndef CO_CSDO_N
#define CO_CSDO_N               1
#endif

/*! \brief DEFAULT EMERGENCY CODES
*
*    This configuration define specifies how many emergency codes the library
*    will support.
*/
#ifndef CO_EMCY_N
#define CO_EMCY_N              32
#endif

/*! \brief DEFAULT RECEIVE PDO
*
*    This configuration define specifies how many receive PDOs the library
*    will support.
*/
#ifndef CO_RPDO_N
#define CO_RPDO_N               4
#endif

/*! \brief DEFAULT TRANSMIT PDO
*
*    This configuration define specifies how many transmit PDOs the library
*    will support.
*/
#ifndef CO_TPDO_N
#define CO_TPDO_N               4
#endif

/*! \brief DEFAULT ENABLE LSS
*
*    This configuration define specifies whether the LSS functionality will
*    be supported by the library.
*/
#ifndef USE_LSS
#define USE_LSS                 1
#endif

/*! \brief DEFAULT ENABLE SDO CLIENT
*
*    This configuration define specifies whether SDO client will be supported
*    by the library.
*/
#ifndef USE_CSDO
#define USE_CSDO                1
#endif

#endif  /* #ifndef CO_CFG_H_ */
