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

#include "ts_env.h"
#include "ts_version.h"
#include "def_suite.h"
#include "co_ver.h"

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TST-0100
*
* \details collect and report the environment identification information
*/
/*---------------------------------------------------------------------------*/
static void TS_IdentEnv(void);
static void TS_IdentEnv(void)
{
    TS_Printf("-----------------------\n");
    TS_Printf(" E N V I R O N M E N T \n");
    TS_Printf("-----------------------\n");
    
    /*--- Framework ---*/
    TS_Printf("%s V", TS_NAME);
    TS_PrintVersion(TS_VERSION, TS_VER_FORMAT, TS_VER_BASE);
    TS_Printf("\n");

    /*--- Compiler ---*/
    TS_Printf("%s V", TS_ENV_NAME);
    TS_PrintVersion(TS_ENV_VER, TS_ENV_VER_FORMAT, TS_ENV_VER_BASE);
    TS_Printf("\n");

    /*--- Unit Under Test ---*/
    TS_Printf("CANopen Stack V%d.%d.%d",
               CO_VER_MAJOR, CO_VER_MINOR, CO_VER_BUILD);
    TS_Printf("\n");
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*! \brief REQ-TST-0110
*
* \details main entry function for the test application
*
* \return  number of failed tests
*/
/*---------------------------------------------------------------------------*/
int main(void)
{
    int result;

    TS_Init();
    TS_SetupAll(TS_IdentEnv, NULL);
 
    result = (int)TS_Start();

    return(result);
}
