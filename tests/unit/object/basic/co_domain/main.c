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
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    uint32_t   size;

    size = COObjGetSize(&Obj, &AppNode, 0);

    TEST_CHECK(size == 8);
}

void test_size_known(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    uint32_t   size;

    size = COObjGetSize(&Obj, &AppNode, 8);

    TEST_CHECK(size == 8);
}

void test_size_small(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    uint32_t   size;

    size = COObjGetSize(&Obj, &AppNode, 5);

    TEST_CHECK(size == 5);
}

void test_size_large(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    uint32_t   size;

    size = COObjGetSize(&Obj, &AppNode, 12);

    TEST_CHECK(size == 8);
}

void test_bad_size(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    uint32_t   size;

    size = COObjGetSize(&Obj, &AppNode, 6);

    TEST_CHECK(size == 6);
}

/******************************************************************************
* TEST CASES - READ
******************************************************************************/

void test_read_all(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    uint8_t    var[9] = { 0 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    for (int i=0; i<8; i++) {
        TEST_CHECK(var[i] == mem[i]);
    }
    TEST_CHECK(var[8] == 0);
}

void test_read_part(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    uint8_t    var[5] = { 0 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjRdValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    for (int i=0; i<5; i++) {
        TEST_CHECK(var[i] == mem[i]);
    }
}

void test_read_bad_node(void)
{
    uint8_t    mem[8] = { 1,2,3,4,5,6,7,8 };
    uint8_t    var[8] = { 0 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjRdValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    for (int i=0; i<8; i++) {
        TEST_CHECK(var[i] == 0);
    }
}

/******************************************************************************
* TEST CASES - WRITE
******************************************************************************/

void test_write_all(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 0 };
    uint8_t    var[9] = { 1,2,3,4,5,6,7,8,9 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    for (int i=0; i<8; i++) {
        TEST_CHECK(mem[i] == 1+i);
    }
}

void test_write_part(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 0 };
    uint8_t    var[5] = { 1,2,3,4,5 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjWrValue(&Obj, &AppNode, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_NONE);
    for (int i=0; i<5; i++) {
        TEST_CHECK(mem[i] == 1+i);
    }
    TEST_CHECK(mem[5] == 0);
    TEST_CHECK(mem[6] == 0);
    TEST_CHECK(mem[7] == 0);
}

void test_write_bad_node(void)
{
    uint8_t    mem[8] = { 0 };
    uint8_t    var[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 0, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjWrValue(&Obj, NULL, &var, sizeof(var));

    TEST_CHECK(err == CO_ERR_BAD_ARG);
    for (int i=0; i<8; i++) {
        TEST_CHECK(var[i] == 1+i);
    }
}

/******************************************************************************
* TEST CASES - INIT
******************************************************************************/

void test_init_offset(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 0 };
    CO_OBJ_DOM data = { 4, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjInit(&Obj, &AppNode);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data.Offset == 0);
}

/******************************************************************************
* TEST CASES - RESET
******************************************************************************/

void test_reset_offset(void)
{
    CO_NODE    AppNode = { 0 };
    uint8_t    mem[8] = { 0 };
    uint8_t    var[8] = { 1,2,3,4,5,6,7,8 };
    CO_OBJ_DOM data = { 4, sizeof(mem), (uint8_t *)&mem[0] };
    CO_OBJ     Obj  = { CO_KEY(0, 0, CO_OBJ_____RW), CO_TDOMAIN, (CO_DATA)(&data)};
    CO_ERR     err;

    err = COObjReset(&Obj, &AppNode, 7);

    TEST_CHECK(err == CO_ERR_NONE);
    TEST_CHECK(data.Offset == 7);
}


TEST_LIST = {
    { "size_unknown",   test_size_unknown   },
    { "size_known",     test_size_known     },
    { "size_small",     test_size_small     },
    { "size_large",     test_size_large     },
    { "bad_size",       test_bad_size       },
    { "read_all",       test_read_all       },
    { "read_part",      test_read_part      },
    { "read_bad_node",  test_read_bad_node  },
    { "write_all",      test_write_all      },
    { "write_part",     test_write_part     },
    { "write_bad_node", test_write_bad_node },
    { "init_offset",    test_init_offset    },
    { "reset_offset",   test_reset_offset   },
    { NULL, NULL }
};
