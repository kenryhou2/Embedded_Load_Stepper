; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2014, Texas Instruments Incorporated
;  All rights reserved.
; 
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
; 
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
; 
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
; 
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
; 
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; 
; ******************************************************************************
;  
;                        MSP430 CODE EXAMPLE DISCLAIMER
; 
;  MSP430 code examples are self-contained low-level programs that typically
;  demonstrate a single peripheral function or device feature in a highly
;  concise manner. For this the code may rely on the device's power-on default
;  register values and settings such as the clock configuration and care must
;  be taken when combining code from several examples to avoid potential side
;  effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
;  for an API functional library-approach to peripheral configuration.
; 
; --/COPYRIGHT--
;*******************************************************************************
;   MSP430FR243x Demo - Timer_A, Ultra-Low Pwr Pulse Accumulator
;
;   Description: Timer_A is used as ultra-low power pulse counter. In this
;   example TAR is offset 10 counts, which are acculmulated on INCLK P4.0,
;   with the system in LPM4 - all internal clocks off. After 20 counts, TAR
;   will overflow requesting an interrupt, and waking the system. Timer_A is
;   then reconfigured with SMCLK as clock source in up mode - TACCR1 will then
;   toggle P1.0 every 50000 SMCLK cycles.
;
;                MSP430FR2433
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;        --->|P1.0/TA0CLK  P1.1|-->LED
;
;   Wei Zhao
;   Texas Instruments Inc.
;   Sep 2014
;   Code Composer Studio v6.0
;*******************************************************************************
            .cdecls C,LIST,"msp430.h"                       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack                                  ; Make stack linker segment ?known?

            .global _main
            .text                                           ; Assemble to Flash memory
            .retain                                         ; Ensure current section gets linked
            .retainrefs
_main

RESET       mov.w   #__STACK_END,SP                         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL                  ; Stop WDT

            bis.b   #BIT1,&P1DIR                            ; Set P1.1 to output direction
            mov.b   #0,&P1OUT                               ; P1.1 low
            bis.b   #BIT0,&P1SEL1                           ; P1.0 selected as TA0CLK
            bic.w   #LOCKLPM5,&PM5CTL0                      ; Unlock I/O pins

            mov.w   #TASSEL_0|MC_2|TACLR|TAIE,&TA0CTL       ; ACLK, count mode, clear TAR, enable interrupt
            mov.w   #0xFFEB,&TA0R                           ; Offset until TAR overflow           

            nop
            bis.w   #LPM4+GIE,SR                            ; Enter LPM4, enable interrupts
            nop                                             ; For debugger

Mainloop    xor.b   #BIT1,&P1OUT                            ; P1.1 = toggle
            bis.w   #CPUOFF,SR                              ; CPU is not required
            jmp     Mainloop                                ; Repeat
;-------------------------------------------------------------------------------
TIMER0_A1_ISR;    Common ISR for TACCR1 and overflow
;-------------------------------------------------------------------------------
            add.w   &TA0IV,PC                               ; Add Timer_A offset vector
            reti                                            ; No interrupt
            reti                                            ; TA0CCR1
            jmp     TACCR2_ISR                              ; TA0CCR2
            reti                                            ; reserved
            reti                                            ; reserved
            reti                                            ; reserved
            reti                                            ; reserved
            jmp     TA_over                                 ; TAIFG overflow
            reti


TA_over     mov.w   #TASSEL_2+TACLR,&TA0CTL                 ; SMCLK, clear TAR
SetupC1     mov.w   #CCIE,&TA0CCTL2                         ; TA0CCR2 interrupt enabled
            mov.w   #50000,&TA0CCR2                         ;
            bis.w   #MC_2,&TA0CTL                           ; Start Timer_A in continuous
            bic.w   #LPM4,0(SP)                             ; Exit LPM4 on reti
            reti                                            ; Return from overflow ISR
                                                            ;
TACCR2_ISR  add.w   #50000,&TA0CCR2                         ; Offset until next interrupt
            bic.w   #CPUOFF,0(SP)                           ; CPU active on reti
            reti                                            ; Return from TA0CCR1 ISR
                                                            ;
;-------------------------------------------------------------------------------
;            Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   RESET_VECTOR                            ; MSP430 RESET Vector
            .short  RESET                                   ;
            .sect   TIMER0_A1_VECTOR                        ; Timer_A0 Vector
            .short  TIMER0_A1_ISR
            .end
