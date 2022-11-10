#include <driverlib.h>

// *****************************************************************************
// MSP430FR5994 Unused Vectors
// *****************************************************************************
// UNUSED_HWI_ISR()
//
// The default linker command file created by CCS links all interrupt vectors to
// their specified address location. This gives you a warning for vectors that
// are not associated with an ISR function. The following function (and pragma's)
// handles all interrupt vectors.
//
// Just make sure you comment out the vector pragmas handled by your own code.
// For example, you will receive a "program will not fit into" error if you do
// not comment out the WDT vector below. This occurs since the linker tries to
// fit both of the vector addresses into the same memory locations ... and they
// won't fit.
// *****************************************************************************
#pragma vector = ADC12_B_VECTOR                                                 // ADC
#pragma vector = AES256_VECTOR                                                  // AES encryption
#pragma vector = COMP_E_VECTOR                                                  // Comparator
#pragma vector = DMA_VECTOR                                                     // DMA
#pragma vector = EUSCI_A0_VECTOR                                                // Serial Port 0 (side A)
#pragma vector = EUSCI_A1_VECTOR                                                // Serial Port 1 (side A)
#pragma vector = EUSCI_A2_VECTOR                                                // Serial Port 2 (side A)
#pragma vector = EUSCI_A3_VECTOR                                                // Serial Port 3 (side A)
#pragma vector = EUSCI_B0_VECTOR                                                // Serial Port 0 (side B)
#pragma vector = EUSCI_B1_VECTOR                                                // Serial Port 1 (side B)
#pragma vector = EUSCI_B2_VECTOR                                                // Serial Port 2 (side B)
#pragma vector = EUSCI_B3_VECTOR                                                // Serial Port 3 (side B)
#pragma vector = LEA_SC_VECTOR                                                  // LEA (Low Energy Accelerator)
#pragma vector = PORT1_VECTOR                                                   // GPIO Port 1
#pragma vector = PORT2_VECTOR                                                   // GPIO Port 2
#pragma vector = PORT3_VECTOR                                                   // GPIO Port 3
#pragma vector = PORT4_VECTOR                                                   // GPIO Port 4
//#pragma vector = PORT5_VECTOR                                                 // GPIO Port 5
#pragma vector = PORT6_VECTOR                                                   // GPIO Port 6
#pragma vector = PORT7_VECTOR                                                   // GPIO Port 7
#pragma vector = PORT8_VECTOR                                                   // GPIO Port 8
#pragma vector = RESET_VECTOR                                                   // RST
#pragma vector = RTC_C_VECTOR                                                   // Real-Time Clock
#pragma vector = SYSNMI_VECTOR                                                  // System Non-Maskable
#pragma vector = TIMER0_A0_VECTOR                                               // TA0 - CCR0
#pragma vector = TIMER0_A1_VECTOR                                               // TA0 - all other timer interrupts
#pragma vector = TIMER0_B0_VECTOR                                               // TB0 - CCR0
#pragma vector = TIMER0_B1_VECTOR                                               // TB0 - all other timer interrupts
#pragma vector = TIMER1_A0_VECTOR                                               // TA1 - CCR0
//#pragma vector = TIMER1_A1_VECTOR                                             // TA1 - all other timer interrupts
#pragma vector = TIMER2_A0_VECTOR                                               // TA2 - CCR0
#pragma vector = TIMER2_A1_VECTOR                                               // TA2 - all other timer interrupts
#pragma vector = TIMER3_A0_VECTOR                                               // TA3 - CCR0
#pragma vector = TIMER3_A1_VECTOR                                               // TA3 - all other timer interrupts
#pragma vector = TIMER4_A0_VECTOR                                               // TA4 - CCR0
#pragma vector = TIMER4_A1_VECTOR                                               // TA4 - all other timer interrupts
#pragma vector = UNMI_VECTOR                                                    // User Non-Maskable
#pragma vector = WDT_VECTOR                                                     // Watchdog Timer
__interrupt void UNUSED_HWI_ISR (void)
{
    __no_operation();
}

