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
#define TEST_INIT   StubReset()

#include "acutest.h"

/******************************************************************************
* TEST CASES - STUB FUNCTIONS
******************************************************************************/

uint32_t StubLoad = 0;
CO_ERR CONodeParaLoad(struct CO_NODE_T *node, enum CO_NMT_RESET_T type)
{
    StubLoad++;
    return (CO_ERR_NONE);
}

uint32_t StubStore = 0;
CO_ERR COParaStore(struct CO_PARA_T *pg, struct CO_NODE_T *node)
{
    StubStore++;
    return (CO_ERR_NONE);
}

void StubReset (void)
{
    StubLoad = 0;
    StubStore = 0;
}

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_sub_0(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1010, 0, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 1);
}

void test_size_sub_1(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_enabled(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    uint32_t val  = 0x11223344;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};
    data.Value = CO_PARA___E;

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x00000001);
}

void test_read_autonomous(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    uint32_t val  = 0x11223344;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};
    data.Value = CO_PARA__A_;

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x00000002);
}

void test_read_bad_node(void)
{
    CO_PARA  data = { 0 };
    uint32_t val  = 0x33445566;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x33445566);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_single(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    uint32_t val = 0x65766173;      /* store signature is ascii: 'save' */
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x1010, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(1)},
        { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjWrValue(&Obj[1], &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubStore == 1);
}

void test_write_all(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  app = { 0 };
    CO_PARA  com = { 0 };
    uint32_t val = 0x65766173;      /* store signature is ascii: 'save' */
    CO_ERR   err;
    CO_OBJ   Obj[4] = {
        { CO_KEY(0x1010, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(3)},
        { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&app)}, /* dummy entry? not ideal */
        { CO_KEY(0x1010, 2, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&app)},
        { CO_KEY(0x1010, 3, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&com)},
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 4);

    err = COObjWrValue(&Obj[1], &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubStore == 2);
}

void test_write_bad_signature(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    uint32_t val = 0x12345678;
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x1010, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(1)},
        { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjWrValue(&Obj[1], &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_TYPE_WR);
    TEST_CHECK(StubStore == 0);
}

void test_write_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x2345, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(1)},
        { CO_KEY(0x2345, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjWrValue(&Obj[1], &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_TYPE_WR);
}

void test_write_bad_node(void)
{
    CO_PARA  data = { 0 };
    uint32_t val  = 0x7788;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 0, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_check(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x1010, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(1)},
        { CO_KEY(0x1010, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubLoad == 2);
}

void test_init_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x2345, 0, CO_OBJ_D___R_), CO_TPARA_STORE, (CO_DATA)(1)},
        { CO_KEY(0x2345, 1, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_com(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 0, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};

    err = COObjReset(&Obj, &AppNode, CO_RESET_COM);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubLoad == 1);
}

void test_reset_node(void)
{
    CO_NODE  AppNode = { 0 };
    CO_PARA  data = { 0 };
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1010, 0, CO_OBJ_____RW), CO_TPARA_STORE, (CO_DATA)(&data)};

    err = COObjReset(&Obj, &AppNode, CO_RESET_NODE);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubLoad == 1);
}


TEST_LIST = {
    { "size_sub_0",          test_size_sub_0          },
    { "size_sub_1",          test_size_sub_1          },
    { "size_bad_size",       test_size_bad_size       },
    { "read_enabled",        test_read_enabled        },
    { "read_autonomous",     test_read_autonomous     },
    { "read_bad_node",       test_read_bad_node       },
    { "write_single",        test_write_single        },
    { "write_all",           test_write_all           },
    { "write_bad_signature", test_write_bad_signature },
    { "write_bad_index",     test_write_bad_index     },
    { "write_bad_node",      test_write_bad_node      },
    { "init_check",          test_init_check          },
    { "init_bad_index",      test_init_bad_index      },
    { "reset_com",           test_reset_com           },
    { "reset_node",          test_reset_node          },
    { NULL, NULL }
};
