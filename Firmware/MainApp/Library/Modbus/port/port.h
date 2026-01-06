#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

/* AT32F403A specific includes */
#include "at32f403a_407.h"

#define INLINE                      inline
#define PR_BEGIN_EXTERN_C           extern "C" {
#define PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION()    __disable_irq()
#define EXIT_CRITICAL_SECTION()     __enable_irq()

typedef uint8_t         BOOL;
typedef unsigned char   UCHAR;
typedef char            CHAR;
typedef uint16_t        USHORT;
typedef int16_t         SHORT;
typedef uint32_t        ULONG;
typedef int32_t         LONG;


void                    modbusInit(void);
void                    modbusPoll(void);

#endif
