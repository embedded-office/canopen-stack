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

#ifndef TS_PIPE_H_
#define TS_PIPE_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_types.h"
#include "ts_list.h"
#include "ts_lock.h"

/*!
* \addtogroup ts_frm_pipe   Test Framework - Named Pipe
* \details  The named pipe is a very easy way for communicate data. It is intended for a
*           point-to-point communication. The identification of a specific pipe is a string
*           (the name of the pipe).
*
*           This identification is given as first argument to all service function. The data
*           flow is fixed to FIFO (first in / first out).
*           
*           To simplify the usage as much as possible, the data is copied from the sender to
*           a memory buffer, internal to this module. Afterwards the receiver gets this data
*           copied to the receive buffer. The benefit of this is the possibility for sending
*           content of local variables with mixed size in one pipe as well.
* @{
*/

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function (re-)initializes the pipe management. All existing pipes, including
*          their content, are destroyed and the lock/unlock functions are disabled.
*
* \note    This function must be called, before any other function is used.
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PipeInit(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function setups the lock and unlock function for the pipe management.
*
* \param   lock
*          The reference to the pipe locking function
*
* \param   unlock
*          The reference to the pipe unlock function
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PipeSetup(TS_LOCK_FUNC lock, TS_UNLOCK_FUNC unlock);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function adds the given data into the pipe with the given name. If the addressed
*          pipe is not existing, this function creates the pipe.
*
* \param   name
*          Name of pipe
*
* \param   data
*          Reference to data
*
* \param   size
*          Size of referenced data
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PipeAdd(char *name, void *data, uint32_t size);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function gets the data (in FIFO order) from the pipe with the given name. If
*          the pipe is not existing, this function creates the pipe.
*
* \param   name
*          Name of pipe
*
* \param   data
*          reference to data buffer
*
* \param   size
*          Size of data buffer
*
* \retval  true   data is copied to the given data buffer
* \retval  false  no data available
*/
/*------------------------------------------------------------------------------------------------*/
bool TS_PipeGet(char *name, void *data, uint32_t size);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function clears the data within the pipe with the given name. If the pipe
*          is not existing, the function does nothing.
*
* \param   name
*          Name of the pipe
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PipeClear(char *name);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function clears and deletes the pipe with the given name. If the pipe is
*          not existing, the function call will do nothing.
*
* \param   name
*          Name of the pipe
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PipeDelete(char *name);
    
/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function checks the existence of the pipe with the given name.
*
* \param   name
*          Name of the pipe
*
* \retval  true   named pipe is existing
* \retval  false  named pipe is not existing
*/
/*------------------------------------------------------------------------------------------------*/
bool TS_PipeIsExisting(char *name);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function checks if the pipe with the given name is empty.
*
* \param   name
*          Name of the pipe
*
* \retval  true   named pipe is empty or not existing
* \retval  false  named pipe contains data
*/
/*------------------------------------------------------------------------------------------------*/
bool TS_PipeIsEmpty(char *name);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function counts the number of named pipes.
*
* \return  Number of named pipes
*/
/*------------------------------------------------------------------------------------------------*/
uint32_t TS_PipeGetNum(void);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function returns the size of the next item in pipe.
*
* \param   name
*          Name of the pipe
*
* \return  Size of item in bytes
*/
/*------------------------------------------------------------------------------------------------*/
uint32_t TS_PipeGetSize(char *name);

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function returns the used amount of memory of the internal pipe memory area.
*
* \return  Used memory in bytes
*/
/*------------------------------------------------------------------------------------------------*/
uint32_t TS_PipeMemUsed(void);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_PIPE_H_ */
/*! @} */
