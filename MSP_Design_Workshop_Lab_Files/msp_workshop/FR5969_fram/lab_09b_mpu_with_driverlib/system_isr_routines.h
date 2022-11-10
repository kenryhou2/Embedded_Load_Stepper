/*
 * system_isr_routines.h
 *
 */

#ifndef SYSTEM_ISR_ROUTINES_H_
#define SYSTEM_ISR_ROUTINES_H_


//***** Prototypes ************************************************************
void RESET_checkLastCondition( void );
void RESET_handleEvent( uint16_t );


//***** Global Externs ********************************************************
extern uint16_t SYSNMIflag[ 6 ];


//***** Definitions ***********************************************************
#define SYSNMIflag_INFO_DUMMY   0    //The program writes to this location during violation test
#define SYSNMIflag_INFO         1
#define SYSNMIflag_SEG1         2
#define SYSNMIflag_SEG2         3
#define SYSNMIflag_SEG3         4
#define SYSNMIflag_RST_EVENT    5



#endif /* SYSTEM_ISR_ROUTINES_H_ */
