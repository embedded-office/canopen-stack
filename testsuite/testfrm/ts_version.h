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

#ifndef TS_VERSION_H_
#define TS_VERSION_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_env.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define TS_NAME            "Test: Framework"
#define TS_VER_BASE        10
#define TS_VER_FORMAT      "VVMMBB"
#define TS_VERSION         (10000)


#if defined(__CC_ARM)
# define TS_ENV_NAME       "ENV : ARM Compiler"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VRPBBB"
# define TS_ENV_VER        (__ARMCC_VERSION)
#endif /* defined(__CC_ARM) */

#if defined(__DCC__)
# define TS_ENV_NAME       "ENV : Diab C/C++"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VRPP"
# define TS_ENV_VER        (__VERSION_NUMBER__)
#endif /* defined(__DCC__) */

#if defined(__ghs__)
# define TS_ENV_NAME       "ENV : Green Hill C/C++"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VRP"
# define TS_ENV_VER        (__GHS_VERSION_NUMBER__)
#endif /* defined(__ghs__) */

#if defined(__GNUC__)
# define TS_ENV_NAME       "ENV : GCC C/C++"
# define TS_ENV_VER_BASE   10
# if defined(__GNUC_PATCHLEVEL__)
#  define TS_ENV_VER_FORMAT "RRBBPP"
#  define TS_ENV_VER (__GNUC__ * 10000L \
                      + __GNUC_MINOR__ * 100L \
                      + __GNUC_PATCHLEVEL__)
# else
#  define TS_ENV_VER_FORMAT "RRBBXX"
#  define TS_ENV_VER (__GNUC__ * 10000L \
                      + __GNUC_MINOR__ * 100L)
# endif
#endif /* defined(__GNUC__) */

#if defined(__ICCARM__)
# define TS_ENV_NAME       "ENV : IAR C/C++"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VRR"
# define TS_ENV_VER        (__VER__)
#endif /* defined(__ICCARM__) */

#if defined(__KEIL__)
# define TS_ENV_NAME       "ENV : KEIL CARM"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VRR"
# define TS_ENV_VER        (__CA__)
#endif /* defined(__KEIL__) */

#if defined(__CWCC__)
# define TS_ENV_NAME       "ENV : Metrowerks CodeWarrior"
# define TS_ENV_VER_BASE   16
# define TS_ENV_VER_FORMAT "VRPP"
# define TS_ENV_VER        (__CWCC__)
#endif /* defined(__CWCC__) */

#if defined(_MSC_VER)
# define TS_ENV_NAME       "ENV : Microsoft Visual C++"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VVRR"
# define TS_ENV_VER        (_MSC_VER)
#endif /* defined(_MSC_VER) */

#if defined(__TMS470__)
# define TS_ENV_NAME       "ENV : TI CCS Compiler"
# define TS_ENV_VER_BASE   10
# define TS_ENV_VER_FORMAT "VVVRRRPPP"
# define TS_ENV_VER        (__TI_COMPILER_VERSION__)
#endif /* defined(__TMS470__) */

#if !defined(TS_ENV_NAME)
#error "Compiler is unknown"
#endif


#if defined(__CORTEX_M)
# if (__CORTEX_M == 0x00)
#  define TS_BSP_NAME       "BSP : Cortex M0 CMSIS"
#  define TS_BSP_VER_BASE   16
#  define TS_BSP_VER_FORMAT "VVVVMMMM"
#  define TS_BSP_VER        (__CM0_CMSIS_VERSION)
# endif /* #if (__CORTEX_M == 0x00) */

# if (__CORTEX_M == 0x03)
#  define TS_BSP_NAME       "BSP : Cortex M3 CMSIS"
#  define TS_BSP_VER_BASE   16
#  define TS_BSP_VER_FORMAT "VVVVMMMM"
#  define TS_BSP_VER        (__CM3_CMSIS_VERSION)
# endif /* #if (__CORTEX_M == 0x03) */

# if (__CORTEX_M == 0x04)
#  if defined(__FPU_PRESENT)
#   define TS_BSP_NAME       "BSP : Cortex M4F CMSIS"
#  else
#   define TS_BSP_NAME       "BSP : Cortex M4 CMSIS"
#  endif
#  define TS_BSP_VER_BASE   16
#  define TS_BSP_VER_FORMAT "VVVVMMMM"
#  define TS_BSP_VER        (__CM4_CMSIS_VERSION)
# endif /* #if (__CORTEX_M == 0x04) */
#endif /* #if defined(__CORTEX_M) */

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/*!
* \details This function prints the given value as a version string with a given format definition
*          string. The format definition accepts the following digit characters:
*          - any letter (A-Z): print as version number
*          - character '-': supress the corresponding value(s)
*          - character 'x': print as variant 'x'
*          Different version/variant parts are separated with a '.'
*
*          The number of format digits defines the multiplier of this part of the version
*          information. e.g. value 123 with format "vvr" is 12.3, whereas value 123 with
*          format "vrr" is 1.23.
*
*          To support hex-values, the given base must be 16. This allows to print the value 0x123
*          with format "vrr" as 1.23
*
* \param   version
*          Single version value, encoded as specified with the format definition
*
* \param   format
*          The format specification
*
* \param   base
*          Base of a single digit in the format specifier (base must be 10 or 16)
*/
/*------------------------------------------------------------------------------------------------*/
void TS_PrintVersion(const uint32_t version,
                     const char *   format,
                     const uint8_t  base);

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif /* TS_VERSION_H_ */
