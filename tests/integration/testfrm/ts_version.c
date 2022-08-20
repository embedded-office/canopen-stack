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

#include "ts_version.h"
#include "ts_printf.h"

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

void TS_PrintVersion(const uint32_t version, const char * format, const uint8_t base)
{
    const char digChar[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    uint32_t next_divisor;
    uint32_t divisor = 1u;
    uint8_t  fmt     = 0u;
    uint8_t  digit;
    char     last;

    if ((format == NULL) || ((base != 10) && (base != 16))) {
        TS_Printf("%d", version);
    } else {
        /* figure out initial divisor */
        while (format[fmt] != 0) {
            fmt++;
        }
        while ((uint32_t)(version / divisor) > (uint32_t)(base - 1)) {
            next_divisor = divisor * base;
            if (next_divisor > divisor) {
                divisor = next_divisor;
                fmt--;
            } else {
                break;
            }
        }
        if (fmt == 0) {
            /* Version overflows format specification */
            TS_Printf("?");
        } else {
            fmt--;
            last = 0;
            do {
                digit = (version / divisor % base);
                divisor /= base;
                if (format[fmt] != '-') {       /* suppress digits for '-' */
                    if (format[fmt] != last) {  /* changed format key? */
                        if (last != 0) {
                            TS_Printf(".");
                        }
                        if (format[fmt] == 'x') {
                            TS_Printf("x");
                        }
                    }
                    if (format[fmt] != 'x') {
                        if (digit > 0) {
                            /* Print Digit 1..9 */
                            TS_Printf("%c", digChar[digit]);
                        } else {
                            /* Print Digit 0 */
                            if ((format[fmt + 1] != format[fmt])  ||
                                (format[fmt + 1] == 0u)) {
                                TS_Printf("0");
                            }
                        }
                    }
                    last = format[fmt];
                }
                fmt++;
            } while (divisor > 0u);
        }
    }
}
