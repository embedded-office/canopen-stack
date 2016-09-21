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
* \file     co_fobj.c
*
* \brief    FULL OBJECT DIRECTORY ENTRY
*
*  $Id: //stream_uccanopen/_root/uccanopen/source/co_fobj.c#6 $
*
*           This source file implements the object entry types, which are needed for full
*           compliance with all data types.
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
*                                    LOCAL FUNCTION PROTOTYPES
****************************************************************************************************
*/

#if CO_OBJ_STRING_EN > 0
static uint32_t CO_TStringSize (CO_OBJ *obj, uint32_t width);
static int16_t CO_TStringCtrl (CO_OBJ *obj, uint16_t func, uint32_t para);
static int16_t CO_TStringRead (CO_OBJ *obj, void *buf, uint32_t len);
#endif

#if CO_OBJ_DOMAIN_EN > 0
static uint32_t CO_TDomainSize (CO_OBJ *obj, uint32_t width);
static int16_t CO_TDomainCtrl (CO_OBJ *obj, uint16_t func, uint32_t para);
static int16_t CO_TDomainRead (CO_OBJ *obj, void *buf, uint32_t len);
static int16_t CO_TDomainWrite(CO_OBJ *obj, void *buf, uint32_t len);
#endif

/*
****************************************************************************************************
*                                       GLOBAL OBJECT TYPES
****************************************************************************************************
*/

#if CO_OBJ_STRING_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE STRING
*
* \ingroup OBJ
*
*          This type is responsible for the access to unlimited string constants. It is assumed,
*          that the strings are declared in nonvolatile memory (e.g. FLASH) and the starting
*          address is stored in the object entry member 'Data'.
*
* \note    If the object entry is marked to be read-/write-able, the write access to the object
*          entry will change the start address of the string - not the string itself. If writing
*          the string itself is needed, another (user-)type must be defined. The write function
*          must be implemented with the needed memory handling. The other functions may be reused.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTString = { 0, 0, CO_TStringSize, CO_TStringCtrl, CO_TStringRead, 0 };
#endif

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief OBJECT TYPE DOMAIN
*
* \ingroup OBJ
*
*          This type is responsible for the access to domain memory areas. It is assumed,
*          that the memory is declared in random accessible memory (e.g. RAM, FLASH, etc..).
*
* \note    This exemplary implementation is usable for reading and writing in direct accessible
*          memory (e.g. RAM). The reading from FLASH memory is no problem, but for the write
*          access to FLASH memory, there should be a special DOMAIN implementation for this
*          media.
*/
/*------------------------------------------------------------------------------------------------*/
CO_OBJ_TYPE COTDomain = { 0, 0, CO_TDomainSize, CO_TDomainCtrl, CO_TDomainRead, CO_TDomainWrite };
#endif

/*
****************************************************************************************************
*                                         LOCAL FUNCTIONS
****************************************************************************************************
*/

#if CO_OBJ_STRING_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief STRING OBJECT SIZE
*
* \ingroup INTERNAL
*
*          This function is responsible to calculate and return the size of the string, which
*          starts at the object entry member 'Data'. The string length calculation stopps at
*          the end-of-string character ('\0') and returns the number of characters without the
*          final 0.
*
* \param[in]       obj          String object entry reference
*
* \param[in]       width        Requested string size (or 0 if unknown)
*
* \return  Number of character in the string, counting without the end of string mark.
*/
/*------------------------------------------------------------------------------------------------*/
static uint32_t CO_TStringSize(CO_OBJ *obj, uint32_t width)
{
    uint32_t  strlen = 0;                           /* LocaL: the resulting string length       */
    uint8_t *str;                                  /* Local: string character pointer          */
                                                      /*------------------------------------------*/
    (void)width;                                      /* unused, prevent compiler warning         */
    str = (uint8_t *)(obj->Data);                  /* set start of string to member 'Data'     */
    while (*str != '\0') {                            /* loop through string until EOS            */
        strlen++;                                     /* increment string length                  */
        str++;                                        /* step to next character in string         */
    }                                                 /*------------------------------------------*/
    return (strlen);                                  /* return string length                     */
}
#endif

#if CO_OBJ_STRING_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief STRING OBJECT ACCESS CONTROL
*
* \ingroup INTERNAL
*
*          This function is responsible to perform special control operations on the object entry.
*          This control function supports the function(s):
*
*          - CO_CTRL_SET_OFF: set the working offset to the given value in para for subsequent
*                             read function calls.
*
* \param[in,out]   obj          String object entry reference
*
* \param[in]       func         Control function code
*
* \param[in]       para         Control function parameter
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TStringCtrl(CO_OBJ *obj, uint16_t func, uint32_t para)
{
    int16_t result = CO_ERR_TYPE_CTRL;             /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (func == CO_CTRL_SET_OFF) {                    /* see, if function code is correct         */
        obj->Type->Offset = para;                     /* yes: set offset for further operations   */
        result = CO_ERR_NONE;                         /* indicate success to calling function     */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_STRING_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief STRING OBJECT READ ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible to copy the string characters from the object entry
*          into the given buffer memory.
*
* \param[in,out]   obj          String object entry reference
*
* \param[in,out]   buf          Pointer to buffer memory
*
* \param[in]       len          Length of buffer memory
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TStringRead(CO_OBJ *obj, void *buf, uint32_t len)
{
    uint32_t  offset;                               /* Local: working offset within string      */
    int16_t  result = CO_ERR_NONE;                 /* Local: function result                   */
    uint8_t *str;                                  /* Local: pointer to string                 */
    uint8_t *dst;                                  /* Local: pointer to destination            */
                                                      /*------------------------------------------*/
    offset = obj->Type->Offset;                       /* get current working offset               */
    str    = (uint8_t *)(obj->Data) + offset;      /* set string pointer to that location      */
    dst    = (uint8_t *)buf;                       /* set dest. pointer to start of buffer     */
    while ( (*str != 0) &&                            /* loop through string until end of string  */
            ( len  > 0) ) {                           /*   or buffer is full                      */
        *dst = *str;                                  /* copy string character to buffer          */
        offset++;                                     /* update working offset                    */
        str++;                                        /* step to next string character            */
        dst++;                                        /* step to next buffer location             */
        len--;                                        /* decrement remaining space in buffer      */
    }                                                 /*------------------------------------------*/
    obj->Type->Offset = offset;                       /* update working offset in type structure  */
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief DOMAIN OBJECT SIZE
*
* \ingroup INTERNAL
*
*          This function is responsible to return the size of the Domain.
*
* \param[in]       obj          Domain object entry reference
*
* \param[in]       width        Requested Domain size (or 0 if unknown)
*
* \return  Size in bytes of the domain.
*/
/*------------------------------------------------------------------------------------------------*/
static uint32_t CO_TDomainSize(CO_OBJ *obj, uint32_t width)
{
    CO_DOM     *dom;                                  /* Local: ptr to the domain info structure  */
    uint32_t  result = 0;                           /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (obj->Data == 0) {                             /* see, if parameter is correct             */
        return (result);                              /* if not, indicate error                   */
    }
    dom = (CO_DOM *)(obj->Data);                      /* get domain info structure                */
    if ((width > 0) && (width < dom->Size)) {         /* see, if part of a domain is requested?   */
        result = width;                               /* allow operations on a part of a domain   */
    } else {
        result = dom->Size;                           /* get domain size information              */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief DOMAIN OBJECT ACCESS CONTROL
*
* \ingroup INTERNAL
*
*          This function is responsible to perform special control operations on the object entry.
*          This control function supports the function(s):
*
*          - CO_CTRL_SET_OFF: set the working offset to the given value in para for subsequent
*                             read function calls.
*
* \param[in,out]   obj          Domain object entry reference
*
* \param[in]       func         Control function code
*
* \param[in]       para         Control function parameter
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TDomainCtrl(CO_OBJ *obj, uint16_t func, uint32_t para)
{
    int16_t result = CO_ERR_TYPE_CTRL;             /* Local: function result                   */
                                                      /*------------------------------------------*/
    if (func == CO_CTRL_SET_OFF) {                    /* see, if function code is correct         */
        obj->Type->Offset = para;                     /* yes: set offset for further operations   */
        result = CO_ERR_NONE;                         /* indicate success to calling function     */
    }                                                 /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief DOMAIN OBJECT READ ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible to copy the domain memory from the object entry
*          into the given buffer memory.
*
* \param[in,out]   obj          Domain object entry reference
*
* \param[in,out]   buf          Pointer to buffer memory
*
* \param[in]       len          Length of buffer memory
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TDomainRead(CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_DOM     *dom;                                  /* Local: ptr to the domain info structure  */
    int16_t  result = CO_ERR_NONE;                 /* Local: function result                   */
    uint8_t *src;                                  /* Local: pointer within RAM domain         */
    uint8_t *dst;                                  /* Local: pointer to buffer                 */
    uint32_t  num;                                  /* Local: remaining bytes in domain         */
                                                      /*------------------------------------------*/
    dom = (CO_DOM *)(obj->Data);                      /* get domain info structure                */
    src = (uint8_t *)(dom->Start +                 /* set pointer to working domain address    */
                         obj->Type->Offset);
    num = dom->Size - obj->Type->Offset;              /* calculate remaining bytes in domain      */
    dst = (uint8_t *)buf;                          /* get buffer pointer                       */
    while ((len > 0) && (num > 0)) {                  /* loop through remaining domain            */
        *dst = *src;                                  /* copy domain into buffer location         */
        src++;                                        /* switch to next domain byte               */
        dst++;                                        /* switch to next buffer location           */
        len--;                                        /* decrement remaining space in buffer      */
        obj->Type->Offset++;                          /* update domain working offset             */
        num--;                                        /* update remaining bytes in domain         */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif

#if CO_OBJ_DOMAIN_EN > 0
/*------------------------------------------------------------------------------------------------*/
/*! \brief DOMAIN OBJECT WRITE ACCESS
*
* \ingroup INTERNAL
*
*          This function is responsible to copy the given buffer memory into the object entry
*          domain memory.
*
* \param[in,out]   obj          Domain object entry reference
*
* \param[in]       buf          Pointer to buffer memory
*
* \param[in]       len          Length of buffer memory
*
* \retval         =CO_ERR_NONE  Successfully operation
* \retval        !=CO_ERR_NONE  An error is detected
*/
/*------------------------------------------------------------------------------------------------*/
static int16_t CO_TDomainWrite(CO_OBJ *obj, void *buf, uint32_t len)
{
    CO_DOM     *dom;                                  /* Local: ptr to the domain info structure  */
    int16_t  result = CO_ERR_NONE;                 /* Local: function result                   */
    uint8_t *src;                                  /* Local: pointer within RAM domain         */
    uint8_t *dst;                                  /* Local: pointer to buffer                 */
    uint32_t  num;                                  /* Local: remaining bytes in domain         */
                                                      /*------------------------------------------*/
    dom = (CO_DOM *)(obj->Data);                      /* get domain info structure                */
    dst = (uint8_t *)(dom->Start +                 /* set pointer to working domain address    */
                         obj->Type->Offset);
    num = dom->Size - obj->Type->Offset;              /* calculate remaining bytes in domain      */
    src = (uint8_t *)buf;                          /* get buffer pointer                       */
    while ((len > 0) && (num > 0)) {                  /* loop through remaining domain            */
        *dst = *src;                                  /* copy buffer location into domain         */
        src++;                                        /* switch to next domain byte               */
        dst++;                                        /* switch to next buffer location           */
        len--;                                        /* decrement remaining space in buffer      */
        obj->Type->Offset++;                          /* update domain working offset             */
        num--;                                        /* update remaining bytes in domain         */
    }
                                                      /*------------------------------------------*/
    return (result);                                  /* return function result                   */
}
#endif
