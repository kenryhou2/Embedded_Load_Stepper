/*
 * myGPIO.h
 *
 */

#ifndef MYGPIO_H_
#define MYGPIO_H_

// Defines
#define LED1_PORT       GPIO_PORT_P1                                            // LED 1 (red)
#define LED1_PIN        GPIO_PIN0

#define LED2_PORT       GPIO_PORT_P1                                            // LED 2 (green)
#define LED2_PIN        GPIO_PIN1

#define BUTTON1_PORT    GPIO_PORT_P5                                            // Pushbutton 1 (S1)
#define BUTTON1_PIN     GPIO_PIN6
#define BUTTON1_VECTOR  PORT5_VECTOR

#define BUTTON2_PORT    GPIO_PORT_P5                                            // Pushbutton 2 (S2)
#define BUTTON2_PIN     GPIO_PIN5
#define BUTTON2_VECTOR  PORT5_VECTOR


// Prototypes
void initGPIO(void);


#endif /* MYGPIO_H_ */

