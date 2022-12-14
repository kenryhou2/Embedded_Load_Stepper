lab_05b_wdtBlink

The goal of this lab is to blink the LED. Rather than using a __delay_cycles() 
function, we?ll actually use the watchdog timer to tell us when to toggle the LED.

In the last lab we used the Watchdog timer as a ? well, watchdog timer. In all 
other exercises, thus far, we just turned it off with WDT_A_hold().

In this lab exercise, we?re going to use it as a standard timer (called 
?interval? timer) to generate a periodic interrupt. In the interrupt service 
routine, we?ll toggle the LED.

As we write the ISR code, you may notice that the Watchdog Interval Timer 
interrupt has a dedicated interrupt vector. (Whereas the GPIO Port interrupt 
had 8 interrupts that shared one vector.) 

Basic Steps:
- Import project from DriverLib Examples
- Review the example's code
- Run and observe the LED flashing
- Change the WDT_A timers rate
- Run, to watch the timer run slower!


File source code in this readme file:
- wdt_a_ex2_intervalACLK.c

Final code ... you can copy from this if you want to save typing time & effort.


//******************************************************************************
//!
//! WDT - Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
//!
//!  Toggle P1.0 using software timed by WDT ISR. Toggle rate is exactly 250ms
//!  based on 32kHz ACLK WDT clock source. In this example the WDT is configured
//!  to divide 32768 watch-crystal(2^15) by 2^13 with an ISR triggered
//!  @ 4Hz = [WDT CLK source/32768].
//!  ACLK = REFO , MCLK = SMCLK = default DCO ~1.045MHz
//!
//!  Tested On: MSP430FR6989
//!             -----------------
//!         /|\|                 |
//!          | |                 |
//!          --|RST              |
//!            |             P1.0|-->LED
//!            |                 |
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - WDT peripheral
//! - GPIO Port peripheral
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - WDT_A_VECTOR
//!
//******************************************************************************
#include "driverlib.h"

void main(void)
{
    //Initialize WDT module in timer interval mode,
    //with ACLK as source at an interval of 250 ms.
	WDT_A_initIntervalTimer( WDT_A_BASE,
                             WDT_A_CLOCKSOURCE_ACLK,
                             //WDT_A_CLOCKDIVIDER_8192);
                             WDT_A_CLOCKDIVIDER_32K);

    WDT_A_start( WDT_A_BASE );

    //Enable Watchdog Interupt
    SFR_clearInterrupt( SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT );
    SFR_enableInterrupt( SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT );

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
    );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    //Enter LPM3, enable interrupts
    __bis_SR_register( LPM3_bits + GIE );

    //For debugger
    __no_operation();
}

//Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(WDT_VECTOR)))
#endif
void WDT_A_ISR(void)
{
    //Toggle P1.0
    GPIO_toggleOutputOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0
    );
}

/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/


