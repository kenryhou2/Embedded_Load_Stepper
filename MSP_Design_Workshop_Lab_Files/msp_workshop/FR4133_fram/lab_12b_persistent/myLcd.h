/*
 * myLcd.h
 *
 */

//***** Header Files **********************************************************
#include <msp430fr4133.h>

#ifndef MYLCD_H_
#define MYLCD_H_

//***** Prototypes ************************************************************
void myLCD_init(void);
void myLCD_showChar( char, int );
void myLCD_displayNumber( unsigned long );
void myLCD_displayString( char s[6] );
int  myLCD_showSymbol( int, int, int );


//***** Defines ***************************************************************
#define LCD_NUM_CHAR                6                                             // Number of character positions in the display

/* LCD Symbol Bits */
#define LCD_TIMER_IDX               LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_TIMER_COM               BIT3

#define LCD_HEART_IDX               LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_HEART_COM               BIT2

#define LCD_REC_IDX                 LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_REC_COM                 BIT1

#define LCD_EXCLAMATION_IDX         LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_EXCLAMATION_COM         BIT0

#define LCD_BRACKETS_IDX            LCD_E_MEMORY_BLINKINGMEMORY_13
#define LCD_BRACKETS_COM            BIT0

#define LCD_BATT_IDX                LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_BATT_COM                BIT4

#define LCD_B1_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_13
#define LCD_B1_COM                  BIT1

#define LCD_B2_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_B2_COM                  BIT5

#define LCD_B3_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_13
#define LCD_B3_COM                  BIT2

#define LCD_B4_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_B4_COM                  BIT6

#define LCD_B5_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_13
#define LCD_B5_COM                  BIT3

#define LCD_B6_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_12
#define LCD_B6_COM                  BIT7

#define LCD_ANT_IDX                 LCD_E_MEMORY_BLINKINGMEMORY_9
#define LCD_ANT_COM                 BIT2

#define LCD_TX_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_19
#define LCD_TX_COM                  BIT2

#define LCD_RX_IDX                  LCD_E_MEMORY_BLINKINGMEMORY_19
#define LCD_RX_COM                  BIT0

#define LCD_NEG_IDX                 LCD_E_MEMORY_BLINKINGMEMORY_5
#define LCD_NEG_COM                 BIT2

#define LCD_DEG_IDX                 LCD_E_MEMORY_BLINKINGMEMORY_3
#define LCD_DEG_COM                 BIT2

#define LCD_A1DP_IDX                LCD_E_MEMORY_BLINKINGMEMORY_5
#define LCD_A1DP_COM                BIT0

#define LCD_A2DP_IDX                LCD_E_MEMORY_BLINKINGMEMORY_7
#define LCD_A2DP_COM                BIT0

#define LCD_A3DP_IDX                LCD_E_MEMORY_BLINKINGMEMORY_9
#define LCD_A3DP_COM                BIT0

#define LCD_A4DP_IDX                LCD_E_MEMORY_BLINKINGMEMORY_11
#define LCD_A4DP_COM                BIT0

#define LCD_A5DP_IDX                LCD_E_MEMORY_BLINKINGMEMORY_3
#define LCD_A5DP_COM                BIT0

#define LCD_A2COL_IDX               LCD_E_MEMORY_BLINKINGMEMORY_7
#define LCD_A2COL_COM               BIT2

#define LCD_A4COL_IDX               LCD_E_MEMORY_BLINKINGMEMORY_11
#define LCD_A4COL_COM               BIT2

//***** myLCD_showSymbol() function operations
#define LCD_UPDATE                  0
#define LCD_CLEAR                   1
#define LCD_TOGGLE                  2
#define LCD_GET                     3

//***** myLCD_showSymbol() function memory enums
#define LCD_MEMORY_MAIN             0
#define LCD_MEMORY_BLINKING         1

//***** myLCD_showSymbol() function icons (for 'FR6989 LaunchPad)
#define LCD_TMR                     0
#define LCD_HRT                     1
#define LCD_REC                     2
#define LCD_EXCLAMATION             3
#define LCD_BRACKETS                4
#define LCD_BATT                    5
#define LCD_B1                      6
#define LCD_B2                      7
#define LCD_B3                      8
#define LCD_B4                      9
#define LCD_B5                      10
#define LCD_B6                      11
#define LCD_ANT                     12
#define LCD_TX                      13
#define LCD_RX                      14
#define LCD_NEG                     15
#define LCD_DEG                     16
#define LCD_A1DP                    17
#define LCD_A2DP                    18
#define LCD_A3DP                    19
#define LCD_A4DP                    20
#define LCD_A5DP                    21
#define LCD_A2COL                   22
#define LCD_A4COL                   23

//***** Global Variables ******************************************************
extern const char digit[10][2];                                                 // Segment values for each number character
extern const char alphabetBig[26][2];                                           // Segment values for each alphabet character


#endif /* MYLCD_H_ */
