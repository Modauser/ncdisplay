/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#ifndef USB_DEVICE_MAIN_H
#define USB_DEVICE_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "usbhc.h"

extern struct usbhc_driver USB_HOST_CORE_INSTANCE_inst;

void USB_HOST_CORE_INSTANCE_example(void);

#include "mschf.h"

extern struct mschf USB_HOST_MSC_0_inst;

/**
 * \brief Example of using USB Host MSC.
 * - Select LUN 0
 * - Do read/write test on LUN 0 (will destroy data on the connected USB key)
 * \param[out] backup_buf Pointer to 32-bit aligned buffer to backup one sector data
 * \param[out] rw_buf     Pointer to 32-bit aligned buffer to do write-verify-read test
 */
void USB_HOST_MSC_0_example(uint8_t *backup_buf, uint8_t *rw_buf);

/**
 * \berif Initialize USB
 */
void usb_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USB_DEVICE_MAIN_H
