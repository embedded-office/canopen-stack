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
/* This file is based on the project 'tinyprintf'. Find the original source code at
*  github: "https://github.com/cjlano/tinyprintf"
*/

#ifndef TS_PRINTF_H_
#define TS_PRINTF_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include <stdarg.h>
#include "ts_types.h"
#include "ts_lock.h"

/*!
* \addtogroup ts_frm_print   Test Framework - Formated Printer
* \details  This is formated printing service, similar to the standard printf() function. It is
*           reduced in features to keep it simple and reduce the memory consumption to a minimum.
*           The parser supports the following elements in the format string with zero padding and
*           field width:
*
*           + '%c' : single character
*           + '%d' : decimal integer
*           + '%i' : integer
*           + '%o' : octal number
*           + '%u' : unsigned integer
*           + '%s' : strings
*           + '%x' : hexadecimal number (lower case digits)
*           + '%X' : hexadecimal number (upper case digits)
*           + '%%' : the character '%'
* @{
*/

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This type specifies the callback function interface for the output stream.
*
* \param   argument
*          This argument is a optional pointer for the callback function. It may be set during
*          the initialization and will be passed unchanged to the callback function.
*
* \param   character
*          The character for printing to the output
*/
/*------------------------------------------------------------------------------------------------*/
typedef void(*TS_OUT_FUNC)(void *argument, char character);

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function initializes the callback interface from the printf parser to the output
*          stream function and returns the previous callback function.
*
* \param   arg
*          The argument is passed unchanged to the callback function for each function call.
*          Mostly unused and may be set to NULL.
*
* \param   function
*          This function pointer points to the output callback function
*/
/*------------------------------------------------------------------------------------------------*/
TS_OUT_FUNC TS_PrintfInit (void *arg, TS_OUT_FUNC function);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function parses the given format string and places the variable arguments in the
*          place holder. The parser supports the following elements in the format string with
*          zero padding and field width:
*          - '%c' : single character
*          - '%d' : decimal integer
*          - '%i' : integer
*          - '%o' : octal number
*          - '%u' : unsigned integer
*          - '%s' : strings
*          - '%x' : hexadecimal number
*          - '%X' : hexadecimal number
*          - '%%' : the character '%'
*
* \param   fmt
*          Format string
*/
/*------------------------------------------------------------------------------------------------*/
void TS_Printf (char *fmt, ...);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif
/*! @} */
