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

#ifndef TS_OUTPUT_H_
#define TS_OUTPUT_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function realizes the routing of printed character to the test report interface.
*
* \param   arg
*          Not used, must be existing to cover callback interface
*
* \param   character
*          Single character byte for output
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PutChar(void *arg, char character);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_OUTPUT_H_ */
