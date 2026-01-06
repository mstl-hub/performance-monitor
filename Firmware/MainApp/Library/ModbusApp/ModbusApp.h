#ifndef _MODBUS_APP_H_
#define _MODBUS_APP_H_

#include "mb.h"
#include "port_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function prototypes */
void modbusFun(void* parameters) __attribute__((noreturn));
eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode);

#ifdef __cplusplus
}
#endif

#endif // _MODBUS_APP_H_
