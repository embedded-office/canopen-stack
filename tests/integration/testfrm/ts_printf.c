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
/* This file is based on the project 'tinyprintf'. Find the original source code at
*  github: "https://github.com/cjlano/tinyprintf"
*/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "ts_printf.h"

/******************************************************************************
* PRIVATE TYPES
******************************************************************************/

typedef struct TS_PARAM_T {
    unsigned       LeadingZeros:1;                    /*!< Leading zeros                          */
    unsigned       Alternate:1;                       /*!< alternate form                         */
    unsigned       UpperCase:1;                       /*!< Upper case (for base16 only)           */
    unsigned       AlignLeft:1;                       /*!< align right(0, default)/left(1)        */
    unsigned int   FieldWidth;                        /*!< field width                            */
    unsigned       Sign;                              /*!< The sign to display (if any)           */
    unsigned int   Base;                              /*!< number base (e.g.: 8, 10, 16)          */
    char          *Buffer;                            /*!< Buffer to output                       */
} TS_PARAM;

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

static TS_OUT_FUNC PrintOutFunction;                  /*!< output function                        */
static void       *PrintOutArgument;                  /*!< output function argument               */
static TS_LOCK     ts_PrintLock = { 0 };

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void ulli2a(unsigned long long int num, TS_PARAM *p)
{
    int n = 0;
    unsigned long long int d = 1;
    char *bf = p->Buffer;
    while (num / d >= p->Base)
        d *= p->Base;
    while (d != 0) {
        int dgt = (int)(num / d);
        num %= d;
        d /= p->Base;
        if (n || dgt > 0 || d == 0) {
            *bf++ = (char)(dgt + (dgt < 10 ? '0' : (p->UpperCase ? 'A' : 'a') - 10));
            ++n;
        }
    }
    *bf = 0;
}

static void lli2a(long long int num, TS_PARAM *p)
{
    if (num < 0) {
        num = -num;
        p->Sign = '-';
    }
    ulli2a(num, p);
}

static void uli2a(unsigned long int num, TS_PARAM *p)
{
    int n = 0;
    unsigned long int d = 1;
    char *bf = p->Buffer;
    while (num / d >= p->Base)
        d *= p->Base;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= p->Base;
        if (n || dgt > 0 || d == 0) {
            *bf++ = (char)(dgt + (dgt < 10 ? '0' : (p->UpperCase ? 'A' : 'a') - 10));
            ++n;
        }
    }
    *bf = 0;
}

static void li2a(long num, TS_PARAM *p)
{
    if (num < 0) {
        num = -num;
        p->Sign = '-';
    }
    uli2a(num, p);
}

static void ui2a(unsigned int num, TS_PARAM *p)
{
    int n = 0;
    unsigned int d = 1;
    char *bf = p->Buffer;
    while (num / d >= p->Base)
        d *= p->Base;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= p->Base;
        if (n || dgt > 0 || d == 0) {
            *bf++ = (char)(dgt + (dgt < 10 ? '0' : (p->UpperCase ? 'A' : 'a') - 10));
            ++n;
        }
    }
    *bf = 0;
}

static void i2a(int num, TS_PARAM *p)
{
    if (num < 0) {
        num = -num;
        p->Sign = '-';
    }
    ui2a(num, p);
}

static int a2d(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}

static char a2u(char ch, const char **src, int base, unsigned int *nump)
{
    const char *p = *src;
    unsigned int num = 0;
    int digit;
    while ((digit = a2d(ch)) >= 0) {
        if (digit > base)
            break;
        num = num * base + digit;
        ch = *p++;
    }
    *src = p;
    *nump = num;
    return ch;
}

static void putchw(void *arg, TS_OUT_FUNC func, TS_PARAM *p)
{
    char ch;
    int n = p->FieldWidth;
    char *bf = p->Buffer;

    /* Number of filling characters */
    while (*bf++ && n > 0)
        n--;
    if (p->Sign)
        n--;
    if (p->Alternate && p->Base == 16)
        n -= 2;
    else if (p->Alternate && p->Base == 8)
        n--;

    /* Fill with space to align to the right, before alternate or sign */
    if (!p->LeadingZeros && !p->AlignLeft) {
        while (n-- > 0)
            func(arg, ' ');
    }

    /* print sign */
    if (p->Sign)
        func(arg, (char)(p->Sign));

    /* Alternate */
    if (p->Alternate && p->Base == 16) {
        func(arg, '0');
        func(arg, (p->UpperCase ? 'X' : 'x'));
    } else if (p->Alternate && p->Base == 8) {
        func(arg, '0');
    }

    /* Fill with zeros, after alternate or sign */
    if (p->LeadingZeros) {
        while (n-- > 0)
            func(arg, '0');
    }

    /* Put actual buffer */
    bf = p->Buffer;
    while ((ch = *bf++))
        func(arg, ch);

    /* Fill with space to align to the left, after string */
    if (!p->LeadingZeros && p->AlignLeft) {
        while (n-- > 0)
            func(arg, ' ');
    }
}

static void format(void *arg, TS_OUT_FUNC func, const char *fmt, va_list va)
{
    TS_PARAM p;
    char  bf[23];
    char  ch;
    p.Buffer = bf;

    while ((ch = *(fmt++))) {
        if (ch != '%') {
            func(arg, ch);
        } else {
            char lng = 0;  /* 1 for 32bit, 2 for 64bit */
            /* Initialize parameter structure */
            p.LeadingZeros = 0;
            p.Alternate = 0;
            p.FieldWidth = 0;
            p.AlignLeft = 0;
            p.Sign = 0;

            /* Flags */
            while ((ch = *(fmt++))) {
                switch (ch) {
                case '-':
                    p.AlignLeft = 1;
                    continue;
                case '0':
                    p.LeadingZeros = 1;
                    continue;
                case '#':
                    p.Alternate = 1;
                    continue;
                default:
                    break;
                }
                break;
            }

            /* Width */
            if (ch >= '0' && ch <= '9') {
                ch = a2u(ch, &fmt, 10, &(p.FieldWidth));
            }

            /* We accept 'x.y' format but don't support it completely:
             * we ignore the 'y' digit => this ignores 0-fill
             * size and makes it == width (ie. 'x') */
            if (ch == '.') {
              p.LeadingZeros = 1;  /* zero-padding */
              /* ignore actual 0-fill size: */
              do {
                ch = *(fmt++);
              } while ((ch >= '0') && (ch <= '9'));
            }

            if (ch == 'l') {
                ch = *(fmt++);
                lng = 1;
                if (ch == 'l') {
                  ch = *(fmt++);
                  lng = 2;
                }
            }

            switch (ch) {
            case 0:
                return;
            case 'u':
                p.Base = 10;
                if (2 == lng)
                    ulli2a(va_arg(va, unsigned long long int), &p);
                else
                  if (1 == lng)
                    uli2a(va_arg(va, unsigned long int), &p);
                else
                    ui2a(va_arg(va, unsigned int), &p);
                putchw(arg, func, &p);
                break;
            case 'd':
            case 'i':
                p.Base = 10;
                if (2 == lng)
                    lli2a(va_arg(va, long long int), &p);
                else
                  if (1 == lng)
                    li2a(va_arg(va, long int), &p);
                else
                    i2a(va_arg(va, int), &p);
                putchw(arg, func, &p);
                break;
            case 'x':
            case 'X':
                p.Base = 16;
                p.UpperCase = (ch == 'X')?1:0;
                if (2 == lng)
                    ulli2a(va_arg(va, unsigned long long int), &p);
                else
                  if (1 == lng)
                    uli2a(va_arg(va, unsigned long int), &p);
                else
                    ui2a(va_arg(va, unsigned int), &p);
                putchw(arg, func, &p);
                break;
            case 'o':
                p.Base = 8;
                ui2a(va_arg(va, unsigned int), &p);
                putchw(arg, func, &p);
                break;
            case 'c':
                func(arg, (char)(va_arg(va, int)));
                break;
            case 's':
                p.Buffer = va_arg(va, char *);
                putchw(arg, func, &p);
                p.Buffer = bf;
                break;
            case '%':
                func(arg, ch);
                break;
            default:
                break;
            }
        }
    }
}

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

TS_OUT_FUNC TS_PrintfInit(void *arg, TS_OUT_FUNC func)
{
    TS_OUT_FUNC result = PrintOutFunction;

    TS_LockSetup(&ts_PrintLock, NULL, NULL);
    PrintOutFunction = func;
    PrintOutArgument = arg;

    return (result);
}


void TS_Printf(char *fmt, ...)
{
    TS_LOCK_SR state = 0;
    
    state = TS_Lock(&ts_PrintLock);

    if (PrintOutFunction != NULL) {
        va_list va;
        va_start(va, fmt);
        format(PrintOutArgument, PrintOutFunction, fmt, va);
        va_end(va);
    }

    TS_Unlock(&ts_PrintLock, state);
}
