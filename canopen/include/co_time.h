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

#ifndef CO_TIME_H_
#define CO_TIME_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

/*! \brief COB-ID time stamp object
*
*    These macros constructs the COB-ID for usage in object entry
*    index 1012: COB-ID time stamp object. The standard defines the
*    following encoding:
*    - bit31: consume: device consumes time stamp objects (1) or not (0)
*    - bit30: produce: device produces time stamp objects (1) or not (0)
*    - bit29: frame = 11bit standard CAN-ID (0) or 29bit extended CAN-ID (1)
*    - bit28-0: CAN-ID
*
* \param consume
*    the device consumes time stamp objects (1), or not (0)
*
* \param produce
*    the device produces time stamp objects (1); or not (0)
*
* \param id
*    the CAN-ID (standard or extended format)
* \{
*/
#define CO_COBID_TIME_STD(consume, produce, id)         \
    (uint32_t)(((uint32_t)(id) & 0x7ffuL)             | \
               (((uint32_t)(consume) & 0x1uL) << 31u) | \
               (((uint32_t)(produce) & 0x1uL) << 30u))

#define CO_COBID_TIME_EXT(consume, produce, id)         \
    (uint32_t)(((uint32_t)(id) & 0x1fffffffuL)        | \
               ((uint32_t)0x1uL << 29u)               | \
               (((uint32_t)(consume) & 0x1uL) << 31u) | \
               (((uint32_t)(produce) & 0x1uL) << 30u))
/*! \} */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_TIME_H_ */
