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

#ifndef _MSCHF_H_INCLUDED
#define _MSCHF_H_INCLUDED

#include <usbhf.h>
#include <usb_protocol_msc.h>
#include <sbc_protocol.h>
#include <spc_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Index for MSC pipe of Bulk IN */
#define MSCHF_IN_PIPE_IDX 0
/** Index for MSC pipe of Bulk OUT */
#define MSCHF_OUT_PIPE_IDX 1

/**
 * \brief Information of Mass Storage (MS) Logic Unit Number (LUN)
 */
typedef struct mschf_lun {
	uint32_t n_block;    /**< Number of blocks */
	uint16_t bsize_x256; /**< Block size in unit of 256 bytes */
	int8_t   lun;        /**< LUN number */
	/** Status flags for LUN */
	union {
		uint8_t byte;
		struct {
			uint8_t is_present : 1;   /**< Media is present */
			uint8_t is_changing : 1;  /**< Not ready to ready change */
			uint8_t is_protected : 1; /**< Media is write protected */
			uint8_t is_error : 1;     /**< Media access error */

			uint8_t is_removable : 1; /**< Media is removable (reserved) */
			uint8_t is_ejected : 1;   /**< Media is ejected (reserved) */
		} bm;
	} status;
} mschf_lun_t;

#define MSCHF_LUN_PRESENT (1 << 0)   /**< Media is present */
#define MSCHF_LUN_CHANGING (1 << 1)  /**< Not ready to ready change */
#define MSCHF_LUN_PROTECTED (1 << 2) /**< Media is write protected */
#define MSCHF_LUN_ERROR (1 << 3)     /**< Media access error */

#define MSCHF_LUN_REMOVABLE (1 << 4) /**< Media is removable (reserved) */
#define MSCHF_LUN_EJECTED (1 << 5)   /**< Media is ejected (reserved) */

/* Forward declare for MSC Host Function Driver */
struct mschf;

/**
 *  \brief MSC state change callback (enable/disable)
 */
typedef void (*mschf_cb_t)(struct mschf *msc);

/**
 *  \brief  MSC lun operation callback (selection/read/write end)
 */
typedef void (*mschf_lun_cb_t)(struct mschf *msc, uint8_t lun, int32_t status);

/** \internal
 *  \brief Callback used internally for MSC command processing
 */
typedef void (*mschf_cmd_cb_t)(struct mschf *msc, int32_t status);

/** \brief USB Mass Storage Host Function Driver */
typedef struct mschf {
	/** General data for USB Host function driver. */
	struct usbhf_driver func;
	/** Interface for the function */
	int8_t iface;
	/** Function is installed */
	uint8_t is_installed : 1;
	/** Function is enabled */
	volatile uint8_t is_enabled : 1;
	/** Busy on read/write */
	volatile uint8_t is_busy : 1;
	/** Clear IN HALT operation pending */
	uint8_t clr_in_halt : 1;
	/** Retry after clear in halt */
	uint8_t retry : 1;
	/** Clear OUT HALT operation pending */
	uint8_t clr_out_halt : 1;
	/** Reset Recovery operation pending */
	uint8_t reset_recovery : 1;
	/** Error */
	uint8_t error : 1;
	/** Max LUN number */
	uint8_t max_lun;
	/** LUN to check */
	uint8_t lun;
	/** Change callback */
	mschf_cb_t change;
	/** Operation callback */
	mschf_lun_cb_t cb;
	/** Resource wait callback */
	struct usbhc_handler_ext rsc_hdl;
#if CONF_MSCHF_TIMEOUT
	/** SOF callback for timeout check */
	struct usbhc_handler_ext sof_hdl;
	/** Timeout */
	uint32_t timeout;
#endif
	/** Pipes */
	struct usb_h_pipe *pipe[2];
	/** Command process data */
	struct {
		uint8_t *      buf;      /**< Pointer to data buffer */
		uint32_t       size;     /**< Size of data (in bytes) */
		uint32_t       tag;      /**< MSC command tag */
		mschf_cmd_cb_t cb;       /**< Callback invoked when command ends */
		mschf_cmd_cb_t sense_cb; /**< Callback invoked when request sense ends */
	} cmd;
	/** Cache for LUN information */
	struct mschf_lun lun_cache[CONF_MSCHF_N_CACHED_LUN];
	/** Memory buffer for command process */
	union {
		/** Command Block Wrapper, 31 Bytes */
		struct usb_msc_cbw cbw;
		/** Command response*/
		struct {
			union {
				/** read_capacity10 data */
				struct sbc_read_capacity10_data cap10;
				/** inquiry data */
				struct scsi_inquiry_data inquiry;
				/** request_sense data */
				struct scsi_request_sense_data sense;
				/** Return data for mode sense, only request
				 *  one of the pages to save memory usage */
				struct {
					/** Mode parameter header */
					struct scsi_mode_param_header6 hdr;
					/** Informational exceptions control page */
					struct spc_control_page_info_execpt cpie;
				} msense;
			} data;
			/** Command Status Wrapper, 13 Bytes */
			struct usb_msc_csw csw;
		} resp;
	} mem;
} mschf_t;

/** Cast pointer to MSC host function driver */
#define MSCHF_PTR(ptr) ((struct mschf *)(ptr))

/** \brief Initialize the MSC function driver and attach it to core
 *  \param[in,out] core Pointer to USB Host core driver instace
 *  \param[in,out] msc  Pointer to USB Host MSC function driver instance
 *  \return Operation result status
 */
int32_t mschf_init(struct usbhc_driver *core, struct mschf *msc);

/** \brief Deitialize the MSC function driver and detach it from core
 *  \param[in,out] core Pointer to USB Host core driver instace
 *  \param[in,out] msc  Pointer to USB Host MSC function driver instance
 *  \return Operation result status
 */
int32_t mschf_deinit(struct usbhc_driver *core, struct mschf *msc);

/** \brief Register callback invoked on driver change (enable/disable)
 *  Check enable/disable with \ref mschf_is_enabled() in callback.
 *  \param[out] msc Pointer to MSC instance
 *  \param[in]  cb  Pointer to callback function
 */
static inline void mschf_register_callback(struct mschf *msc, mschf_cb_t cb)
{
	ASSERT(msc);
	msc->change = cb;
}

/** \brief Check if MSC is enabled
 *  \param[in] msc Pointer to MSC instance
 */
static inline bool mschf_is_enabled(const struct mschf *msc)
{
	ASSERT(msc);
	return msc->is_enabled;
}

/** \brief Check if MSC is installed
 *  \param[in] msc Pointer to MSC instance
 */
static inline bool mschf_is_installed(const struct mschf *msc)
{
	ASSERT(msc);
	return msc->is_installed;
}

/** \brief Check if MSC is busy (selecting/reading/writing)
 *  \param[in] msc Pointer to MSC instance
 */
static inline bool mschf_is_busy(const struct mschf *msc)
{
	ASSERT(msc);
	return msc->is_busy;
}

/** \brief Return pointer to current selected LUN
 *  \param[in] msc Pointer to MSC instance
 */
static inline struct mschf_lun *mschf_curr_lun(struct mschf *msc)
{
	ASSERT(msc);
	return &msc->lun_cache[0];
}

/** \brief Check if LUN is in a state that needs re-check
 *  \param[in] lun Pointer to LUN instance
 */
static inline bool mschf_lun_need_check(struct mschf_lun *lun)
{
	ASSERT(lun);
	/* Check if it's present without any error */
	return (MSCHF_LUN_PRESENT
	        != (lun->status.byte & (MSCHF_LUN_PRESENT | MSCHF_LUN_CHANGING | MSCHF_LUN_ERROR | MSCHF_LUN_EJECTED)));
}

/** \brief Return LUN block size of bytes
 *  \param[in] lun Pointer to LUN instance
 */
static inline uint32_t mschf_lun_block_size(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->bsize_x256 << 8);
}

/** \brief Return the last block address of LUN
 *  \param[in] lun Pointer to LUN instance
 */
static inline uint32_t mschf_lun_last_block(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->n_block - 1);
}

/** \brief Return size of LUN in number of blocks
 *  \param[in] lun Pointer to LUN instance
 */
static inline uint32_t mschf_lun_size_blocks(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->n_block);
}

/** \brief Return size of LUN in bytes
 *  \param[in] lun Pointer to LUN instance
 */
static inline uint32_t mschf_lun_size_bytes(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->bsize_x256 * lun->n_block << 8);
}

/** \brief Return size of LUN in 1024-byte-unit
 *  \param[in] lun Pointer to LUN instance
 */
static inline uint32_t mschf_lun_size_kb(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->bsize_x256 * lun->n_block >> 2);
}

/** \brief Check if the LUN is changing from not ready to ready
 *  \param[in] lun Pointer to LUN instance
 */
static inline bool mschf_lun_is_changing(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->status.bm.is_changing);
}

/** \brief Check if the LUN is present
 *  \param[in] lun Pointer to LUN instance
 */
static inline bool mschf_lun_is_present(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->status.bm.is_present);
}

/** \brief Check if the LUN is protected
 *  \param[in] lun Pointer to LUN instance
 */
static inline bool mschf_lun_is_protected(const struct mschf_lun *lun)
{
	ASSERT(lun);
	return (lun->status.bm.is_protected);
}

/** \brief Return max LUN number
 *  \param[in] msc Pointer to MSC instance
 */
static inline uint8_t mschf_max_lun(const struct mschf *msc)
{
	ASSERT(msc);
	return msc->max_lun;
}

/**
 * \brief Try to select a LUN for further operations (read/write)
 *
 * \param[in,out] msc   Pointer to MSC instance
 * \param[in]     lun   LUN number
 * \param[in]     check Set to \c true to force LUN status update even it's cached
 * \param[in]     cb    Callback to be invoked when selection process is done
 *
 * \note Callback is used only if process is started in background, that is,
 *       \ref ERR_NOT_READY is returned.
 *
 * \return Operation result status code
 * \retval ERR_DENIED Operation denied because driver not ready or
 *                    other LUN operation in progress
 * \retval ERR_INVALID_ARG LUN number exceeds max
 * \retval ERR_NOT_READY LUN selection in progress,
 *                       user should wait end callback to be invoked.
 * \retval ERR_NONE LUN selection is done
 */
int32_t mschf_select_lun(struct mschf *msc, const uint8_t lun, const bool check, const mschf_lun_cb_t cb);

/**
 * \brief Start reading up to 65535 blocks from selected LUN
 *
 * \note Callback is used only if process is started in background, that is,
 *       \ref ERR_NONE is returned.
 *
 * \param[in,out] msc  Pointer to MSC instance
 * \param[in]     addr Block address to read
 * \param[out]    buf  Pointer to buffer to fill data
 * \param[in]     cb   Callback invoked when read done
 *
 * \return Operation result status code
 * \retval ERR_DENIED Operation denied because driver not ready or
 *                    other LUN operation in progress
 * \retval ERR_NOT_READY Function not enabled
 * \retval ERR_INVALID_ARG Address or number of blocks exceeds available area
 * \retval ERR_NONE Reading started successfully
 */
int32_t mschf_read(struct mschf *msc, uint32_t addr, uint8_t *buf, uint16_t n_blk, mschf_lun_cb_t cb);

/**
 * \brief Start writing up to 65535 blocks to selected LUN
 *
 * \note Callback is used only if process is started in background, that is,
 *       \ref ERR_NONE is returned.
 *
 * \param[in,out] msc  Pointer to MSC instance
 * \param[in]     addr Block address to read
 * \param[in]     buf  Pointer to buffer of data to write
 * \param[in]     cb   Callback invoked when read done
 *
 * \return Operation result status code
 * \retval ERR_DENIED Operation denied because driver not ready or
 *                    other LUN operation in progress
 * \retval ERR_NOT_READY Function not enabled
 * \retval ERR_INVALID_ARG Address or number of blocks exceeds available area
 * \retval ERR_NONE Writing started successfully
 */
int32_t mschf_write(struct mschf *msc, uint32_t addr, const uint8_t *buf, uint16_t n_blk, mschf_lun_cb_t cb);

#ifdef __cplusplus
}
#endif

#endif /* _MSCHF_H_INCLUDED */
