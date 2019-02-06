/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include "usb_start.h"

/** USB Host Core driver instance */
struct usbhc_driver USB_HOST_CORE_INSTANCE_inst;

/** Ctrl requests process buffer size */
#define CTRL_BUFFER_SIZE CONF_USBH_CTRL_BUF_SIZE
/** Ctrl requests process buffer for enumeration */
static uint32_t USB_HOST_CORE_INSTANCE_ctrl_buf[CTRL_BUFFER_SIZE / 4];

/** Flag to indicate request done */
static volatile bool USB_HOST_CORE_INSTANCE_req_done;

/**
 * Enumeration callback to handle device state changes
 */
static void USB_HOST_CORE_INSTANCE_enum_cb(struct usbhc_driver *c, int32_t status, void *param)
{
	if (status == USBHC_ENUM_DISCONNECTED) {
		/* A device is disconnected,
		 * param is pointer to device instance */
		(void)USBHD_PTR(param);
	} else if (status == USBHC_ENUM_SUCCESS) {
		/* A device is connected and enumeration OK,
		 * param is pointer to device instance */
		(void)USBHD_PTR(param);
	}
	(void)c;
}

/**
 * Request end callback invoked when request is done
 */
static void USB_HOST_CORE_INSTANCE_req_cb(struct usbhd_driver *d, struct usb_h_pipe *pipe)
{
	USB_HOST_CORE_INSTANCE_req_done = true;
}

/** Enumeration state change handler */
static struct usbhc_handler USB_HOST_CORE_INSTANCE_enum_h = {NULL, (FUNC_PTR)USB_HOST_CORE_INSTANCE_enum_cb};
/** Request done handler */
static struct usbhc_handler USB_HOST_CORE_INSTANCE_req_h = {NULL, (FUNC_PTR)USB_HOST_CORE_INSTANCE_req_cb};

/**
 * Example of using USB Host Core.
 */
void USB_HOST_CORE_INSTANCE_example(void)
{
	usbhc_register_handler(&USB_HOST_CORE_INSTANCE_inst, USBHC_HDL_ENUM, &USB_HOST_CORE_INSTANCE_enum_h);
	usbhc_register_handler(&USB_HOST_CORE_INSTANCE_inst, USBHC_HDL_REQ, &USB_HOST_CORE_INSTANCE_req_h);
	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	while (1) {
		if (!usbhc_is_dev_usable(USBHD_PTR(&USB_HOST_CORE_INSTANCE_inst))) {
			/* Wait until device connected */
			continue;
		}
		/* Request device descriptor */
		USB_HOST_CORE_INSTANCE_req_done = false;
		while (ERR_BUSY == usbhc_get_dev_desc(USBHD_PTR(&USB_HOST_CORE_INSTANCE_inst)))
			;
		while (!USB_HOST_CORE_INSTANCE_req_done)
			;
		/* Now request is done.
		 *
		 * Device descriptor is at offset USBHC_CTRL_DATA_OFFSET of Ctrl buffer.
		 *
		 * Note that if there are multiple device sharing the control buffer
		 * the data could be changed since the other device may use the buffer.
		 * In that case, create a copy of descriptor in request done callback if
		 * it should be used somewhere.
		 */

		/* Keep waiting until device detach */
		while (usbhc_is_dev_usable(USBHD_PTR(&USB_HOST_CORE_INSTANCE_inst)))
			;
	}
}

/** USB Host MSC function driver instance */
struct mschf USB_HOST_MSC_0_inst;

/** Operation end */
volatile static bool USB_HOST_MSC_0_operation_end = false;

static void USB_HOST_MSC_0_operation_cb(struct mschf *msc, uint8_t lun, int32_t status)
{
	/* Handle operation end */
	USB_HOST_MSC_0_operation_end = true;
}

void USB_HOST_MSC_0_example(uint8_t *backup_buf, uint8_t *rw_buf)
{
	volatile bool tested = false;
	uint32_t      i;
	int32_t       rc;

	usbhc_start(&USB_HOST_CORE_INSTANCE_inst);
	while (1) {
		if (!mschf_is_enabled(&USB_HOST_MSC_0_inst)) {
			/* MSC device disconnected */
			tested = false;
			continue;
		}
		/* MSC device connected */
		if (tested) {
			continue;
		}
		tested = true;

		/* Select the LUN 0 */
		USB_HOST_MSC_0_operation_end = false;
		rc                           = mschf_select_lun(&USB_HOST_MSC_0_inst, 0, false, USB_HOST_MSC_0_operation_cb);
		if (rc == ERR_NOT_READY) {
			/* Wait until selection end */
			while (!USB_HOST_MSC_0_operation_end)
				;
		} else if (rc != ERR_NONE) {
			/* Select LUN fail! */
			continue;
		}
		/* Read LUN test */
		USB_HOST_MSC_0_operation_end = false;
		rc = mschf_read(&USB_HOST_MSC_0_inst, 0, (uint8_t *)backup_buf, 1, USB_HOST_MSC_0_operation_cb);
		if (rc != ERR_NONE) {
			/* Fail! */
			continue;
		}
		/* Wait until read end */
		while (!USB_HOST_MSC_0_operation_end)
			;

/* Data may be destroied if following code enabled !!! */
#if !CONF_MSCHF_READ_ONLY
		/* Write LUN test */
		for (i = 0; i < 512 / 4; i++) {
			rw_buf[i] = i;
		}
		USB_HOST_MSC_0_operation_end = false;
		rc = mschf_write(&USB_HOST_MSC_0_inst, 0, (uint8_t *)rw_buf, 1, USB_HOST_MSC_0_operation_cb);
		if (rc != ERR_NONE) {
			/* Fail! */
			continue;
		}
		/* Wait until write end */
		while (!USB_HOST_MSC_0_operation_end)
			;
		/* Read back and verify */
		for (i = 0; i < 512 / 4; i++) {
			rw_buf[i] = 0;
		}
		USB_HOST_MSC_0_operation_end = false;
		rc = mschf_read(&USB_HOST_MSC_0_inst, 0, (uint8_t *)rw_buf, 1, USB_HOST_MSC_0_operation_cb);
		if (rc != ERR_NONE) {
			/* Fail! */
			continue;
		}
		/* Wait until read end */
		while (!USB_HOST_MSC_0_operation_end)
			;
		/* Check data */
		for (i = 0; i < 512 / 4; i++) {
			if (rw_buf[i] != i) {
				/* Fail! */
				continue;
			}
		}
		/* Restore backup data
		 * If the program goes here, the read/write/verify tests are good.
		 */
		USB_HOST_MSC_0_operation_end = false;
		rc = mschf_write(&USB_HOST_MSC_0_inst, 0, (uint8_t *)backup_buf, 1, USB_HOST_MSC_0_operation_cb);
		if (rc != ERR_NONE) {
			/* Fail! */
			continue;
		}
		/* Wait until restore end */
		while (!USB_HOST_MSC_0_operation_end)
			;
#else
		(void)rw_buf;
#endif /* !CONF_MSCHF_READ_ONLY */
	}
}

void usb_init(void)
{

	usbhc_init(&USB_HOST_CORE_INSTANCE_inst,
	           &USB_HOST_INSTANCE_inst,
	           (uint8_t *)USB_HOST_CORE_INSTANCE_ctrl_buf,
	           CTRL_BUFFER_SIZE);

	mschf_init(&USB_HOST_CORE_INSTANCE_inst, &USB_HOST_MSC_0_inst);
}
