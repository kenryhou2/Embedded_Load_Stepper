lab_10d_usb

After exploring the HID & CDC LED ON/OFF examples, and the CDC Short Data Send
example, it's time to try creating our own USB device based upon the USB 
Developer Kit's Empty USB example.

This example will:
- Toggle an LED every second
- Read a pushbutton every second and light another LED if it is down
- Send a string every second to the host over USB HID interface indicating
  if the button is UP or DOWN
- If device is not enumerated, only the blinking light will be active (in the ISR)

Basic Steps:
- Import empty USB project from USB Developers Package (via TI Resource Explorer)
- Run Descriptor Tool
- Rename project
- Add myTimer.c and myTimer.h to project
- To main() setup code, init timers and gpio
- Create required global variables
- Add code to ST_ENUM_ACTIVE state to read button & send data
- Build & Debug (to load program to Launchpad)
- Run and watch LED toggle every second; also, ...
  ... wait for enumeration ... and driver to be loaded
- Once driver is active, start the "USB HID Demo tool", which lets us view the
  data coming from the Launchpad (think of it as a serial terminal for HID ports)
- You should see "DOWN" or "UP  " in terminal based upon the button's state


File source code in this readme file:
- main.c

Final code ... you can copy from this if you want to save typing time & effort.


// ----------------------------------------------------------------------------
// main.c  (for lab_10d_usb project)
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <string.h>

#include "driverlib.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_MSC_API/UsbMscScsi.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"
#include "USB_API/USB_MSC_API/UsbMscStateMachine.h"
#include "USB_API/USB_HID_API/UsbHid.h"
#include "USB_app/usbConstructs.h"
#include "myTimer.h"

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"

//***** Global Variables ******************************************************
char pbStr[5] = "";                             // Stores the string to send to the host
volatile unsigned short usiButton1 = 0;         // Stores the state of the pushbutton


//*****************************************************************************
// Main
//*****************************************************************************
void main (void)
{
    // Set up clocks/IOs.  initPorts()/initClocks() will need to be customized
    // for your application, but MCLK should be between 4-25MHz.  Using the
    // DCO/FLL for MCLK is recommended, instead of the crystal.  For examples 
    // of these functions, see the complete USB code examples.  Also see the 
    // Programmer's Guide for discussion on clocks/power.
    WDT_A_hold(WDT_A_BASE); // Stop watchdog timer
    
    // Minimum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
#ifndef DRIVERLIB_LEGACY_MODE
    PMM_setVCore(PMM_CORE_LEVEL_2);
#else
    PMM_setVCore(PMM_BASE, PMM_CORE_LEVEL_2);
#endif
    
    initPorts();             // Configure all GPIOs
    initClocks(8000000);     // Configure clocks

    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P2, GPIO_PIN1 );

    initTimers();

    USB_setup(TRUE,TRUE);    // Init USB & events; if a host is present, connect

    __enable_interrupt();    // Enable interrupts globally

    while (1)
    {
        // This switch() creates separate main loops, depending on whether USB 
        // is enumerated and active on the host, or disconnected/suspended.  If
        // you prefer, you can eliminate the switch, and just call
        // USB_connectionState() prior to sending data (to ensure the state is
        // ST_ENUM_ACTIVE). 
        switch(USB_connectionState())
        {
            // This case is executed while your device is connected to the USB
            // host, enumerated, and communication is active.  Never enter 
            // LPM3/4/5 in this mode; the MCU must be active or LPM0 during USB
            // communication.
            case ST_ENUM_ACTIVE:

                // If USB is present, sent the time to the host. Flag is set every sec
                if (bSend)
                {
                    bSend = FALSE;

                    usiButton1 = GPIO_getInputPinValue ( GPIO_PORT_P2, GPIO_PIN1 );

                    if ( usiButton1 == GPIO_INPUT_PIN_LOW ) {
                        // If button is down, turn on LED
                        GPIO_setOutputHighOnPin( GPIO_PORT_P4, GPIO_PIN7 );
                        pbStr[0] = 'D';
                        pbStr[1] = 'O';
                        pbStr[2] = 'W';
                        pbStr[3] = 'N';
                        pbStr[4] = '\n';
                    }
                    else {
                        // If button is up, turn off LED
                        GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN7 );
                        pbStr[0] = 'U';
                        pbStr[1] = 'P';
                        pbStr[2] = ' ';
                        pbStr[3] = ' ';
                        pbStr[4] = '\n';
                    }

                    // This function begins the USB send operation, and immediately
                    // returns, while the sending happens in the background.
                    // Send pbStr, 5 bytes, to intf #0 (which is enumerated as a
                    // HID port).  1000 retries.  (Retries will be attempted if the
                    // previous send hasn't completed yet).  If the bus isn't present,
                    // it simply returns and does nothing.
                    if (hidSendDataInBackground((BYTE*)pbStr, 5, HID0_INTFNUM, 1000))
                    {
                      _NOP();  // If it fails, it'll end up here.  Could happen if
                               // the cable was detached after the connectionState()
                    }           // check, or if somehow the retries failed
                }


            // These cases are executed while your device is:
            case ST_USB_DISCONNECTED: // physically disconnected from the host
            case ST_ENUM_SUSPENDED:   // connected/enumerated, but suspended
            case ST_NOENUM_SUSPENDED: // connected, enum started, but the host is unresponsive
            
                // In this example, for all of these states we enter LPM3.  If 
                // the host performs a "USB resume" from suspend, the CPU will
                // automatically wake.  Other events can also wake the
                // CPU, if their event handlers in eventHandlers.c are 
                // configured to return TRUE.
//                __bis_SR_register(LPM3_bits + GIE);
                __bis_SR_register(LPM0_bits + GIE);
                break;

            // The default is executed for the momentary state
            // ST_ENUM_IN_PROGRESS.  Almost always, this state lasts no more than a 
            // few seconds.  Be sure not to enter LPM3 in this state; USB 
            // communication is taking place, so mode must be LPM0 or active.
            case ST_ENUM_IN_PROGRESS:
            default:;
        }
    }  //while(1)
} //main()


//*****************************************************************************
// Interrupt Service Routines
//*****************************************************************************
/*  
 * ======== UNMI_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not found!
#endif
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
#ifndef DRIVERLIB_LEGACY_MODE
            UCS_clearAllOscFlagsWithTimeout(0);
            SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
#else
            UCS_clearAllOscFlagsWithTimeout(UCS_BASE, 0);
            SFR_clearInterrupt(SFR_BASE, SFR_OSCILLATOR_FAULT_INTERRUPT);

#endif
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:
            // If the CPU accesses USB memory while the USB module is 
            // suspended, a "bus error" can occur.  This generates an NMI, and
            // execution enters this case.  This should never occur.  If USB is
            // automatically disconnecting in your software, set a breakpoint
            // here and see if execution hits it.  See the Programmer's
            // Guide for more information. 
            SYSBERRIV = 0; // Clear bus error flag
            USB_disable(); // Disable USB -- USB must be reset after a bus error
    }
}
//Released_Version_4_20_00

/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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

