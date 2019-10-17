/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME54/SAMD51 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13



#define PA08 GPIO(GPIO_PORTA, 8)
#define PA09 GPIO(GPIO_PORTA, 9)
#define PA10 GPIO(GPIO_PORTA, 10)
#define PA11 GPIO(GPIO_PORTA, 11)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define PA19 GPIO(GPIO_PORTA, 19)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define PB02 GPIO(GPIO_PORTB, 2)
#define PB03 GPIO(GPIO_PORTB, 3)
#define PB10 GPIO(GPIO_PORTB, 10)
#define PB11 GPIO(GPIO_PORTB, 11)

#define SPI_CS_N GPIO(GPIO_PORTA, 18)
#define SPI_PD_N GPIO(GPIO_PORTA, 20)
#define SPI_INT_N GPIO(GPIO_PORTA, 21)

#define ESP32_CS GPIO(GPIO_PORTA, 15)
#define ESP32_HS GPIO(GPIO_PORTA, 14)

#endif // ATMEL_START_PINS_H_INCLUDED
