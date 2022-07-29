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
#define TEST_INIT  StubReset()

#include "acutest.h"

/******************************************************************************
* TEST CASES - STUB FUNCTIONS
******************************************************************************/

uint32_t StubTmrDelete = 0;
int16_t COTmrDelete(CO_TMR *tmr, int16_t actId)
{
    StubTmrDelete++;
    return (0);
}

uint32_t StubTmrCreate = 0;
int16_t COTmrCreate(CO_TMR *tmr, uint32_t startTicks, uint32_t cycleTicks, CO_TMR_FUNC func, void *para)
{
    StubTmrCreate++;
    return (0);
}

void StubReset (void)
{
    StubTmrDelete = 0;
    StubTmrCreate = 0;
}

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_unknown(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 2);
}

void test_size_known(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 2);

    TEST_CHECK(size == 2);
}

void test_size_bad_size(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = { 0 };
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 2);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_time(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x2233;
    uint16_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x2233);
}

void test_read_bad_node(void)
{
    uint16_t data = 0x1122;
    uint32_t val  = 0x3344;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x3344);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_start(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x2233;
    uint16_t val  = 0x4455;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    AppNode.Nmt.Tmr = -1;

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x4455);
    TEST_CHECK(StubTmrDelete == 0);
    TEST_CHECK(StubTmrCreate == 1);
}

void test_write_restart(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x3344;
    uint16_t val  = 0x5566;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    AppNode.Nmt.Tmr = 11;

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x5566);
    TEST_CHECK(StubTmrDelete == 1);
    TEST_CHECK(StubTmrCreate == 1);
}

void test_write_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x2233;
    uint16_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x2345, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_TYPE_WR);
    TEST_CHECK(data == 0x2233);
}

void test_write_bad_node(void)
{
    uint16_t data = 0x5566;
    uint16_t val  = 0x7788;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_check(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    AppNode.Nmt.Tmr = -1;

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubTmrDelete == 0);
    TEST_CHECK(StubTmrCreate == 0);
}

void test_init_start(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x1122;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};
    AppNode.Nmt.Tmr = -1;

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(StubTmrDelete == 0);
    TEST_CHECK(StubTmrCreate == 1);
}

void test_init_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x2233;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x3456, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE  AppNode = { 0 };
    uint16_t data = 0x2233;
    uint16_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1017, 0, CO_OBJ_____RW), CO_THB_PROD, (CO_DATA)(&data)};

    err = COObjReset(&Obj, &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
}


TEST_LIST = {
    { "size_unknown",    test_size_unknown    },
    { "size_known",      test_size_known      },
    { "size_bad_size",   test_size_bad_size   },
    { "read_time",       test_read_time       },
    { "read_bad_node",   test_read_bad_node   },
    { "write_start",     test_write_start     },
    { "write_restart",   test_write_restart   },
    { "write_bad_index", test_write_bad_index },
    { "write_bad_node",  test_write_bad_node  },
    { "init_check",      test_init_check      },
    { "init_start",      test_init_start      },
    { "init_bad_index",  test_init_bad_index  },
    { "reset_unused",    test_reset_unused    },
    { NULL, NULL }
};
