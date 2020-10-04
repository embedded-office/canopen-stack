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

#ifndef CO_IF_H_
#define CO_IF_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* PUBLIC CAN MACROS
******************************************************************************/

/*! \brief GET IDENTIFIER
*
*    This macro extracts the CAN identifier out of the CAN frame.
*
* \param f
*    The CAN frame
*/
#define CO_GET_ID(f)         \
    ( (uint32_t)(f)->Identifier )

/*! \brief SET IDENTIFIER
*
*    This macro sets the CAN identifier within the CAN frame.
*
* \param f
*    The CAN frame
*
* \param n
*    The CAN identifier
*/
#define CO_SET_ID(f,n)       \
    ( (f)->Identifier = (uint32_t)(n) )

/*! \brief GET DATA LENGTH CODE
*
*    This macro extracts the data length code (DLC) out of the CAN frame.
*
* \param f
*    The CAN frame
*/
#define CO_GET_DLC(f)        \
    ( (uint8_t)(f)->DLC )

/*! \brief SET DATA LENGTH CODE
*
*    This macro sets the data length code (DLC) within the CAN frame.
*
* \param f
*    The CAN frame
*
* \param n
*    The data length code (DLC)
*/
#define CO_SET_DLC(f,n)      \
    ((f)->DLC = (uint8_t)(n))

/*! \brief GET DATA BYTE
*
*    This macro extracts a data byte (8 bit) out of the CAN frame.
*
* \param f
*    The CAN frame
*
* \param p
*    The data position (0..7)
*/
#define CO_GET_BYTE(f,p)     \
    (uint8_t)( (uint8_t)(f)->Data[(p)&0x7] )

/*! \brief SET DATA BYTE
*
*    This macro sets a data byte (8 bit) within the CAN frame.
*
* \param f
*    The CAN frame
*
* \param n
*    The data value
*
* \param p
*    The data position (0..7)
*/
#define CO_SET_BYTE(f,n,p)   \
    { (f)->Data[(p)&0x7] = (uint8_t)(n); }

/*! \brief GET DATA WORD
*
*    This macro extracts a data word (16 bit) out of the CAN frame.
*
* \param f
*    The CAN frame
*
* \param p
*    The data position (0..6)
*/
#define CO_GET_WORD(f,p)     \
    (uint16_t)( ( ( (uint16_t)((f)->Data[((p)+1)&0x7]) ) << 8 ) | \
                ( ( (uint16_t)((f)->Data[((p)  )&0x7]) )      )   )

/*! \brief SET DATA WORD
*
*    This macro sets a data word (16 bit) within the CAN frame.
*
* \param f
*    The CAN frame
*
* \param n
*    The data value
*
* \param p
*    The data position (0..6)
*/
#define CO_SET_WORD(f,n,p)   \
    { (f)->Data[((p)  )&0x7] = (uint8_t)( ((uint16_t)(n) )     ); \
      (f)->Data[((p)+1)&0x7] = (uint8_t)( ((uint16_t)(n) ) >> 8); }

/*! \brief GET DATA LONG
*
*    This macro extracts a data long (32 bit) out of the CAN frame.
*
* \param f
*    The CAN frame
*
* \param p
*    The data position (0..4)
*/
#define CO_GET_LONG(f,p)     \
    (uint32_t)( ( ( (uint32_t)((f)->Data[((p)+3)&0x7]) ) << 24 ) | \
                ( ( (uint32_t)((f)->Data[((p)+2)&0x7]) ) << 16 ) | \
                ( ( (uint32_t)((f)->Data[((p)+1)&0x7]) ) <<  8 ) | \
                ( ( (uint32_t)((f)->Data[((p)  )&0x7]) )       )   )

/*! \brief SET DATA LONG
*
*    This macro sets a data long (32 bit) within the CAN frame.
*
* \param f
*    The CAN frame
*
* \param n
*    The data value
*
* \param p
*    The data position (0..4)
*/
#define CO_SET_LONG(f,n,p)   \
    { (f)->Data[((p)  )&0x7] = (uint8_t)(((uint32_t)(n))      ); \
      (f)->Data[((p)+1)&0x7] = (uint8_t)(((uint32_t)(n)) >>  8); \
      (f)->Data[((p)+2)&0x7] = (uint8_t)(((uint32_t)(n)) >> 16); \
      (f)->Data[((p)+3)&0x7] = (uint8_t)(((uint32_t)(n)) >> 24); }

/******************************************************************************
* PUBLIC CAN TYPES
******************************************************************************/

struct CO_NODE_T;

typedef struct CO_IF_FRM_T {         /*!< Type, which represents a CAN frame */
    uint32_t  Identifier;            /*!< CAN message identifier             */
    uint8_t   Data[8];               /*!< CAN message Data (payload)         */
    uint8_t   DLC;                   /*!< CAN message data length code (DLC) */
} CO_IF_FRM;

/*! CAN init function prototype */
typedef int16_t (*CO_IF_CAN_INIT_FUNC)(void);
/*! CAN enable function prototype */
typedef int16_t (*CO_IF_CAN_ENABLE_FUNC)(uint32_t);
/*! CAN read function prototype */
typedef int16_t (*CO_IF_CAN_READ_FUNC)(CO_IF_FRM *);
/*! CAN send function prototype */
typedef int16_t (*CO_IF_CAN_SEND_FUNC)(CO_IF_FRM *);
/*! CAN reset function prototype */
typedef int16_t (*CO_IF_CAN_RESET_FUNC)(void);
/*! CAN close function prototype */
typedef int16_t (*CO_IF_CAN_CLOSE_FUNC)(void);

typedef struct CO_IF_CAN_DRV_T {
    CO_IF_CAN_INIT_FUNC   Init;
    CO_IF_CAN_ENABLE_FUNC Enable;
    CO_IF_CAN_READ_FUNC   Read;
    CO_IF_CAN_SEND_FUNC   Send;
    CO_IF_CAN_RESET_FUNC  Reset;
    CO_IF_CAN_CLOSE_FUNC  Close;
} CO_IF_CAN_DRV;

/******************************************************************************
* PUBLIC INTERFACE TYPES
******************************************************************************/

typedef struct CO_IF_DRV_T {         /*!< Type, which links driver functions */
    const CO_IF_CAN_DRV *Can;        /*!< Link to CAN driver functions       */
} CO_IF_DRV;

typedef struct CO_IF_T {          /*!< Driver interface structure            */
    struct CO_NODE_T *Node;       /*!< Link to parent node                   */
    CO_IF_DRV         Drv;        /*!< Link to hardware driver functions     */
} CO_IF;

/******************************************************************************
* PUBLIC CAN INTERFACE FUNCTIONS
******************************************************************************/

/*! \brief  READ CAN FRAME
*
*    This function waits for a CAN frame on the interface without timeout.
*    If a CAN frame is received, the given frm will be filled with the
*    received data.
*
* \param cif
*    pointer to the interface structure
*
* \param frm
*    pointer to the receive frame buffer
*
* \retval  >0    the size of CO_IF_FRM on success
* \retval  <0    the CAN driver error code
*/
int16_t COIfCanRead(CO_IF *cif, CO_IF_FRM *frm);

/*! \brief  SEND CAN FRAME
*
*    This function sends the given CAN frame on the interface without delay.
*
* \param cif
*     pointer to the interface structure
*
* \param frm
*     pointer to the receive frame buffer
*
* \retval  >0    the size of CO_IF_FRM on success
* \retval  <0    the internal CanBus error code
*/
int16_t COIfCanSend(CO_IF *cif, CO_IF_FRM *frm);

/*! \brief  RESET CAN INTERFACE
*
*    This function resets the CAN interface and flushes all already
*    buffered CAN frames.
*
* \param cif
*    pointer to the interface structure
*/
void COIfCanReset(CO_IF *cif);

/*! \brief  CLOSE CAN INTERFACE
*
*    This function closes the CAN interface.
*
* \param cif
*    pointer to the interface structure
*/
void COIfCanClose(CO_IF *cif);

/*! \brief  INITIALIZE CAN INTERFACE
*
*    This function initializes the CAN bus interface.
*
* \param cif
*    pointer to the interface structure
*
* \param node
*    pointer to the parent node
*/
void COIfCanInit(CO_IF *cif, struct CO_NODE_T *node);

/*! \brief  ENABLE CAN INTERFACE
*
*    This function enables the CAN bus interface.
*
* \param cif
*    pointer to the interface structure
*
* \param baudrate
*    baudrate, if set to 0 the default baudrate is used
*/
void COIfCanEnable(CO_IF *cif, uint32_t baudrate);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief INTERFACE RECEIVE CALLBACK
*
*    This function is called for each CAN frame, which is not consumed
*    (processed) by the CANopen stack.
*
* \note
*    The CAN frame pointer is checked to be valid before calling this
*    function.
*
* \param frm
*    The received CAN frame
*/
extern void COIfReceive(CO_IF_FRM *frm);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* CO_IF_H_ */
