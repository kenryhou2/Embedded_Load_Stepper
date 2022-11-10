lab_06c_timerHal

This lab is a variation on the lab_06c_timerDirectDriveLed project. The goal
here was to create a set of files (myTimer.c, myGpio.c, main.c) that were
(mostly) consistent between the processors ('F5529, FR4133, 'FR5994, 'FR6989,
and MSP432P401). This was done by defining a set of hardware abstractions in 
the file "hal.h".

Whenever you compile for one processor vs another, the correct HAL values are
used. While this idea could be extended further (and more completely), we 
hope this gives you a chance to investigate the HAL concept, just in case 
something like this is useful for your end application.

As we have provided this project already complete, no code is found in this
readme file.

