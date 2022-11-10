/*
 * myClocks.h
 *
 */

#ifndef MYCLOCKS_H_
#define MYCLOCKS_H_

//***** Prototypes ************************************************************
void initClocks(void);

//***** Defines ***************************************************************
#define XT1_CRYSTAL_FREQUENCY_IN_HZ    32768
#define REFOCLK_FREQUENCY              32768
#define MCLK_DESIRED_FREQUENCY_IN_KHZ  8000

#define myMCLK_FREQUENCY_IN_HZ         8000000
#define mySMCLK_FREQUENCY_IN_HZ        4000000
#define myACLK_FREQUENCY_IN_HZ         32768


#endif /* MYCLOCKS_H_ */

