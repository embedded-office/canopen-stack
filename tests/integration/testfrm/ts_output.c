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

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_output.h"
#include <stdio.h>

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function is the implementation for routing the output stream to the operating
*          system stdio terminal.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PutChar(void *arg, char character)
{
#if defined ( _MSC_VER )
    (void)arg;
    putchar((int)character);
#else
/*
* \note  The testsuite is running with MSVC compiler on the windows host, only. You may
*        adjust the settings ts_types.h and provide an output channel here to get the tests
*        running on your target, too.
*/
    (void)arg;
    (void)character;
#endif
}
