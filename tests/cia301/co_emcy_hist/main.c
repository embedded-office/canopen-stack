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

void test_size_sub_0(void)
{
    CO_NODE  AppNode;
    uint8_t  data;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 1);
}

void test_size_sub_1(void)
{
    CO_NODE  AppNode;
    uint32_t data;
    CO_OBJ   Obj = { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 4);
}

void test_size_bad_size(void)
{
    CO_NODE  AppNode;
    uint32_t data;
    CO_OBJ   Obj = { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};
    uint32_t size;

    size = COObjGetSize(&Obj, &AppNode, 7);

    TEST_CHECK(size == 4);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_sub_0(void)
{
    CO_NODE  AppNode;
    uint8_t  data = 0x22;
    uint8_t  val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(val == 0x22);
}

void test_read_sub_1(void)
{
    CO_NODE  AppNode;
    uint32_t data = 0x22334455;
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, &AppNode, &val, sizeof(val));

    /* Note: reading the history requires integration testing, because the
     * subindex entries are re-mapped in dependence to the number of stored
     * entries in the EMCY history.
     */
    TEST_CHECK(err == CO_ERR_TYPE_RD);
}

void test_read_bad_node(void)
{
    CO_NODE  AppNode;
    uint8_t  data = 0x33;
    uint8_t  val  = 0x44;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjRdValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    TEST_CHECK(val == 0x44);
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_sub_0(void)
{
    CO_NODE  AppNode;
    uint8_t  data = 0x22;
    uint8_t  val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_NONE);
}

void test_write_bad_sub_0(void)
{
    CO_NODE  AppNode;
    uint8_t  data = 0x33;
    uint8_t  val  = 0x44;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_OBJ_RANGE);
}

void test_write_sub_1(void)
{
    CO_NODE  AppNode;
    uint32_t data = 0x33445566;
    uint32_t val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, &AppNode, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_TYPE_WR);
}

void test_write_bad_node(void)
{
    CO_NODE  AppNode;
    uint8_t  data = 0x33;
    uint8_t  val  = 0;
    CO_ERR   err;
    CO_OBJ   Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST,  (CO_DATA)(&data)};

    err = COObjWrValue(&Obj, NULL, &val, sizeof(val));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_hist_min(void)
{
    CO_NODE AppNode;
    uint8_t data8;
    uint8_t data32;
    CO_ERR  err;
    CO_OBJ  Obj[2] = {
        { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data8)},
        { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data32)},
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 2);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(AppNode.Emcy.Hist.Max == 1);
}

void test_init_hist_8(void)
{
    CO_NODE AppNode;
    uint8_t data8 = 3;
    uint8_t data32;   /* just for testing initialization: one data for all */
    CO_ERR  err;
    CO_OBJ  Obj[4] = {
        { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data8)},
        { CO_KEY(0x1003, 1, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data32)},
        { CO_KEY(0x1003, 2, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data32)},
        { CO_KEY(0x1003, 3, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data32)}
    };
    CODictInit(&AppNode.Dict, &AppNode, &Obj[0], 4);

    err = COObjInit(&Obj[0], &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(AppNode.Emcy.Hist.Max == 3);
}

void test_init_bad_hist(void)
{
    CO_NODE AppNode;
    uint8_t data8;
    CO_ERR  err;
    CO_OBJ  Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data8)};

    CODictInit(&AppNode.Dict, &AppNode, &Obj, 1);

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_TYPE_INIT);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_unused(void)
{
    CO_NODE AppNode;
    uint8_t data8;
    CO_ERR  err;
    CO_OBJ  Obj = { CO_KEY(0x1003, 0, CO_OBJ_____RW), CO_TEMCY_HIST, (CO_DATA)(&data8)};

    err = COObjReset(&Obj, &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
}


TEST_LIST = {
    { "size_sub_0",      test_size_sub_0      },
    { "size_sub_1",      test_size_sub_1      },
    { "bad_size",        test_size_bad_size   },
    { "read_sub_0",      test_read_sub_0      },
    { "read_sub_1",      test_read_sub_1      },
    { "read_bad_node",   test_read_bad_node   },
    { "write_sub_0",     test_write_sub_0     },
    { "write_bad_sub_0", test_write_bad_sub_0 },
    { "write_sub_1",     test_write_sub_1     },
    { "write_bad_node",  test_write_bad_node  },
    { "init_hist_min",   test_init_hist_min   },
    { "init_hist_8",     test_init_hist_8     },
    { "init_bad_hist",   test_init_bad_hist   },
    { "reset_unused",    test_reset_unused    },
    { NULL, NULL }
};
