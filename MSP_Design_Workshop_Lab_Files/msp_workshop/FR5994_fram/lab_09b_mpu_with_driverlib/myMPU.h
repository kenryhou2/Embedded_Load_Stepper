/*
 * myMPU.h
 *
 */

#ifndef MYMPU_H_
#define MYMPU_H_

//***** Prototypes ************************************************************
void initMPU(void);
void violateMemory( int16_t vLocation );

#define NUMBER_OF_MEMORY_VIOLATION_TESTS  4

#endif /* MYMPU_H_ */
