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

#include "def_suite.h"

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the error register contains the correct value
*          after registration of error #1
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_ErrRegOnSet)
{
    CO_NODE    node;
    uint8_t err_reg;
    
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1001,0),&err_reg);

    TS_ASSERT(3 == err_reg);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the error register contains the correct value
*          after registration of error #1 and #2
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_ErrRegOnSetRepeat)
{
    CO_NODE node;
    uint8_t err_reg;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2                        */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1001,0),&err_reg);

    TS_ASSERT(7 == err_reg);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the error register contains the correct value
*          after registration of error #1 and #2 and clearing errors #1.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_ErrRegOnClr)
{
    CO_NODE        node;
    uint8_t err_reg;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2                        */

    COEmcyClr(&node.Emcy, 1);                         /* clear error #1                           */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1001,0),&err_reg);

    TS_ASSERT(5 == err_reg);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/*------------------------------------------------------------------------------------------------*/
/*
*          This test will check that the error register contains the correct value
*          after resetting all errors.
*/
/*------------------------------------------------------------------------------------------------*/
TS_DEF_MAIN(TS_Emcy_ErrRegOnReset)
{
    CO_NODE        node;
    uint8_t err_reg;
                                                      /*------------------------------------------*/
    TS_CreateMandatoryDir();
    TS_CreateEmcy();
    TS_CreateNode(&node,0);

    COEmcySet(&node.Emcy, 1, 0);                      /* register error #1                        */
    COEmcySet(&node.Emcy, 2, 0);                      /* register error #2                        */

    COEmcyReset(&node.Emcy, 0);                       /* reset error #1                           */

    (void)CODictRdByte(&node.Dict,CO_DEV(0x1001,0),&err_reg);
    TS_ASSERT(0 == err_reg);

    CHK_NO_ERR(&node);                                /* check error free stack execution         */
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

SUITE_EMCY_ERR()
{
    TS_Begin(__FILE__);

//    CanDiagnosticOn(0);

    TS_RUNNER(TS_Emcy_ErrRegOnSet);
    TS_RUNNER(TS_Emcy_ErrRegOnSetRepeat);
    TS_RUNNER(TS_Emcy_ErrRegOnClr);
    TS_RUNNER(TS_Emcy_ErrRegOnReset);

//    CanDiagnosticOff(0);

    TS_End();
}
