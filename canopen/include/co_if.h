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

#ifndef CO_IF_H_
#define CO_IF_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_types.h"
#include "co_if_can.h"
#include "co_if_timer.h"
#include "co_if_nvm.h"

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

struct CO_NODE_T;              /* Declaration of canopen node structure      */

typedef struct CO_IF_DRV_T {         /*!< Type, which links driver functions */
    const CO_IF_CAN_DRV    *Can;     /*!< Link to CAN driver functions       */
    const CO_IF_TIMER_DRV  *Timer;   /*!< Link to Timer driver functions     */
    const CO_IF_NVM_DRV    *Nvm;     /*!< Link to NVM driver functions       */
} CO_IF_DRV;

typedef struct CO_IF_T {          /*!< Driver interface structure            */
    struct CO_NODE_T *Node;       /*!< Link to parent node                   */
    CO_IF_DRV        *Drv;        /*!< Link to hardware driver functions     */
} CO_IF;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief  INITIALIZE INTERFACES
*
*    This function initialize all hardware interfaces.
*
* \param cif
*    pointer to the interface structure
*
* \param node
*    pointer to the parent node
*
* \param freq
*    timer clock frequency
*/
void COIfInit(CO_IF *cif, struct CO_NODE_T *node, uint32_t freq);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* CO_IF_H_ */
