/******************************************************************************
   Copyright 2022 Generac Holdings Inc.

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
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 8);
}

void test_size_known(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 8);

    TEST_CHECK(size == 8);
}

void test_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 2);

    TEST_CHECK(size == 8);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_ref(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var;
    CO_ERR   err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(var == data);
}

void test_read_direct(void)
{
    CO_NODE  AppNode = { 0 };
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TSIGNED64, (CO_DATA)(-22334455)};
    int64_t  var;
    CO_ERR   err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_OBJ_READ);
}

void test_read_nodeid(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -33445566LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ__N__RW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var;
    CO_ERR   err;

    AppNode.NodeId = 7;
    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(var == data + 7);
}

void test_read_bad_node(void)
{
    int64_t  var = -44556677LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TSIGNED64, (CO_DATA)(-11223344)};
    CO_ERR   err;

    err = COObjRdValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(var == -44556677LL);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_ref(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var = -1122334455667788LL;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -1122334455667788LL);
}

void test_write_direct(void)
{
    CO_NODE  AppNode = { 0 };
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TSIGNED64, (CO_DATA)(-22334455)};
    int64_t  var = -33445566LL;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_OBJ_WRITE);
}

void test_write_nodeid(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -33445566778899LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ__N__RW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var = -445566778899LL + 7LL;
    CO_ERR   err;

    AppNode.NodeId = 7;
    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -445566778899LL);
}

void test_write_pdo_async(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -33445566778899LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ___APRW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var = -445566778899LL;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -445566778899LL);
    TEST_CHECK(StubPdoTrigObj == 1);
}

void test_write_pdo(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -33445566778899LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ____PRW), CO_TSIGNED64, (CO_DATA)(&data)};
    int64_t  var = -445566778899LL;
    CO_ERR   err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -445566778899LL);
    TEST_CHECK(StubPdoTrigObj == 0);
}

void test_write_bad_node(void)
{
    int64_t  var = -445566778899LL;
    CO_OBJ   Obj = { CO_KEY(0, 0, CO_OBJ_D___RW), CO_TSIGNED64, (CO_DATA)(-11223344)};
    CO_ERR   err;

    err = COObjWrValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(var == -445566778899LL);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_unused(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    CO_ERR   err;

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -1122334455667788LL);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE  AppNode = { 0 };
    int64_t  data = -1122334455667788LL;
    CO_OBJ   Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TSIGNED64, (CO_DATA)(&data)};
    CO_ERR   err;

    err = COObjReset(&Obj, &AppNode, 0);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == -1122334455667788LL);
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
