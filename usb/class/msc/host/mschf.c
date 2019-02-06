/**
 * \file
 *
 * \brief USB Host Stack Mass Storage Bulk Only Transfer Function Definition.
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 */

#include <mschf.h>
#include <stdio.h>
#include <string.h>

/** \internal
 * \brief Scan LUN cache to find specific LUN
 * \param msc Pointer to MSC instance
 * \param lun LUN number to find
 * \return Index of LUN in cache, <0 if not found
 */
static int8_t mschf_find_lun_cache(struct mschf *msc, uint8_t lun);

/** \internal
 * \brief Start of the scsi INQUIRY command
 * \param msc Pointer to MSC instance
 * \return \c true if OK, else \c false
 */
static bool mschf_scsi_inquiry(struct mschf *msc);

/** \internal
 * \brief Start of the scsi READ_CAPACITY command
 * \param msc Pointer to MSC instance
 * \return \c true if OK, else \c false
 */
static bool mschf_scsi_read_capacity(struct mschf *msc);

#if !CONF_MSCHF_READ_ONLY
/** \internal
 * \brief Start of the scsi MODE_SENSE6 command
 * \param msc Pointer to MSC instance
 * \return \c true if OK, else \c false
 */
static bool mschf_scsi_mode_sense6(struct mschf *msc);
#endif

/** \internal
 * \brief Start of the scsi TEST_UNIT_READY command
 * \param msc Pointer to MSC instance
 * \return \c true if OK, else \c false
 */
static bool mschf_scsi_test_unit_ready(struct mschf *msc);

/** \internal
 * \brief Start of the scsi REQUEST_SENSE command
 * \param msc Pointer to MSC instance
 * \return \c true if OK, else \c false
 */
static bool mschf_scsi_request_sense(struct mschf *msc);

/** \internal
 *  \brief Pipe callback invoked on CBW sent
 */
static void mschf_cbw_done(struct usb_h_pipe *pipe);
/** \internal
 *  \brief Pipe callback invoked on DATA sent/received
 */
static void mschf_data_done(struct usb_h_pipe *pipe);
/** \internal
 *  \brief Pipe callback invoked on CSW received
 */
static void mschf_csw_done(struct usb_h_pipe *pipe);

/** \internal
 *  \brief Start CSW phase
 *  \param msc Pointer to MSC driver instance
 *  \return \c true if transfer started OK
 */
static inline bool mschf_csw_start(struct mschf *msc);

/** \internal
 *  \brief Issue Bulk-only reset
 *  \param msc Pointer to MSC driver instance
 */
static void mschf_bulk_reset(struct mschf *msc);

/** \internal
 *  \brief Issue ClearEndpointHalt
 *  \param msc Pointer to MSC driver instance
 */
static void mschf_clear_halt(struct mschf *msc);

/**
 * \brief MSC operation end
 *
 * Will modify related LUN status
 *
 * \param msc    Pointer to MSC driver instance
 * \param status Status code:
 *               - \ref ERR_INVALID_DATA: Sense data not handled
 *               - \ref ERR_IO: USB transfer error
 *               - \ref ERR_NOT_FOUND: Media not present
 *               - \ref ERR_NOT_READY: Media changing
 *               - \ref ERR_DENIED: Media protected
 *               - \ref ERR_PROTOCOL: Phase error
 */
static void mschf_op_end(struct mschf *msc, int32_t status);

/** \brief Do post enumeration operation on function
 *  \param msc Pointer to Mass Storage function driver
 */
static inline void mschf_post_enum(struct mschf *msc);

/** \internal
 *  \brief Handle requests
 * - GetMaxLUN
 * - Bulk-only reset
 * - ClearEndpointHalt
 */
static void mschf_request_done(struct usbhd_driver *dev, struct usb_h_pipe *pipe);

/** \internal
 *  \brief Request handler instance
 */
struct usbhc_handler mschf_req_hdl = {NULL, (FUNC_PTR)mschf_request_done};

#if CONF_MSCHF_TIMEOUT
/** \internal
 *  \brief Set timeout
 *  \param msc Pointer to Mass Storage function driver
 *  \param timeout Timeout value to set
 */
static void mschf_set_timeout(struct mschf *msc, const uint32_t timeout)
{
	msc->timeout = timeout;
	if (timeout) {
		usbhc_register_handler(usbhf_get_core(&msc->func), USBHC_HDL_SOF, (struct usbhc_handler *)&msc->sof_hdl);
	}
}

/** \internal
 * \brief Check timeout
 * \param msc Pointer to the MSC instance
 * \return \c true if timeout
 */
static bool mschf_is_timeout(struct mschf *msc)
{
	if (msc->timeout > 0) {
		if (--msc->timeout == 0) {
			mschf_op_end(msc, ERR_TIMEOUT);
			return true;
		}
	}
	return false;
}
#else
/** \internal
 *  \brief Set timeout
 *  Do nothing since timeout not used.
 *  \param msc Pointer to Mass Storage function driver
 *  \param timeout Timeout value to set
 */
#define mschf_set_timeout(msc, timeout)                                                                                \
	do {                                                                                                               \
		(void)msc;                                                                                                     \
		(void)timeout;                                                                                                 \
	} while (0)
/** \internal
 * \brief Check timeout
 * \param msc Pointer to the MSC instance
 * \return always \c false since timeout not used
 */
static inline bool mschf_is_timeout(struct mschf *msc)
{
	(void)msc;
	return false;
}
#endif

/** \internal
 *  \brief SOF callback
 *  \param core Pointer to USB Host Core instance
 *  \param hdl  Pointer to extended handler instance
 */
static void mschf_sof(struct usbhc_driver *core, struct usbhc_handler_ext *hdl)
{
	struct mschf *msc;
	(void)core;
	if (hdl->func != (FUNC_PTR)mschf_sof) {
		return;
	}
	msc = (struct mschf *)hdl->ext;
	mschf_is_timeout(msc);
}

/** \internal
 *  \brief Reset the MSC driver instance
 *  \param msc Pointer to Mass Storage function driver
 */
static void mschf_reset(struct mschf *msc)
{
	uint8_t i;
	memset(&msc->iface, 0, sizeof(struct mschf) - sizeof(struct usbhf_driver));
	msc->rsc_hdl.ext = (void *)msc;
#if CONF_MSCHF_TIMEOUT
	msc->sof_hdl.func = (FUNC_PTR)mschf_sof;
	msc->sof_hdl.ext  = (void *)msc;
#endif
	for (i = 0; i < CONF_MSCHF_N_CACHED_LUN; i++) {
		msc->lun_cache[i].lun = -1;
	}
}

/** \internal
 *  \brief Dummy callback while it's not specified
 */
static void mschf_dummy_cb(struct mschf *msc, uint8_t lun, int32_t status)
{
	(void)msc;
	(void)lun;
	(void)status;
}

static void mschf_op_end(struct mschf *msc, int32_t status)
{
	int8_t lun_i;
	msc->is_busy = false;
#if CONF_MSCHF_TIMEOUT
	usbhc_unregister_handler(usbhf_get_core(msc), USBHC_HDL_SOF, USBHC_HDL_PTR(&msc->sof_hdl));
#endif

	if (!msc->is_enabled) {
		return;
	}
	lun_i = mschf_find_lun_cache(msc, msc->lun);
	if (lun_i >= 0) {
		struct mschf_lun *lun = &msc->lun_cache[lun_i];
		switch (status) {
		case ERR_NONE: /* Clear error/not ready */
			lun->status.byte &= ~(MSCHF_LUN_ERROR | MSCHF_LUN_CHANGING);
			break;
		case ERR_NOT_FOUND: /* Keep removable bit only */
			lun->status.byte &= MSCHF_LUN_REMOVABLE;
			break;
		case ERR_NOT_READY: /* Keep changing bit only */
			lun->status.byte &= MSCHF_LUN_CHANGING | MSCHF_LUN_REMOVABLE;
			break;
		case ERR_DENIED:
			lun->status.bm.is_protected = true;
			break;
		default: /* Set error bit */
			lun->status.bm.is_error = true;
			break;
		}
	}
	msc->cb(msc, msc->lun, status);
}

/** \brief Uninstall the MSC function driver
 *  \param msc Pointer to the MSC function driver instance
 */
static inline void mschf_uninstall(struct mschf *msc)
{
	uint8_t    i;
	bool       change = msc->is_enabled;
	mschf_cb_t cb     = msc->change;
	for (i = 0; i < 2; i++) {
		if (msc->pipe[i]) {
			usb_h_pipe_abort(msc->pipe[i]);
			usb_h_pipe_free(msc->pipe[i]);
		}
	}
	mschf_reset(msc);
	usbhc_unregister_handler(usbhf_get_core(msc), USBHC_HDL_REQ, &mschf_req_hdl);
	usbhc_unregister_handler(usbhf_get_core(msc), USBHC_HDL_RSC, USBHC_HDL_PTR(&msc->rsc_hdl));
#if CONF_MSCHF_TIMEOUT
	usbhc_unregister_handler(usbhf_get_core(msc), USBHC_HDL_SOF, USBHC_HDL_PTR(&msc->sof_hdl));
#endif
	if (change && cb) {
		cb(msc);
	}
}

/** \brief Try to install the MSC driver
 *  \retval ERR_NONE installed
 *  \retval ERR_NOT_FOUND not installed
 *  \retval ERR_NO_CHANGE already taken
 */
static inline int32_t mschf_install(struct mschf *msc, struct usbh_descriptors *desc)
{
	struct usbhd_driver *  dev  = usbhf_get_dev(msc);
	struct usbhc_driver *  core = usbhf_get_core(msc);
	struct usb_h_desc *    hcd  = core->hcd;
	struct usb_iface_desc *piface;
	struct usb_ep_desc *   pep = NULL;
	struct usb_h_pipe *    pipe;
	uint8_t *              pd;
	if (msc->is_installed) {
		/* Driver already in use */
		return ERR_NO_CHANGE;
	}
	/* Find very first interface */
	pd = usb_find_desc(desc->sod, desc->eod, USB_DT_INTERFACE);
	if (!pd) {
		/* No interface found */
		return ERR_NOT_FOUND;
	}
	/* Try to install MSC */
	piface = (struct usb_iface_desc *)pd;
	if (piface->bInterfaceClass != MSC_CLASS || piface->bInterfaceProtocol != MSC_PROTOCOL_BULK
	    || piface->bNumEndpoints < 2) {
		return ERR_NOT_FOUND;
	}
	/* Find endpoints */
	while (1) {
		pd = usb_desc_next(pd);
		pd = usb_find_ep_desc(pd, desc->eod);
		if (NULL == pd) {
			break;
		}
		pep = (struct usb_ep_desc *)pd;
		if ((pep->bmAttributes & USB_EP_TYPE_MASK) == USB_EP_TYPE_BULK) {
			pipe = usb_h_pipe_allocate(hcd,
			                           dev->dev_addr,
			                           pep->bEndpointAddress,
			                           pep->wMaxPacketSize,
			                           pep->bmAttributes,
			                           pep->bInterval,
			                           dev->speed,
			                           true);
			if (pipe == NULL) {
				mschf_uninstall(msc);
				return ERR_NO_RESOURCE;
			}
			pipe->owner                                                                        = (void *)msc;
			msc->pipe[(pep->bEndpointAddress & 0x80) ? MSCHF_IN_PIPE_IDX : MSCHF_OUT_PIPE_IDX] = pipe;
		}
	}
	/* Update descriptors pointers */
	desc->sod = usb_find_iface_after((uint8_t *)piface, desc->eod, piface->bInterfaceNumber);
	/* Update status */
	msc->iface        = piface->bInterfaceNumber;
	msc->is_installed = true;
	usbhc_register_handler(core, USBHC_HDL_REQ, &mschf_req_hdl);
	/* Post enumeration operation start when
	 * enumeration callback is invoked */
	return ERR_NONE;
}

/** \brief Callback invoked on install/uninstall the function driver
 *  \param func  Pointer to the function driver instance
 *  \param ctrl  Control operation code
 *  \param param Parameter for install/uninstall
 */
static int32_t mschf_ctrl(struct usbhf_driver *func, enum usbhf_control ctrl, void *param)
{
	switch (ctrl) {
	case USBHF_INSTALL:
		return mschf_install(MSCHF_PTR(func), (struct usbh_descriptors *)param);
	case USBHF_UNINSTALL:
		mschf_uninstall(MSCHF_PTR(func));
		break;
	case USBHF_ENABLE:
		mschf_post_enum(MSCHF_PTR(func));
		break;
	default:
		return ERR_INVALID_ARG;
	}
	return ERR_NONE;
}

/** \internal
 *  \brief Try to start post enumeration
 *  \param core Pointer to core instance
 *  \param hdl  Pointer to extended handler instance for
 *              resource free notification
 */
static void mschf_post_enum_start(struct usbhc_driver *core, struct usbhc_handler_ext *hdl)
{
	struct usb_req *req  = (struct usb_req *)core->ctrl_buf;
	uint8_t *       data = core->ctrl_buf + USBHC_CTRL_DATA_OFFSET;
	struct mschf *  msc  = (struct mschf *)hdl->ext;

	if (hdl->func != (FUNC_PTR)mschf_post_enum_start) {
		return;
	}
	if (usbhd_take_control(msc->func.pdev, NULL) != ERR_NONE) {
		return;
	}
	usbhc_unregister_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);
	/* Take the control, issue GetMaxLUN */
	usb_fill_GetMaxLUN_req(req, (uint8_t)msc->iface);
	if (usbhc_request(core, (uint8_t *)req, data) != ERR_NONE) {
		usbhc_release_control(core);
	}
}

static inline void mschf_post_enum(struct mschf *msc)
{
	struct usbhc_driver *core = usbhf_get_core(&msc->func);
	if (msc->func.ctrl != (usbhf_control_cb_t)mschf_ctrl) {
		return;
	}
	if (msc->is_enabled) {
		return;
	}
	/* Register wait control resource to start post enumeration process */
	msc->rsc_hdl.func = (FUNC_PTR)mschf_post_enum_start;
	usbhc_register_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);
}

/** \internal
 * \brief GetMaxLUN done
 * On error: stop post enumeration as fail.
 * On STALL: max LUN is 0.
 * On no error: max LUN is from returned data.
 * Start inquiry on bulk pipes if max LUN is saved.
 */
static void mschf_get_max_lun_done(struct mschf *msc, struct usb_h_pipe *pipe)
{
	if (pipe->x.ctrl.status == USB_H_STALL) {
		msc->max_lun = 0;
	} else if (pipe->x.ctrl.status == USB_H_OK) {
		msc->max_lun = *pipe->x.ctrl.data;
	} else {
		/* IO error, driver not enabled */
		msc->error = true;
		return;
	}
	/* Issue inquiry from LUN #0 */
	mschf_scsi_inquiry(msc);
}

/** \internal
 * \brief Bulk-only reset done
 * On error: stop current process as fail.
 * Start clear halts on bulk pipes if no error.
 */
static void mschf_bulk_reset_done(struct mschf *msc, struct usb_h_pipe *pipe)
{
	if (pipe->x.ctrl.status != USB_H_OK) {
		/* error */
		mschf_op_end(msc, ERR_PROTOCOL);
		return;
	}
	msc->clr_in_halt  = true;
	msc->clr_out_halt = true;
	/* Issue Clear HALT */
	mschf_clear_halt(msc);
}

/** \internal
 * \brief Issue bulk-only reset
 * \return \c true if request issued or error
 */
static bool mschf_bulk_reset_ex(struct usbhc_driver *core, struct mschf *msc)
{
	struct usb_req *req = (struct usb_req *)core->ctrl_buf;

	if (usbhd_take_control(msc->func.pdev, NULL) != ERR_NONE) {
		return false;
	}
	usbhc_unregister_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);
	/* Take the control, issue BulkOnlyReset */
	usb_fill_BOMSReset_req(req, (uint8_t)msc->iface);
	if (usbhc_request(core, (uint8_t *)req, NULL) != ERR_NONE) {
		usbhc_release_control(core);
		mschf_op_end(msc, ERR_IO);
		return true;
	}
	return true;
}

/** \internal
 *  \brief Try to start bulk only reset request
 *  \param core Pointer to core instance
 *  \param hdl  Pointer to extended handler instance for
 *              resource free notification
 */
static void mschf_bulk_reset_start(struct usbhc_driver *core, struct usbhc_handler_ext *hdl)
{
	struct mschf *msc = (struct mschf *)hdl->ext;
	if (hdl->func != (FUNC_PTR)mschf_bulk_reset_start) {
		return;
	}
	mschf_bulk_reset_ex(core, msc);
}

static void mschf_bulk_reset(struct mschf *msc)
{
	struct usbhc_driver *core = usbhf_get_core(&msc->func);
	if (!mschf_bulk_reset_ex(core, msc)) {
		msc->rsc_hdl.func = (FUNC_PTR)mschf_bulk_reset_start;
		usbhc_register_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);
	}
}

static void mschf_clear_halt_done(struct mschf *msc, struct usb_h_pipe *pipe)
{
	struct usbhc_driver *core = usbhf_get_core(&msc->func);
	struct usb_req *     req  = (struct usb_req *)core->ctrl_buf;
	(void)pipe;
	if (req->wIndex == msc->pipe[MSCHF_IN_PIPE_IDX]->ep) {
		msc->clr_in_halt = false;
		/* Read CSW if not phase error (reset recovery) */
		if (!msc->clr_out_halt) {
			msc->retry = true;
			mschf_csw_start(msc);
		}
	} else {
		msc->clr_out_halt   = false;
		msc->reset_recovery = false;
		mschf_op_end(msc, ERR_IO);
	}
}

/** \internal
 * \brief Issue ClearFeature - endpoint halt
 * \return Operation status
 * \retval ERR_NO_CHANGE No endpoint halts, no request issued
 * \retval ERR_NO_RESOURCE Control resource busy
 * \retval ERR_IO IO error, operation terminated
 * \retval ERR_NONE request issued
 */
static int32_t mschf_clear_halt_ex(struct usbhc_driver *core, struct mschf *msc)
{
	struct usb_req *req = (struct usb_req *)core->ctrl_buf;
	uint8_t *       buf = core->ctrl_buf + USBHC_CTRL_DATA_OFFSET;
	if (msc->clr_in_halt) {
		req->wIndex = msc->pipe[MSCHF_IN_PIPE_IDX]->ep;
	} else if (msc->clr_out_halt) {
		req->wIndex = msc->pipe[MSCHF_OUT_PIPE_IDX]->ep;
	} else {
		return ERR_NO_CHANGE;
	}

	if (usbhd_take_control(msc->func.pdev, NULL) != ERR_NONE) {
		return ERR_NO_RESOURCE;
	}
	usbhc_unregister_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);

	req->bmRequestType = USB_REQT_RECIP_ENDPOINT | USB_REQT_TYPE_STANDARD | USB_REQT_DIR_OUT;
	req->bRequest      = USB_REQ_CLEAR_FTR;
	req->wValue        = USB_EP_FTR_HALT;
	req->wLength       = 0;
	if (usbhc_request(core, (uint8_t *)req, buf) != ERR_NONE) {
		usbhc_release_control(core);
		mschf_op_end(msc, ERR_IO);
		return ERR_IO;
	}
	return ERR_NONE;
}

/** \internal
 *  \brief Try to start clear halt request
 *  \param core Pointer to core instance
 *  \param hdl  Pointer to extended handler instance for
 *              resource free notification
 */
static void mschf_clear_halt_start(struct usbhc_driver *core, struct usbhc_handler_ext *hdl)
{
	struct mschf *msc = (struct mschf *)hdl->ext;
	if (hdl->func != (FUNC_PTR)mschf_clear_halt_start) {
		return;
	}
	mschf_clear_halt_ex(core, msc);
}

static void mschf_clear_halt(struct mschf *msc)
{
	int32_t              rc;
	struct usbhc_driver *core = usbhf_get_core(&msc->func);
	rc                        = mschf_clear_halt_ex(core, msc);
	if (rc == ERR_NO_RESOURCE) {
		msc->rsc_hdl.func = (FUNC_PTR)mschf_clear_halt_start;
		usbhc_register_handler(core, USBHC_HDL_RSC, (struct usbhc_handler *)&msc->rsc_hdl);
	}
}

static void mschf_request_done(struct usbhd_driver *dev, struct usb_h_pipe *pipe)
{
	struct usb_req *req = (struct usb_req *)pipe->x.ctrl.setup;
	struct mschf *  msc = MSCHF_PTR(dev->func.pfunc);
	if (!dev->status.bm.usable) {
		return;
	}
	if (req->bRequest != USB_REQ_MSC_GET_MAX_LUN && req->bRequest != USB_REQ_MSC_BULK_RESET
	    && req->bRequest != USB_REQ_CLEAR_FTR) {
		return;
	}
	while (msc) {
		/* Find function that issues the request */
		if (msc->func.ctrl != mschf_ctrl) {
			msc = MSCHF_PTR(msc->func.next.pfunc);
			continue;
		}
		if (req->bRequest == USB_REQ_CLEAR_FTR) {
			if (req->wIndex != msc->pipe[0]->ep && req->wIndex != msc->pipe[0]->ep) {
				msc = MSCHF_PTR(msc->func.next.pfunc);
				continue;
			}
			mschf_clear_halt_done(msc, pipe);
			return;
		}

		if (msc->iface != req->wIndex) {
			msc = MSCHF_PTR(msc->func.next.pfunc);
			continue;
		}
		if (req->bRequest == USB_REQ_MSC_GET_MAX_LUN) {
			mschf_get_max_lun_done(msc, pipe);
			return;
		} else { /* USB_REQ_MSC_BULK_RESET */
			mschf_bulk_reset_done(msc, pipe);
			return;
		}
	}
}

/* -------------------------------------- CBW / DATA / CSW ----------------- */

/** \internal
 *  \brief Start CBW phase
 *  \param msc Pointer to MSC driver instance
 *  \return \c true if transfer started OK
 */
static inline bool mschf_cbw_start(struct mschf *msc)
{
	usb_h_pipe_register_callback(msc->pipe[MSCHF_OUT_PIPE_IDX], mschf_cbw_done);
	return USB_H_OK
	       == usb_h_bulk_int_iso_xfer(
	              msc->pipe[MSCHF_OUT_PIPE_IDX], (uint8_t *)&msc->mem.cbw, sizeof(msc->mem.cbw), false);
}

/** \internal
 *  \brief Start CSW phase
 *  \param msc Pointer to MSC driver instance
 *  \return \c true if transfer started OK
 */
static inline bool mschf_csw_start(struct mschf *msc)
{
	usb_h_pipe_register_callback(msc->pipe[MSCHF_IN_PIPE_IDX], mschf_csw_done);
	return USB_H_OK
	       == usb_h_bulk_int_iso_xfer(
	              msc->pipe[MSCHF_IN_PIPE_IDX], (uint8_t *)&msc->mem.resp.csw, sizeof(msc->mem.resp.csw), false);
}

/** \internal
 *  \brief Start Data phase
 *  \param msc Pointer to MSC driver instance
 *  \return \c true if transfer started OK
 */
static inline bool mschf_data_start(struct mschf *msc, const uint8_t pipe_idx)
{
	usb_h_pipe_register_callback(msc->pipe[pipe_idx], mschf_data_done);
	return USB_H_OK == usb_h_bulk_int_iso_xfer(msc->pipe[pipe_idx], msc->cmd.buf, msc->cmd.size, false);
}

/** \internal
 *  \brief Checks the sense data
 */
static void mschf_scsi_sense_done(struct mschf *msc, int32_t status)
{
	struct scsi_request_sense_data *sense = &msc->mem.resp.data.sense;
	int32_t                         rc    = ERR_NONE;

	if (ERR_NONE != status) {
		mschf_op_end(msc, ERR_IO);
		return;
	}

	/* Decode data sense */
	if ((sense->valid_reponse_code & SCSI_SENSE_RESPONSE_CODE_MASK) == SCSI_SENSE_CURRENT) {
		/* Valid code */
		if (sense->sense_flag_key == SCSI_SK_NO_SENSE) {
			rc = ERR_NONE;
		} else if (sense->sense_flag_key == SCSI_SK_NOT_READY && sense->AddSense == BE16(SCSI_ASC_MEDIUM_NOT_PRESENT)) {
			rc = ERR_NOT_FOUND;
		} else if (sense->sense_flag_key == SCSI_SK_UNIT_ATTENTION
		           && sense->AddSense == BE16(SCSI_ASC_NOT_READY_TO_READY_CHANGE)) {
			rc = ERR_NOT_READY;
		} else if (sense->sense_flag_key == SCSI_SK_DATA_PROTECT) {
			rc = ERR_DENIED;
		} else {
			/* Unexpected sense data */
			rc = ERR_INVALID_DATA;
		}
	} else {
		/* Invalid sense data */
		rc = ERR_INVALID_DATA;
	}
	msc->cmd.sense_cb(msc, rc);
}

/** \internal
 * \brief Checks the CSW packet transfer
 * - Stall again: start reset recovery
 * - Stall once: start clear halt IN
 * - Error: command end with error
 * - Data error: command end with error
 * - Phase error: start reset recovery
 * - No error: command handling with good status
 */
static void mschf_csw_done(struct usb_h_pipe *pipe)
{
	struct mschf *msc = MSCHF_PTR(pipe->owner);

	/* Check DATA transfer result */
	if (pipe->x.general.status == USB_H_STALL) {
		/* Stall handling */
		if (msc->retry) {
			msc->reset_recovery = true;
			mschf_bulk_reset(msc);
		} else {
			msc->clr_in_halt = true;
			mschf_clear_halt(msc);
		}
		return;
	} else if (pipe->x.general.status != USB_H_OK) {
		/* Command Error handling */
		msc->cmd.cb(msc, ERR_IO);
		return;
	}
	if (pipe->x.bii.count != sizeof(msc->mem.resp.csw) || msc->cmd.tag != msc->mem.resp.csw.dCSWTag
	    || USB_CSW_SIGNATURE != msc->mem.resp.csw.dCSWSignature) {
		/* Command Error handling */
		msc->cmd.cb(msc, ERR_IO);
		return;
	}

	/* CSW process */
	if (msc->mem.resp.csw.bCSWStatus == USB_CSW_STATUS_PE) {
		/* The host shall perform a Reset Recovery when
		 * Phase Error status is returned in the CSW */
		msc->reset_recovery = true;
		mschf_bulk_reset(msc);
	} else if (msc->mem.resp.csw.bCSWStatus == USB_CSW_STATUS_FAIL) {
		/* Start request sense */
		msc->cmd.sense_cb = msc->cmd.cb;
		mschf_scsi_request_sense(msc);
	} else {
		/* Command specific handling for CSW */
		msc->cmd.cb(msc, ERR_NONE);
	}
}

/** \internal
 * \brief Checks the DATA phase transfer and launch the next step
 * The next step can be a CSW packet or a endpoint reset in case of STALL.
 */
void mschf_data_done(struct usb_h_pipe *pipe)
{
	struct mschf *msc = MSCHF_PTR(pipe->owner);

	/* Check DATA transfer result */
	if (pipe->x.general.status == USB_H_STALL) {
		/* Stall handling */
		if (pipe->ep & 0x80) {
			msc->clr_in_halt = true;
		} else {
			msc->clr_out_halt = true;
		}
		mschf_clear_halt(msc);
		return;
	} else if (pipe->x.general.status != USB_H_OK) {
		/* Command Error handling */
		msc->cmd.cb(msc, ERR_IO);
		return;
	}

	/* Start CSW phase */
	mschf_csw_start(msc);
}

/** \internal
 *  \brief CBW done
 *  - STALL: start clear halt
 *  - Size error: start reset recovery
 *  - IO error: Command error handling
 *  - Has data: start data phase
 *  - No data: start CSW phase
 */
void mschf_cbw_done(struct usb_h_pipe *pipe)
{
	struct mschf *msc = MSCHF_PTR(pipe->owner);
	uint8_t       ep;

	/* Check the result of CBW transfer */
	if (pipe->x.general.status == USB_H_STALL) {
		/* Stall handling */
		msc->clr_out_halt = true;
		mschf_clear_halt(msc);
		return;
	} else if (pipe->x.general.status != USB_H_OK) {
		/* Command Error handling */
		msc->cmd.cb(msc, ERR_IO);
		return;
	} else if (pipe->x.bii.count != sizeof(msc->mem.cbw)) {
		/* Error handling */
		msc->reset_recovery = true;
		mschf_bulk_reset(msc);
		return;
	}
	if (!msc->cmd.size) {
		/* Start CSW phase */
		mschf_csw_start(msc);
		return;
	}

	/* Start DATA phase */
	ep = (msc->mem.cbw.bmCBWFlags & USB_CBW_DIRECTION_IN) ? MSCHF_IN_PIPE_IDX : MSCHF_OUT_PIPE_IDX;
	mschf_data_start(msc, ep);
}

/**
 * \brief Sends the CBW packet
 * \param buf Pointer on the data to transfer (Optional)
 */
static bool mschf_scsi(struct mschf *msc)
{
	msc->cmd.tag++;

	/* Prepare CBW */
	msc->mem.cbw.dCBWSignature          = LE32(USB_CBW_SIGNATURE);
	msc->mem.cbw.bCBWLUN                = msc->lun;
	msc->mem.cbw.dCBWTag                = LE32(msc->cmd.tag);
	msc->mem.cbw.dCBWDataTransferLength = LE32(msc->cmd.size);
	return mschf_cbw_start(msc);
}

/* ------------------------------------------------------------------------- */

/** \brief Update LUN caches to reserve space for current LUN */
#if CONF_MSCHF_N_CACHED_LUN > 1
static void mschf_sel_lun_cache(struct mschf *msc, int8_t n, bool reuse)
{
	struct mschf_lun tmp;
	if (n < 0) { /* Move all around */
		n = CONF_MSCHF_N_CACHED_LUN - 1;
	} else if (n < 1) { /* No need to move around */
		return;
	}
	if (reuse) {
		memcpy(&tmp, &msc->lun_cache[n], sizeof(tmp));
	}
	for (; n > 0; n--) {
		memcpy(&msc->lun_cache[n], &msc->lun_cache[n - 1], sizeof(tmp));
	}
	if (reuse) {
		memcpy(msc->lun_cache, &tmp, sizeof(tmp));
	} else {
		memset(msc->lun_cache, 0, sizeof(tmp));
	}
}
#else
#define mschf_sel_lun_cache(msc, n, reuse)                                                                             \
	do {                                                                                                               \
		(void)msc;                                                                                                     \
		(void)n;                                                                                                       \
		(void)reuse;                                                                                                   \
	} while (0)
#endif

static int8_t mschf_find_lun_cache(struct mschf *msc, uint8_t lun)
{
#if CONF_MSCHF_N_CACHED_LUN
	uint8_t i;
	/* Find in cached LUN */
	for (i = 0; i < CONF_MSCHF_N_CACHED_LUN; i++) {
		if (msc->lun_cache[i].lun == lun) {
			return i;
		}
	}
	return -1;
#else
	return (msc->lun_cache->lun == lun) ? 0 : -1;
#endif
}

/* ------------------------------------------------------------------------- */

/** \internal
 * \brief MSC inquiry command done
 * - Error case: leave device not enabled, with error
 * - Not last LUN: Inquiry next LUN
 * - Last LUN: Enable driver
 */
static void mschf_scsi_inquiry_end(struct mschf *msc, int32_t status)
{
	if (status != ERR_NONE || msc->mem.resp.csw.dCSWDataResidue) {
		msc->error = true;
		mschf_op_end(msc, ERR_IO);
		return;
	}
	mschf_op_end(msc, ERR_NONE);

	if (msc->lun < msc->max_lun) {
		msc->lun++;
		/* Check next LUN */
		mschf_scsi_inquiry(msc);
	} else {
		/* All LUN checked
		 * Post enumeration end
		 */
		msc->is_enabled = true;
		if (msc->change) {
			msc->change(msc);
		}
	}
}

/** \internal
 * \brief MSC read_capacity command done.
 * - STALL: request sense
 * - ERROR: end selection with error
 * - OK: save capacity, start ModeSense6 for protection status
 */
static void mschf_scsi_read_capacity_end(struct mschf *msc, int32_t status)
{
	int8_t lun_i;
	if (status != ERR_NONE) {
		mschf_op_end(msc, status);
		return;
	}
	/* LUN is selected now, update cache */
	lun_i = mschf_find_lun_cache(msc, msc->lun);
	mschf_sel_lun_cache(msc, lun_i, true);
	msc->lun_cache->lun        = msc->lun;
	msc->lun_cache->bsize_x256 = BE32(msc->mem.resp.data.cap10.block_len) >> 8;
	msc->lun_cache->n_block    = BE32(msc->mem.resp.data.cap10.max_lba) + 1;
	msc->lun_cache->status.byte &= ~(MSCHF_LUN_ERROR | MSCHF_LUN_CHANGING | MSCHF_LUN_EJECTED | MSCHF_LUN_PROTECTED);
	msc->lun_cache->status.byte |= MSCHF_LUN_PRESENT;

#if CONF_MSCHF_READ_ONLY
	/* No need to update protection status, just done without error */
	mschf_op_end(msc, ERR_NONE);
#else
	/* Mode sense6 - return write-protected mode */
	mschf_scsi_mode_sense6(msc);
#endif
}

#if !CONF_MSCHF_READ_ONLY
/** \internal
 * \brief MSC mode_sense6 command done.
 * - STALL: request sense
 * - ERROR: end selection with error
 * - OK: save capacity, start ModeSense6
 */
static void mschf_scsi_mode_sense6_end(struct mschf *msc, int32_t status)
{
	if (status == ERR_NONE && msc->mem.resp.csw.bCSWStatus == USB_CSW_STATUS_PASS
	    && msc->mem.resp.csw.dCSWDataResidue >= 4) {
		/* Decode field */
		if (msc->mem.resp.data.msense.hdr.device_specific_parameter & SCSI_MS_SBC_WP) {
			msc->lun_cache->status.bm.is_protected = true;
		}
	} else {
		/* Sense6 not supported,
		 * device must be left not protected
		 */
	}
	mschf_op_end(msc, ERR_NONE);
}
#endif

static void mschf_scsi_test_unit_ready_end(struct mschf *msc, int32_t status)
{
	int8_t lun_i;

	if (status != ERR_NONE) {
		mschf_op_end(msc, status);
		return;
	}

	/* LUN present */
	lun_i = mschf_find_lun_cache(msc, msc->lun);
	if (lun_i >= 0) {
		struct mschf_lun *lun = &msc->lun_cache[lun_i];
		/* If LUN is present without any error, just select it. Note if LUN is
		 * changed, not ready to ready change must be reported by device.
		 */
		if ((lun->status.byte & ~(MSCHF_LUN_REMOVABLE | MSCHF_LUN_PROTECTED)) == MSCHF_LUN_PRESENT) {
			mschf_sel_lun_cache(msc, lun_i, true);
			mschf_op_end(msc, ERR_NONE);
			return;
		}
		/* LUN needs update */
	}
	mschf_scsi_read_capacity(msc);
}

static void mschf_read_write_end(struct mschf *msc, int32_t status)
{
	if (msc->mem.resp.csw.bCSWStatus != USB_CSW_STATUS_PASS || msc->mem.resp.csw.dCSWDataResidue) {
		mschf_op_end(msc, ERR_IO);
		return;
	}
	mschf_op_end(msc, status);
}

/* ------------------------------------------------------------------------- */

static bool mschf_scsi_inquiry(struct mschf *msc)
{
	mschf_set_timeout(msc, CONF_MSCHF_TIMEOUT);

	/* Prepare specific value of CBW packet */
	msc->cmd.buf  = (uint8_t *)&msc->mem.resp.data.inquiry;
	msc->cmd.size = sizeof(struct scsi_inquiry_data);

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_IN;
	msc->mem.cbw.bCBWCBLength = 6;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));
	msc->mem.cbw.CDB[0] = SPC_INQUIRY;
	msc->mem.cbw.CDB[4] = sizeof(struct scsi_inquiry_data);

	msc->cmd.cb = mschf_scsi_inquiry_end;
	return mschf_scsi(msc);
}

static bool mschf_scsi_read_capacity(struct mschf *msc)
{
	/* Prepare specific value of CBW packet */
	msc->cmd.buf  = (uint8_t *)&msc->mem.resp.data.cap10;
	msc->cmd.size = sizeof(struct sbc_read_capacity10_data);

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_IN;
	msc->mem.cbw.bCBWCBLength = 10;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));
	msc->mem.cbw.CDB[0] = SBC_READ_CAPACITY10;

	msc->cmd.cb = mschf_scsi_read_capacity_end;
	return mschf_scsi(msc);
}

#if !CONF_MSCHF_READ_ONLY
static bool mschf_scsi_mode_sense6(struct mschf *msc)
{
	/* Prepare specific value of CBW packet */
	msc->cmd.buf  = (uint8_t *)&msc->mem.resp.data.msense;
	msc->cmd.size = sizeof(msc->mem.resp.data.msense);

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_IN;
	msc->mem.cbw.bCBWCBLength = 6;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));
	msc->mem.cbw.CDB[0] = SPC_MODE_SENSE6;
	msc->mem.cbw.CDB[2] = SCSI_MS_MODE_INFEXP;
	msc->mem.cbw.CDB[4] = sizeof(msc->mem.resp.data.msense);

	msc->cmd.cb = mschf_scsi_mode_sense6_end;
	return mschf_scsi(msc);
}
#endif

static bool mschf_scsi_request_sense(struct mschf *msc)
{
	/* Prepare specific value of CBW packet */
	msc->cmd.buf  = (uint8_t *)&msc->mem.resp.data.sense;
	msc->cmd.size = sizeof(msc->mem.resp.data.sense);

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_IN;
	msc->mem.cbw.bCBWCBLength = 6;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));
	msc->mem.cbw.CDB[0] = SPC_REQUEST_SENSE;
	msc->mem.cbw.CDB[4] = sizeof(msc->mem.resp.data.sense);

	msc->cmd.cb = mschf_scsi_sense_done;
	return mschf_scsi(msc);
}

static bool mschf_scsi_test_unit_ready(struct mschf *msc)
{
	mschf_set_timeout(msc, CONF_MSCHF_TIMEOUT);

	/* Prepare specific value of CBW packet */
	msc->cmd.size = 0;

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_OUT;
	msc->mem.cbw.bCBWCBLength = 6;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));
	msc->mem.cbw.CDB[0] = SPC_TEST_UNIT_READY;

	msc->cmd.cb = mschf_scsi_test_unit_ready_end;
	return mschf_scsi(msc);
}

/*---------------------------------------------- Public APIs ----------------*/

int32_t mschf_init(struct usbhc_driver *core, struct mschf *msc)
{
	int32_t rc = 0;
	if (msc->is_installed) {
		return ERR_DENIED;
	}

	rc = usbhc_register_funcd(core, USBHF_PTR(msc));
	if (rc) {
		return rc;
	}
	mschf_reset(msc);
	msc->iface     = -1;
	msc->func.ctrl = mschf_ctrl;

	return ERR_NONE;
}

int32_t mschf_deinit(struct usbhc_driver *core, struct mschf *msc)
{
	int32_t rc;
	if (msc->is_installed) {
		return ERR_DENIED;
	}

	rc = usbhc_unregister_funcd(core, USBHF_PTR(msc));
	if (rc) {
		return rc;
	}
	return ERR_NONE;
}

int32_t mschf_select_lun(struct mschf *msc, const uint8_t lun, const bool check, const mschf_lun_cb_t cb)
{
	int8_t       lun_i;
	hal_atomic_t flags;
	ASSERT(msc);

	if (!msc->is_enabled) {
		return ERR_DENIED;
	}
	if (lun > msc->max_lun) {
		return ERR_INVALID_ARG;
	}

	atomic_enter_critical(&flags);
	if (msc->is_busy) {
		atomic_leave_critical(&flags);
		return ERR_DENIED;
	}
	msc->is_busy = true;
	atomic_leave_critical(&flags);

	msc->lun = lun;
	msc->cb  = cb ? cb : mschf_dummy_cb;
	lun_i    = mschf_find_lun_cache(msc, lun);
	if (check || lun_i < 0 || (lun_i >= 0 && mschf_lun_need_check(&msc->lun_cache[lun_i]))) {
		/* In case
		 *  - force check
		 *  - not cached LUN
		 *  - LUN status not ready
		 * start with TestUnitReady.
		 * Callback will be invoked when selection done and status updated
		 */
		if (mschf_scsi_test_unit_ready(msc)) {
			return ERR_NOT_READY;
		} else {
			/* Communication error, selection end */
			msc->is_busy = false;
			return ERR_IO;
		}
	}
	/* Update cache to select LUN */
	mschf_sel_lun_cache(msc, lun_i, true);
	msc->is_busy = false;
	return ERR_NONE;
}

int32_t mschf_read(struct mschf *msc, uint32_t addr, uint8_t *buf, uint16_t n_blk, mschf_lun_cb_t cb)
{
	hal_atomic_t flags;
	ASSERT(msc && buf && n_blk);

	if (!msc->is_enabled) {
		return ERR_NOT_READY;
	}
	if (addr >= msc->lun_cache[0].n_block || addr + n_blk > msc->lun_cache[0].n_block) {
		return ERR_INVALID_ARG;
	}

	atomic_enter_critical(&flags);
	if (msc->is_busy) {
		atomic_leave_critical(&flags);
		return ERR_DENIED;
	}
	msc->is_busy = true;
	atomic_leave_critical(&flags);

	if (mschf_lun_need_check(msc->lun_cache)) {
		msc->is_busy = false;
		return ERR_DENIED; /* LUN not ready */
	}

	mschf_set_timeout(msc, CONF_MSCHF_TIMEOUT);

	msc->cb = cb ? cb : mschf_dummy_cb;

	msc->cmd.buf  = buf;
	msc->cmd.size = n_blk * msc->lun_cache[0].bsize_x256 << 8;
	msc->cmd.cb   = mschf_read_write_end;

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_IN;
	msc->mem.cbw.bCBWCBLength = 10;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));

	/* CBWCB0 - Operation Code */
	msc->mem.cbw.CDB[0] = SBC_READ10;
	/* CBWCB1 - RDPROTECT, DPO, FUA, Obsolete (0x00)
	 * (done by previous memset()) */
	/* CBWCB2 to 5 - Logical Block Address (BE32) */
	msc->mem.cbw.CDB[2] = BE32B0(addr);
	msc->mem.cbw.CDB[3] = BE32B1(addr);
	msc->mem.cbw.CDB[4] = BE32B2(addr);
	msc->mem.cbw.CDB[5] = BE32B3(addr);
	/* CBWCW6 - Reserved (0x00) (done by previous memset()) */
	/* CBWCW7 to 8 - Transfer Length (BE16) */
	msc->mem.cbw.CDB[7] = BE16B0(n_blk);
	msc->mem.cbw.CDB[8] = BE16B1(n_blk);
	/* CBWCW9 - Control (0x00) (done by previous memset()) */

	return mschf_scsi(msc) ? ERR_NONE : ERR_IO;
}

int32_t mschf_write(struct mschf *msc, uint32_t addr, const uint8_t *buf, uint16_t n_blk, mschf_lun_cb_t cb)
{
#if CONF_MSCHF_READ_ONLY
	return ERR_UNSUPPORTED_OP;
#else
	hal_atomic_t flags;
	ASSERT(msc && buf && n_blk);

	if (!msc->is_enabled) {
		return ERR_NOT_READY;
	}
	if (addr >= msc->lun_cache[0].n_block || addr + n_blk > msc->lun_cache[0].n_block) {
		return ERR_INVALID_ARG;
	}

	atomic_enter_critical(&flags);
	if (msc->is_busy) {
		atomic_leave_critical(&flags);
		return ERR_DENIED;
	}
	msc->is_busy = true;
	atomic_leave_critical(&flags);

	if (mschf_lun_need_check(msc->lun_cache) | msc->lun_cache->status.bm.is_protected) {
		msc->is_busy = false;
		return ERR_DENIED; /* LUN not ready */
	}

	mschf_set_timeout(msc, CONF_MSCHF_TIMEOUT);

	msc->cb = cb ? cb : mschf_dummy_cb;

	msc->cmd.buf  = (uint8_t *)buf;
	msc->cmd.size = n_blk * msc->lun_cache[0].bsize_x256 << 8;
	msc->cmd.cb   = mschf_read_write_end;

	msc->mem.cbw.bmCBWFlags   = USB_CBW_DIRECTION_OUT;
	msc->mem.cbw.bCBWCBLength = 10;
	memset(msc->mem.cbw.CDB, 0, sizeof(msc->mem.cbw.CDB));

	/* CBWCB0 - Operation Code */
	msc->mem.cbw.CDB[0] = SBC_WRITE10;
	/* CBWCB1 - RDPROTECT, DPO, FUA, Obsolete (0x00)
	 * (done by previous memset()) */
	/* CBWCB2 to 5 - Logical Block Address (BE32) */
	msc->mem.cbw.CDB[2] = BE32B0(addr);
	msc->mem.cbw.CDB[3] = BE32B1(addr);
	msc->mem.cbw.CDB[4] = BE32B2(addr);
	msc->mem.cbw.CDB[5] = BE32B3(addr);
	/* CBWCW6 - Reserved (0x00) (done by previous memset()) */
	/* CBWCW7 to 8 - Transfer Length (BE16) */
	msc->mem.cbw.CDB[7] = BE16B0(n_blk);
	msc->mem.cbw.CDB[8] = BE16B1(n_blk);
	/* CBWCW9 - Control (0x00) (done by previous memset()) */

	return mschf_scsi(msc) ? ERR_NONE : ERR_IO;
#endif
}
