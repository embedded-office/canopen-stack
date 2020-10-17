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

#ifndef CO_PARA_H_
#define CO_PARA_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "co_obj.h"
#include "co_err.h"
#include "co_nmt.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_PARA____   0x0000     /*!< disable                                */
#define CO_PARA___E   0x0001     /*!< enable  (on command)                   */
#define CO_PARA__A_   0x0002     /*!< enable  (autonomously)                 */
#define CO_PARA__AE   0x0003     /*!< enable  (autonomously and on command)  */

#define CO_TPARA  ((CO_OBJ_TYPE *)&COTPara)  /*!< Object Type Parameter      */

/******************************************************************************
* PUBLIC TYPES
******************************************************************************/

/*! \brief PARAMETER GROUP INFO
*
*    This structure holds the informations of a parameter group. The
*    parameter group is used within the special function parameter object in
*    an object dictionary.
*
* \note
*    This structure may be placed into ROM to reduce RAM usage.
*/
typedef struct CO_PARA_T {
    uint32_t             Offset;   /*!< Offset in non-volatile memory area   */
    uint32_t             Size;     /*!< Size of parameter memory block       */
    uint8_t             *Start;    /*!< Start of parameter memory block      */
    uint8_t             *Default;  /*!< Start of default memory block        */
    enum CO_NMT_RESET_T  Type;     /*!< Parameter reset type                 */
    void                *Ident;    /*!< Ptr to User Ident-Code               */
    uint32_t             Value;    /*!< value when reading parameter object  */

} CO_PARA;

/******************************************************************************
* EXTERN CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE PARAMETER
*
*    This object type specializes the general handling of objects for the
*    object dictionary entries 0x1010 and 0x1011. These entries are designed
*    to provide the store and restore feature of a configurable parameter
*    group.
*/
extern const CO_OBJ_TYPE COTPara;

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*! \brief PARAMETER STORE
*
*    This function is responsible for the storing activities of the given
*    parameter group. The whole parameter group will be stored in NVM by
*    calling the nvm driver function.
*
* \param pg
*    Ptr to parameter group info
*
* \param node
*    Ptr to node info
*/
void COParaStore(CO_PARA *pg, struct CO_NODE_T *node);

/*! \brief PARAMETER RESTORE DEFAULT
*
*    This function is responsible for removing the changes on the parameter
*    values of the given parameter group. The changes in NVM of the given
*    parameter group will be replaced with the default values by calling
*    the user callback function \ref COParaDefault().
*
* \param pg
*    Ptr to parameter group info
*
* \param node
*    Ptr to node info
*/
void COParaRestore(CO_PARA *pg, struct CO_NODE_T *node);
    
/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/
    
/*! \brief CHECK PARAMETER OBJECT ACCESS
*
*    This function is responsible for checking the access to parameter
*    storage (0x1010) and parameter restore default (0x1011) object
*    entries. There are some plausibility checks of given parameter and
*    configuration. If all checks are passed, the requested activity is
*    returned with the encoding: 0 = restore parameter, 1 = store parameter.
*
* \param obj
*    Ptr to addressed object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    Ptr to data buffer
*
* \param size
*    Size of given data in buffer
*
* \retval  ==CO_ERR_NONE     store/restore activity successfull
* \retval  !=CO_ERR_NONE     error is detected and function aborted
*/
CO_ERR COParaCheck(CO_OBJ* obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/*! \brief PARAMETER OBJECT READ ACCESS
*
*    This function is responsible for the delivery of the store- and
*    restore-feature encoding during a read access of the parameter
*    object entry. The feature encoding is static configured within
*    the parameter group structure (\ref CO_PARA).
*
* \param obj
*    Ptr to addressed parameter object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    Ptr to data buffer
*
* \param size
*    Size of given data in buffer
*
* \retval  ==CO_ERR_NONE    store / restore successful
* \retval  !=CO_ERR_NONE    error is detected and function aborted
*/
CO_ERR COTypeParaRead(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/*! \brief PARAMETER OBJECT WRITE ACCESS
*
*    This function is responsible for performing all neccessary
*    activities during a write access of the parameter object entry.
*
* \param obj
*    Ptr to addressed parameter object entry
*
* \param node
*    reference to parent node
*
* \param buf
*    Ptr to data buffer
*
* \param size
*    Size of given data in buffer
*
* \retval  ==CO_ERR_NONE    store / restore successful
* \retval  !=CO_ERR_NONE    error is detected and function aborted
*/
CO_ERR COTypeParaWrite(struct CO_OBJ_T *obj, struct CO_NODE_T *node, void *buf, uint32_t size);

/******************************************************************************
* CALLBACK FUNCTIONS
******************************************************************************/

/*! \brief SET DEFAULT PARAMETER VALUES CALLBACK
*
*    This callback function will be called during restoring the default values
*    of a parameter group. The function is responsible for setting the factory
*    defaults in the current parameter group memory.
*
* \note
*    The parameter group info pointer is checked to be valid before calling
*    this function.
*
* \param pg
*    Ptr to parameter group info
*
* \retval  =0    parameter loading successful
* \retval  <0    error is detected and function aborted
*/
extern int16_t COParaDefault(CO_PARA *pg);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_PARA_H_ */
