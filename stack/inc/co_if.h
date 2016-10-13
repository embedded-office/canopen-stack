/******************************************************************************
* (c) by Embedded Office GmbH & Co. KG, <http://www.embedded-office.com/>
*------------------------------------------------------------------------------
* This file is part of CANopenStack, an open source CANopen Stack.
* Project home page is <https://github.com/MichaelHillmann/CANopenStack.git>.
* For more information on CANopen see <http://www.can-cia.org/>.
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

#ifndef CO_IF_H_
#define CO_IF_H_

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"

/******************************************************************************
* PUBLIC MACROS
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

/*! \brief CAN DRIVER IDENTIFIER
*
*    The driver specific bus identifier type.
*/
#define CO_IF_DRV    int

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;

typedef struct CO_IF_FRM_T {         /*!< Type, which represents a CAN frame */
    uint32_t  Identifier;            /*!< CAN message identifier             */
    uint8_t   Data[8];               /*!< CAN message Data (payload)         */
    uint8_t   DLC;                   /*!< CAN message data length code (DLC) */

} CO_IF_FRM;

typedef struct CO_IF_T {          /*!< Driver interface structure            */
    struct CO_NODE_T *Node;       /*!< Link to parent node                   */
    CO_IF_DRV         Drv;        /*!< Specific bus identifier of CAN driver */

} CO_IF;

/******************************************************************************
* PUBLIC FUNCTIONS
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
* \retval  <0    the internal CanBus error code
*/
int16_t COIfRead(CO_IF *cif, CO_IF_FRM *frm);

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
int16_t COIfSend(CO_IF *cif, CO_IF_FRM *frm);

/*! \brief  RESET CAN INTERFACE
*
*    This function resets the CAN interface and flushes all already
*    buffered CAN frames.
*
* \param cif
*    pointer to the interface structure
*/
void COIfReset(CO_IF *cif);

/*! \brief  CLOSE CAN INTERFACE
*
*    This function closes the CAN interface.
*
* \param cif
*    pointer to the interface structure
*/
void COIfClose(CO_IF *cif);

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

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
void COIfInit(CO_IF *cif, struct CO_NODE_T *node);

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
void COIfEnable(CO_IF *cif, uint32_t baudrate);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief INTERFACE RECEIVE CALLBACK
*
*    This function is called for each CAN frame, which is not consumed
*    (processed) by the CANopen stack.
*
* \note
*    This implementation is an example implementation, which will do
*    nothing. This function is optional and application specific. The
*    function can be implemented somewhere in the in the application
*    code.
*
* \note
*    The CAN frame pointer is checked to be valid before calling this
*    function.
*
* \param frm
*    The received CAN frame
*/
void COIfReceive(CO_IF_FRM *frm);

#endif /* CO_IF_H_ */
