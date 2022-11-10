/*
 * myTimer.h
 */

//***** Header Files **********************************************************
#include "USB_API/USB_Common/types.h"
#include <stdbool.h>
#include <driverlib.h>

#ifndef MYTIMER_H_
#define MYTIMER_H_

//***** Prototypes ************************************************************
void initTimers(void);

//***** Global Variables ******************************************************
extern bool bSend;

#endif /* MYTIMER_H_ */

