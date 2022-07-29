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

uint32_t StubAktivate = 0;
void COSyncProdActivate(CO_SYNC *sync)
{
    StubAktivate++;
}

void StubReset(void)
{
    StubAktivate = 0;
}

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_unknown(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_known(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 2);

    TEST_CHECK(size == 4);
}

void test_size_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_time(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x22334455);
}

void test_read_bad_node(void)
{
    uint32_t data = 0x11223344;
    uint32_t val  = 0x33445566;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x33445566);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_active(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    uint32_t val  = 0x11223344;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};
    AppNode.Sync.CobId = (1uL << 30);   /* generator active */

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x11223344);
    TEST_CHECK(StubAktivate == 1);
}

void test_write_inactive(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x33445566;
    uint32_t val  = 0x22334455;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};
    AppNode.Sync.CobId = (0uL << 30);   /* generator inactive */

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x22334455);
    TEST_CHECK(StubAktivate == 0);
}

void test_write_bad_node(void)
{
    uint32_t data = 0x55667788;
    uint32_t val  = 0x77889900;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(data == 0x55667788);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_check(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
}

void test_init_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x3456, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

void test_init_bad_sub(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 12, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE  AppNode = { 0 };
    uint32_t data = 0x22334455;
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1006, 0, CO_OBJ_____RW), CO_TSYNC_CYCLE, (CO_DATA)(&data)};

    err = COObjReset(&Obj, &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
}


TEST_LIST = {
    { "size_unknown",    test_size_unknown    },
    { "size_known",      test_size_known      },
    { "size_bad_size",   test_size_bad_size   },
    { "read_time",       test_read_time       },
    { "read_bad_node",   test_read_bad_node   },
    { "write_active",    test_write_active    },
    { "write_inactive",  test_write_inactive  },
    { "write_bad_node",  test_write_bad_node  },
    { "init_check",      test_init_check      },
    { "init_bad_index",  test_init_bad_index  },
    { "init_bad_sub",    test_init_bad_sub    },
    { "reset_unused",    test_reset_unused    },
    { NULL, NULL }
};
