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

#include "co_core.h"
#include "acutest.h"

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_unknown(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_known(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_ref(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x22334455);
}

void test_read_direct(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TPDO_MAP, (CO_DATA)(0x6677)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x6677);
}

void test_read_bad_node(void)
{
    uint32_t data = 0x33445566;
    uint32_t val  = 0x44556677;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x44556677);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_mapping(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data  = 0x88776655;
    uint8_t  data8 = 0x22;
    uint32_t val   = CO_LINK(0x2345,67,8);
    CO_ERR   err;
    CO_OBJ   Obj[3] = {
        { CO_KEY(0x1800, 1, CO_OBJ_D___RW), CO_TPDO_ID, (CO_DATA)((1ul<<31)|200uL)}, /* inactive 200h */
        { CO_KEY(0x1A00, 1, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)},
        { CO_KEY(0x2345,67, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8)}
    };
    (void)CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 3);

    err = COObjWrValue(&Obj[1], &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
}

void test_write_bad_node(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data  = 0x88776655;
    uint8_t  data8 = 0x22;
    uint32_t val   = CO_LINK(0x2345,67,8);
    CO_ERR   err;
    CO_OBJ   Obj[3] = {
        { CO_KEY(0x1800, 1, CO_OBJ_D___RW), CO_TPDO_ID, (CO_DATA)((1ul<<31)|200uL)}, /* inactive 200h */
        { CO_KEY(0x1A00, 1, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)},
        { CO_KEY(0x2345,67, CO_OBJ____PRW), CO_TUNSIGNED8, (CO_DATA)(&data8)}
    };
    (void)CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 3);

    err = COObjWrValue(&Obj[1], NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_check(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1A00, 1, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
}

void test_init_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x2345, 1, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1A00, 1, CO_OBJ_____RW), CO_TPDO_MAP, (CO_DATA)(&data)};

    err = COObjReset(&Obj, &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
}


TEST_LIST = {
    { "size_unknown",     test_size_unknown     },
    { "size_known",       test_size_known       },
    { "size_bad_size",    test_size_bad_size    },
    { "read_ref",         test_read_ref         },
    { "read_direct",      test_read_direct      },
    { "read_bad_node",    test_read_bad_node    },
    { "write_mapping",    test_write_mapping    },
    { "write_bad_node",   test_write_bad_node   },
    { "init_check",       test_init_check       },
    { "init_bad_index",   test_init_bad_index   },
    { "reset_unused",     test_reset_unused     },
    { NULL, NULL }
};
