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

#define CONmtHbConsActivate Stub_CONmtHbConsActivate
#include "co_core.h"
#include "acutest.h"

/******************************************************************************
* STUB FUNCTIONS
******************************************************************************/

CO_ERR CONmtHbConsActivate(CO_HBCONS *hbc, uint16_t time, uint8_t nodeid)
{
    return CO_ERR_NONE;
}

/******************************************************************************
* TEST CASES - SIZE
******************************************************************************/

void test_size_sub_0(void)
{
    CO_NODE  AppNode = { 0 };
    uint8_t  data = 0;
    CO_OBJ   Obj = { CO_KEY(0x1016, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 1);
}

void test_size_consumer(void)
{
    CO_NODE   AppNode = { 0 };
    CO_HBCONS data = { 0 };
    CO_OBJ    Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};
    uint32_t  size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_bad_size(void)
{
    CO_NODE   AppNode = { 0 };
    CO_HBCONS data = { 0 };
    CO_OBJ    Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};
    uint32_t  size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_sub_0(void)
{
    CO_NODE  AppNode = { 0 };
    uint8_t  data = 0x22;
    uint8_t  val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1016, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x22);
}

void test_read_consumer(void)
{
    CO_NODE  AppNode = { 0 };
    CO_HBCONS data = { 0 };
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};
    data.Time = 0x1122;
    data.NodeId = 0x33;

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x00331122);
}

void test_read_bad_node(void)
{
    CO_HBCONS data = { 0 };
    uint32_t val  = 0x11223344;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x11223344);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_sub_0(void)
{
    CO_NODE  AppNode = { 0 };
    uint8_t  data = 0x22;
    uint8_t  val  = 0x33;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1016, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data == 0x33);
}

void test_write_consumer(void)
{
    CO_NODE  AppNode = { 0 };
    CO_HBCONS data = { 0 };
    uint32_t val  = 0x55667788;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};
    data.Time = 0x2233;
    data.NodeId = 0x44;
    data.Node = &AppNode;

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data.Time == 0x7788);
    TEST_CHECK(data.NodeId == 0x66);
}

void test_write_bad_node(void)
{
    CO_HBCONS data = { 0 };
    uint32_t  val  = 0;
    CO_ERR    err;
    CO_OBJ    Obj = { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_consumer(void)
{
    CO_NODE   AppNode = { 0 };
    uint8_t   data8  = 1;
    CO_HBCONS data = { 0 };
    CO_ERR    err;
    CO_OBJ    Obj[2] = {
        { CO_KEY(0x1016, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data8)},
        { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)},
    };
    data.NodeId = 0x11;
    data.Time = 0x2233;
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
}

void test_init_bad_index(void)
{
    CO_NODE  AppNode = { 0 };
    uint8_t  data8 = 1;
    uint32_t data32 = 0x11223344;
    CO_ERR   err;
    CO_OBJ   Obj[2] = {
        { CO_KEY(0x2123, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data8)},
        { CO_KEY(0x2123, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data32)},
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE   AppNode = { 0 };
    uint8_t   data8 = 1;
    CO_HBCONS data = { 0 };
    CO_ERR    err;
    CO_OBJ    Obj[2] = {
        { CO_KEY(0x1016, 0, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data8)},
        { CO_KEY(0x1016, 1, CO_OBJ_____RW), CO_THB_CONS, (CO_DATA)(&data)},
    };
    data.NodeId = 0x11;
    data.Time = 0x2233;
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjReset(&Obj[0], &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data8 == 1);
    TEST_CHECK(data.NodeId == 0x11);
    TEST_CHECK(data.Time == 0x2233);
}


TEST_LIST = {
    { "size_sub_0",      test_size_sub_0      },
    { "size_consumer",   test_size_consumer   },
    { "size_bad_size",   test_size_bad_size   },
    { "read_sub_0",      test_read_sub_0      },
    { "read_consumer",   test_read_consumer   },
    { "read_bad_node",   test_read_bad_node   },
    { "write_sub_0",     test_write_sub_0     },
    { "write_consumer",  test_write_consumer  },
    { "write_bad_node",  test_write_bad_node  },
    { "init_consumer",   test_init_consumer   },
    { "init_bad_index",  test_init_bad_index  },
    { "reset_unused",    test_reset_unused    },
    { NULL, NULL }
};
