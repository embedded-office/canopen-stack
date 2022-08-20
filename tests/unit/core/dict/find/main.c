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

void test_no_init(void)
{
    CO_DICT  dict = { 0 };
    CO_OBJ  *result;

    result = CODictFind(&dict, CO_DEV(0x1234, 0x56));

    TEST_CHECK(result == NULL);
}

void test_empty(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[1] = {
        CO_OBJ_DICT_ENDMARK
    };
    CODictInit(&node.Dict, &node, &obj[0], 1);

    result = CODictFind(&node.Dict, CO_DEV(0x1234, 0x56));

    TEST_CHECK(result == NULL);
}

void test_single(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[1] = {
        { CO_KEY(0x1234, 0x56, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(0) }
    };
    CODictInit(&node.Dict, &node, &obj[0], 1);

    result = CODictFind(&node.Dict, CO_DEV(0x1234, 0x56));

    TEST_CHECK(result == &obj[0]);
}

void test_first(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[4] = {
        { CO_KEY(0x1234, 0x56, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(0) },
        { CO_KEY(0x2345, 0x67, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(1) },
        { CO_KEY(0x3456, 0x78, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(2) },
        { CO_KEY(0x4567, 0x89, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(3) }
    };
    CODictInit(&node.Dict, &node, &obj[0], 4);

    result = CODictFind(&node.Dict, CO_DEV(0x1234, 0x56));

    TEST_CHECK(result == &obj[0]);
}

void test_middle(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[4] = {
        { CO_KEY(0x1234, 0x56, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(0) },
        { CO_KEY(0x2345, 0x67, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(1) },
        { CO_KEY(0x3456, 0x78, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(2) },
        { CO_KEY(0x4567, 0x89, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(3) }
    };
    CODictInit(&node.Dict, &node, &obj[0], 4);

    result = CODictFind(&node.Dict, CO_DEV(0x3456, 0x78));

    TEST_CHECK(result == &obj[2]);
}

void test_last(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[4] = {
        { CO_KEY(0x1234, 0x56, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(0) },
        { CO_KEY(0x2345, 0x67, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(1) },
        { CO_KEY(0x3456, 0x78, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(2) },
        { CO_KEY(0x4567, 0x89, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(3) }
    };
    CODictInit(&node.Dict, &node, &obj[0], 4);

    result = CODictFind(&node.Dict, CO_DEV(0x4567, 0x89));

    TEST_CHECK(result == &obj[3]);
}

void test_not_found(void)
{
    CO_NODE  node = { 0 };
    CO_OBJ  *result;
    CO_OBJ   obj[4] = {
        { CO_KEY(0x1234, 0x56, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(0) },
        { CO_KEY(0x2345, 0x67, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(1) },
        { CO_KEY(0x3456, 0x78, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(2) },
        { CO_KEY(0x4567, 0x89, CO_OBJ_D___RW), CO_TUNSIGNED8, (CO_DATA)(3) }
    };
    CODictInit(&node.Dict, &node, &obj[0], 4);

    result = CODictFind(&node.Dict, CO_DEV(0x5678, 0x90));

    TEST_CHECK(result == NULL);
}


TEST_LIST = {
    { "no_init",       test_no_init       },
    { "empty",         test_empty         },
    { "single",        test_single        },
    { "first",         test_first         },
    { "middle",        test_middle        },
    { "last",          test_last          },
    { "not_found",     test_not_found     },
    { NULL, NULL }
};
