/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_sync.h>
#include <spi_lite.h>

#include <hal_mci_sync.h>

#include <hal_delay.h>

#include <hal_usb_host.h>

#include <hal_flash.h>

extern struct usart_sync_descriptor USART_0;

extern struct mci_sync_desc IO_BUS;

extern struct usb_h_desc USB_HOST_INSTANCE_inst;

extern struct flash_descriptor FLASH_0;

void USART_0_PORT_init(void);
void USART_0_CLOCK_init(void);
void USART_0_init(void);

void   SPI_0_PORT_init(void);
void   SPI_0_CLOCK_init(void);
int8_t SPI_0_init(void);

void IO_BUS_PORT_init(void);
void IO_BUS_CLOCK_init(void);
void IO_BUS_init(void);

void delay_driver_init(void);

void USB_HOST_INSTANCE_CLOCK_init(void);
void USB_HOST_INSTANCE_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
