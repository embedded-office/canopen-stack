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

void StubReset(void);
#define TEST_INIT StubReset()

#include "acutest.h"

/******************************************************************************
* TEST CASE - STUB FUNCTIONS
******************************************************************************/

uint32_t StubPdoTrigObj = 0;
void COTPdoTrigObj(CO_TPDO *tpdo, CO_OBJ *obj)
{
    StubPdoTrigObj++;
}

void StubReset(void)
{
    StubPdoTrigObj = 0;
}

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_unknown(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_known(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 4);

    TEST_CHECK(size == 4);
}

void test_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x1122;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 2);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_ref(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var;
    CO_ERR   err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(var == data);
}

void test_read_direct(void)
{
    CO_NODE  AppNode = { 0 };
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TUNSIGNED32, (CO_DATA)(0x22334455)};
    uint32_t var;
    CO_ERR   err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(var == 0x22334455);
}

void test_read_nodeid(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x33445566;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ__N__RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var;
    CO_ERR   err;

    AppNode.NodeId = 7;
    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(var == data + 7);
}

void test_read_bad_node(void)
{
    uint32_t var = 0x44556677;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TUNSIGNED32, (CO_DATA)(0x11223344)};
    CO_ERR   err;

    err = COObjRdValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(var == 0x44556677);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_ref(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var = 0x22334455;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x22334455);
}

void test_write_direct(void)
{
    CO_NODE  AppNode = { 0 };
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TUNSIGNED32, (CO_DATA)(0x22334455)};
    uint32_t var = 0x33445566;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK((uint32_t)Obj.Data == 0x33445566);
}

void test_write_nodeid(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x33445566;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ__N__RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var = 0x44556677 + 7;
    CO_ERR   err;

    AppNode.NodeId = 7;
    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x44556677);
}

void test_write_pdo_async(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x33445566;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ___APRW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var = 0x44556677;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x44556677);
    TEST_CHECK(StubPdoTrigObj == 1);
}

void test_write_pdo(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x33445566;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ____PRW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    uint32_t var = 0x44556677;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x44556677);
    TEST_CHECK(StubPdoTrigObj == 0);
}

void test_write_bad_node(void)
{
    uint32_t var = 0x44556677;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TUNSIGNED32, (CO_DATA)(0x11223344)};
    CO_ERR   err;

    err = COObjWrValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(var == 0x44556677);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_unused(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    CO_ERR   err;

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x11223344);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x11223344;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TUNSIGNED32, (CO_DATA)(&data)};
    CO_ERR   err;

    err = COObjReset(&Obj, &AppNode, 0);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x11223344);
}


TEST_LIST = {
    { "size_unknown",    test_size_unknown    },
    { "size_known",      test_size_known      },
    { "bad_size",        test_bad_size        },
    { "read_ref",        test_read_ref        },
    { "read_direct",     test_read_direct     },
    { "read_nodeid",     test_read_nodeid     },
    { "read_bad_node",   test_read_bad_node   },
    { "write_ref",       test_write_ref       },
    { "write_direct",    test_write_direct    },
    { "write_nodeid",    test_write_nodeid    },
    { "write_pdo_async", test_write_pdo_async },
    { "write_pdo",       test_write_pdo       },    
    { "write_bad_node",  test_write_bad_node  },
    { "init_unused",     test_init_unused     },
    { "reset_unused",    test_reset_unused    },
    { NULL, NULL }
};
