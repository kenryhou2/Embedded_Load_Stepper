/*
 * myClocks.h
 *
 */

#ifndef MYCLOCKS_H_
#define MYCLOCKS_H_

//***** Prototypes ************************************************************
void initClocks(void);

//***** Defines ***************************************************************
#define LF_CRYSTAL_FREQUENCY_IN_HZ     32768                                    // 32KHz - Freq of external crystal
#define HF_CRYSTAL_FREQUENCY_IN_HZ     4000000                                  // 4MHz  - Freq of external crystal

// The following oscillator and clock frequencies are set in the associated 'C' file
#define myMCLK_FREQUENCY_IN_HZ         8000000
#define mySMCLK_FREQUENCY_IN_HZ        8000000
#define myACLK_FREQUENCY_IN_HZ         32768


#endif /* MYCLOCKS_H_ */

