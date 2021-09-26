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

#ifndef APP_DICT_H_
#define APP_DICT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_core.h"

/******************************************************************************
* PUBLIC TYPEDEF
******************************************************************************/

typedef struct {
    CO_OBJ   *Root;
    uint32_t  Length;
    uint32_t  Used;
} OD_DYN;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void ODInit(OD_DYN *self, CO_OBJ *root, uint32_t length);
void ODAddUpdate(OD_DYN *self, uint32_t key, const CO_OBJ_TYPE *type, CO_DATA data);
void ODSetSpec(OD_DYN *self, CO_NODE_SPEC *spec);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* APP_DICT_H_ */
