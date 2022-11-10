// --------------------------------------------------------------------------------------
// system_isr_routines.c  (for lab_09b_mpu_with_driverlib project)  ('FR5994 Launchpad)
//
// For more info on handling MSP430 System Reset Events, check out the wiki page:
//   http://processors.wiki.ti.com/index.php/Handling_MSP430_System_Reset_Events
// --------------------------------------------------------------------------------------
#include <stdio.h>
#include <driverlib.h>
#include "system_isr_routines.h"
#include "myMPU.h"

//***** Defines *************************************************************************
#define  LED1_PORT  GPIO_PORT_P4
#define  LED1_PIN   GPIO_PIN6
#define  LED2_PORT  GPIO_PORT_P1
#define  LED2_PIN   GPIO_PIN0
#define _DEBUG  1                            // Set to "0" to turn off Debug statements


//***** Global Variables ****************************************************************
#pragma PERSISTENT ( SYSNMIflag )
uint16_t SYSNMIflag[ 6 ]  = { 0, 0, 0, 0, 0, 0 };

//***************************************************************************************
// system_pre_init Function
//***************************************************************************************
void _system_pre_init(void)
{
    // Stop watchdog timer
    WDT_A_hold( WDT_A_BASE );

    // Check for a Reset Event flag
    RESET_checkLastCondition();

    initMPU();
}


//***************************************************************************************
// 'FR5994 System-Level Interrupt Service Routines
//***************************************************************************************
void RESET_checkLastCondition(void)
{
    SYSNMIflag[ SYSNMIflag_RST_EVENT ] = SYSRSTIV;
}


void RESET_handleEvent( uint16_t RST_flag )
{
    switch ( __even_in_range( RST_flag, SYSRSTIV__MPUSEG3IFG ))                  // Where SYSRSTIV__MPUSEG3IFG is last case in range
    {
        case SYSRSTIV__NONE: break;                                              // (0x0000) No interrupt pending
        case SYSRSTIV__BOR:                                                      // (0x0002) Brownout (BOR)
            RST_flag = 0;
            if (_DEBUG) printf( "(BOR) Brownout occurred (SYSRSTIV__BOR) \n" );
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );
            break;
        case SYSRSTIV__RSTNMI:                                                   // (0x0004) RSTIFG RST/NMI (BOR)
            RST_flag = 0;
            if (_DEBUG) printf( "(BOR) Reset/NMI (SYSRSTIV__RSTNMI) \n" );
            break;
        case SYSRSTIV__PMMSWBOR:                                                 // (0x0006) PMMSWBOR software BOR "Do BOR" (BOR)
            RST_flag = 0;
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );
            if (_DEBUG) printf( "(BOR) Software initiated BOR (SYSRSTIV__PMMSWBOR) \n" );
            break;
        case SYSRSTIV__LPM5WU:                                                   // (0x0008) Port LPMx.5 wakeup (BOR)
            RST_flag = 0;
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );
            if (_DEBUG) printf( "(BOR) Port LPM5 Wake Up (SYSRSTIV__LPM5WU) \n" );
            break;
        case SYSRSTIV__SECYV:                                                    // (0x000A) Security violation (BOR)
            RST_flag = 0;
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );
            if (_DEBUG) printf( "(BOR) Security violation (SYSRSTIV__SECYV) \n" );
            break;
        case SYSRSTIV__SVSHIFG:                                                  // (0x000E) SVSHIFG SVSH event (BOR)
            RST_flag = 0;
            GPIO_setOutputHighOnPin( LED1_PORT, LED1_PIN );
            if (_DEBUG) printf( "(BOR) Voltage Supply Supervisor (SYSRSTIV__SVSHIFG) \n" );
            break;
        case SYSRSTIV__PMMSWPOR:                                                 // (0x0014) PMMSWPOR software POR "Do POR" (POR)
            RST_flag = 0;
            GPIO_setOutputHighOnPin( LED2_PORT, LED2_PIN );
            if (_DEBUG) printf( "(POR) Software initiated POR (SYSRSTIV__PMMSWPOR) \n" );
            break;
        case SYSRSTIV__WDTIFG:                                                   // (0x0016) WDTIFG watchdog time-out (PUC)
            if (_DEBUG) printf( "(POR) Watchdog Timeout (SYSRSTIV__WDTIFG) \n" );
            break;
        case SYSRSTIV__WDTPW:                                                    // (0x0018) WDTPW password key violation (PUC)
            if (_DEBUG) printf( "(POR) WDT Key violation (SYSRSTIV__WDTPW) \n" );
            break;
        case SYSRSTIV__FRCTLPW:                                                  // (0x001A) FRCTLPW FRAM password key violation (PUC)
            if (_DEBUG) printf( "(PUC) FRAM Key violation (SYSRSTIV__FRCTLPW) \n" );
            break;
        case SYSRSTIV__UBDIFG:                                                   // (0x001C) Uncorrectable FRAM bit error detection (PUC)
            if (_DEBUG) printf( "(PUC) FRAM Uncorrectable bit Error (SYSRSTIV__UBDIFG) \n" );
            break;
        case SYSRSTIV__PERF:                                                     // (0x001E) Peripheral/config area fetch (PUC)
            if (_DEBUG) printf( "(PUC) Peripheral/config area fetch (SYSRSTIV__PERF) \n" );
            break;
        case SYSRSTIV__PMMPW:                                                    // (0x0020) PMMPW PMM password violation (PUC)
            if (_DEBUG) printf( "(PUC) PMM Password violation (SYSRSTIV__PMMPW) \n" );
            break;
        case SYSRSTIV__MPUPW:                                                    // (0x0022) MPUPW MPU password violation (PUC)
            if (_DEBUG) printf( "(PUC) MPU Password violation occurred (SYSRSTIV__MPUPW) \n" );
            break;
        case SYSRSTIV__CSPW:                                                     // (0x0024) CSPW CS password violation (PUC)
            if (_DEBUG) printf( "(PUC) CS Password violation occurred (SYSRSTIV__CSPW) \n" );
            break;
        case SYSRSTIV__MPUSEGPIFG:                                               // (0x0026) MPUSEGPIFG encapsulated IP memory segment violation (PUC)
            if (_DEBUG) printf( "(PUC) MPUSEGPIFG violation occurred (SYSRSTIV__MPUSEGPIFG) \n" );
            break;
        case SYSRSTIV__MPUSEGIIFG:                                               // (0x0028) MPUSEGIIFG information memory segment violation (PUC)
            if (_DEBUG) printf( "(PUC) Info Segment violation occurred (SYSRSTIV__MPUSEGIIFG) \n" );
            break;
        case SYSRSTIV__MPUSEG1IFG:                                               // (0x002A) MPUSEG1IFG MPU FRAM segment 1 memory violation (PUC)
            if (_DEBUG) printf( "(PUC) FRAM Segment 1 violation occurred (SYSRSTIV__MPUSEG1IFG) \n" );
            break;
        case SYSRSTIV__MPUSEG2IFG:                                               // (0x002C) MPUSEG2IFG MPU FRAM  segment 2 memory violation (PUC)
            if (_DEBUG) printf( "(PUC) FRAM Segment 2 violation occurred (SYSRSTIV__MPUSEG2IFG) \n" );
            break;
        case SYSRSTIV__MPUSEG3IFG:                                               // (0x002E) MPUSEG3IFG MPU FRAM sgement 3 memory violation (PUC)
            if (_DEBUG) printf( "(PUC) FRAM Segment 3 violation occurred (SYSRSTIV__MPUSEGPIFG) \n" );
            break;
        default: break;
    }
}


// System NMI Interrupt Service Routine
#pragma vector = SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
{
    switch ( __even_in_range( SYSSNIV, SYSSNIV__CBDIFG ))                       // Where SYSSNIV__CBDIFG is the last case in range
    {
        case SYSSNIV__NONE: break;                                              // (0x0000) No interrupt pending
        case SYSSNIV__UBDIFG:                                                   // (0x0004) Uncorrectable FRAM bit error detection
            break;
        case SYSSNIV__ACCTEIFG:                                                 // (0x0006) MPUSEGPIFG encapsulated IP memory segment violation
            break;
        case SYSSNIV__MPUSEGPIFG:                                               // (0x0008) MPUSEGPIFG encapsulated IP memory segment violation
            break;
        case SYSSNIV__MPUSEGIIFG:                                               // (0x000A) MPUSEGIIFG FRAM information memory (A-D) segment violation
            SYSNMIflag[ SYSNMIflag_INFO ] = 1;  // Set user flag to notify main program
            if (_DEBUG) printf("(NMI) FRAM Info Memory was violated (SYSSNIV.MPUSEGI)\n");
            break;
        case SYSSNIV__MPUSEG1IFG:                                               // (0x000C) MPUSEG1IFG FRAM segment 1 memory violation
            SYSNMIflag[ SYSNMIflag_SEG1 ] = 1;  // Set user flag to notify main program
            if (_DEBUG) printf("(NMI) FRAM Segment 1 Memory was violated (SYSSNIV.MPUSEG1)\n");
            break;
        case SYSSNIV__MPUSEG2IFG:                                               // (0x000E) MPUSEG2IFG FRAM segment 2 memory violation
            SYSNMIflag[ SYSNMIflag_SEG2 ] = 1;  // Set user flag to notify main program
            if (_DEBUG) printf("(NMI) FRAM Segment 2 Memory was violated (SYSSNIV.MPUSEG2)\n");
            break;
        case SYSSNIV__MPUSEG3IFG:                                               // (0x0010) MPUSEG3IFG FRAM segment 3 memory violation
            SYSNMIflag[ SYSNMIflag_SEG3 ] = 1;  // Set user flag to notify main program
            if (_DEBUG) printf("(NMI) FRAM Segment 3 Memory was violated (SYSSNIV.MPUSEGI3)\n");
            break;
        case SYSSNIV__VMAIFG:                                                   // (0x0012) VMAIFG Vacant memory access
            break;
        case SYSSNIV__JMBINIFG:                                                 // (0x0014) JMBINIFG JTAG mailbox input
            break;
        case SYSSNIV__JMBOUTIFG:                                                // (0x0016) JMBOUTIFG JTAG mailbox output
            break;
        case SYSSNIV__CBDIFG:                                                   // (0x0018) Correctable FRAM bit error detection
            break;
        case SYSSNIV__WPROT:                                                    // (0x001a) FRAM write protection detection
            break;
        case SYSSNIV__LEASCTO:                                                  // (0x001c) LEA time-out fault
            break;
        case SYSSNIV__LEASCCMD:                                                 // (0x001e) LEA command fault
            break;
        default: break;
    }
}

// User NMI Interrupt Service Routine
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
    switch (__even_in_range( SYSUNIV, SYSUNIV_OFIFG ))                          // Where SYSUNIV_OFIFG is the last item in range
    {
        case SYSUNIV__NONE: break;                                              // (0x0000) No Interrupt pending
        case SYSUNIV__NMIIFG:                                                   // (0x0002) SYSUNIV : NMIFG NMI pin
            break;
        case SYSUNIV__OFIFG:                                                    // (0x0004) SYSUNIV : OFIFG oscillator fault
            break;
        default: break;
    }
}

