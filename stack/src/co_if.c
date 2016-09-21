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
* \file     co_if.c
*
* \brief    INTERFACE
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_if.c#4 $
*
*           This source file implements the abstracted CAN interface.
****************************************************************************************************
*/
/*----------------------------------------END OF HEADER-------------------------------------------*/

/*
****************************************************************************************************
*                                             INCLUDES
****************************************************************************************************
*/

#include "co_core.h"

/*
****************************************************************************************************
*                                             FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  READ CAN FRAME
*
* \ingroup  API
*
*           This function waits for a CAN frame on the interface without timeout. If a CAN frame
*           is received, the given frm will be filled with the received data.
*
* \param[in]       cif           pointer to the interface structure
*
* \param[in,out]   frm           pointer to the receive frame buffer
*
* \retval   >0     the size of CO_IF_FRM on success
* \retval   <0     the internal CanBus error code
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COIfRead (CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err = -1;                                 /* Local: function error code               */
                                                      /*------------------------------------------*/
    /* insert your code here */

    return (err);                                     /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  SEND CAN FRAME
*
* \ingroup  API
*
*           This function sends the given CAN frame on the interface without delay.
*
* \param[in]       cif           pointer to the interface structure
*
* \param[in]       frm           pointer to the receive frame buffer
*
* \retval   >0     the size of CO_IF_FRM on success
* \retval   <0     the internal CanBus error code
*/
/*------------------------------------------------------------------------------------------------*/
int16_t COIfSend  (CO_IF *cif, CO_IF_FRM *frm)
{
    int16_t err = -1;                                 /* Local: function error code               */
                                                      /*------------------------------------------*/
    /* insert your code here */

    if (err < 0) {                                    /* see, if an error is detected             */
        cif->Node->Error = CO_ERR_IF_SEND;            /* set bus write error                      */
    }
                                                      /*------------------------------------------*/
    return (err);                                     /* return function result                   */
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  RESET CAN INTERFACE
*
* \ingroup  API
*
*           This function resets the CAN interface and flushes all already buffered CAN frames.
*
* \param[in,out]   cif           pointer to the interface structure
*/
/*------------------------------------------------------------------------------------------------*/
void COIfReset(CO_IF *cif)
{
    int16_t err = -1;                                 /* Local: function error code               */
                                                      /*------------------------------------------*/
    /* insert your code here */
    
    if (err < 0) {                                    /* see, if an error is detected             */
        cif->Node->Error = CO_ERR_IF_RESET;           /* set error code                           */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  CLOSE CAN INTERFACE
*
* \ingroup  API
*
*           This function closes the CAN interface.
*
* \param[in,out]   cif           pointer to the interface structure
*/
/*------------------------------------------------------------------------------------------------*/
void COIfClose(CO_IF *cif)
{
    int16_t err = -1;                                 /* Local: function error code               */
                                                      /*------------------------------------------*/
    /* insert your code here */
    
    if (err < 0) {                                    /* see, if an error is detected             */
        cif->Node->Error = CO_ERR_IF_CLOSE;           /* set error code                           */
    }
}

/*
****************************************************************************************************
*                                         INTERNAL FUNCTIONS
****************************************************************************************************
*/

/*------------------------------------------------------------------------------------------------*/
/*! \brief  INITIALIZE CAN INTERFACE
*
* \ingroup  INTERNAL
*
*           This function initializes the CAN bus interface.
*
* \param[in,out]   cif           pointer to the interface structure
*
* \param[in,out]   node          pointer to the parent node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_IfInit (CO_IF *cif, struct CO_NODE_T *node)
{
    int16_t err = -1;                                 /* Local: function error code               */
                                                      /*------------------------------------------*/
    /* insert your code here */
    
    if (err < 0) {                                    /* see, if an error is detected             */
        node->Error = CO_ERR_IF_INIT;                 /* yes: set error code                      */
    }
}

/*------------------------------------------------------------------------------------------------*/
/*! \brief  ENABLE CAN INTERFACE
*
* \ingroup  INTERNAL
*
*           This function enables the CAN bus interface.
*
* \param[in,out]   cif           pointer to the interface structure
*
* \param[in,out]   node          pointer to the parent node
*/
/*------------------------------------------------------------------------------------------------*/
void CO_IfEnable (CO_IF *cif, uint32_t baudrate)
{
    int16_t err = -1;                                 /* Local: function error code               */
    CO_IF_DRV  drv;
                                                      /*------------------------------------------*/
    if (baudrate == 0) {                              /* see, if no baudrate is given             */
    	baudrate = cif->Node->Baudrate;               /* get default baudrate from node           */
    }

    /* insert your code here */

    if (err < 0) {                                    /* see, if an error is detected             */
        cif->Node->Error = CO_ERR_IF_ENABLE;          /* yes: set error code                      */
    } else {
    	cif->Node->Baudrate = baudrate;               /* store new baudrate in parent node        */
    }
}

/*
****************************************************************************************************
*                                         CALLBACK FUNCTIONS
****************************************************************************************************
*/

#if CO_CB_IF_RECEIVE_EN == 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief INTERFACE RECEIVE CALLBACK
*
* \ingroup CALLBACK
*
*          This function is called for each CAN frame, which is not consumed (processed) by the
*          CANopen stack.
*
* \note    This implementation is an example implementation, which will do nothing. This function
*          is optional and application specific. The function can be implemented somewhere in the
*          in the application code. The activation of the application callback function is done
*          with \ref CO_CB_IF_RECEIVE_EN.
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
void CO_IfReceive(CO_IF_FRM *frm)
{
    (void)frm;
}
#endif
