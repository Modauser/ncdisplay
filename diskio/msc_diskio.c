/**
 * \file
 *
 * \brief Common MSC DiskIO implementation
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
 *
 */

#include "diskio.h"
#include "usb_start.h"
#include <mschf.h>

/**
 * \brief Select a MSC disk
 *
 * \param[in]  drv          The driver number
 * \param[in]  force_check  Force to check the driver status instead of take cached things
 *
 * \return     Pointer to MSC LUN structure for disk information
 */
struct mschf_lun *msc_diskio_select(uint8_t drv, bool force_check)
{
	struct mschf *    msc     = &USB_HOST_MSC_0_inst;
	struct mschf_lun *lun     = mschf_curr_lun(msc);
	uint8_t           drv_off = 0;
	int32_t           rc;
	int8_t            i;
	if (!mschf_is_enabled(msc)) {
		return NULL;
	}
	if (drv - drv_off > mschf_max_lun(msc)) {
		return NULL;
	}
	/* USB Disk could report not ready to ready then present, some devices may
	 * report more, check several times here to confirm the media is ready.
	 */
	for (i = 0; i < 5; i++) {
		rc = mschf_select_lun(msc, drv - drv_off, force_check, NULL);
		if (rc == ERR_NOT_READY) {
			while (mschf_is_busy(msc))
				;
		}
		lun = mschf_curr_lun(msc);
		if (lun->status.byte & (MSCHF_LUN_PRESENT | MSCHF_LUN_ERROR | MSCHF_LUN_EJECTED)) {
			/* State confirmed, no need to check any more */
			break;
		}
	}
	if (mschf_lun_need_check(lun)) {
		return NULL;
	}
	return lun;
}

/**
 * \brief Read sectors from selected LUN
 *
 * \param[in]  sector  The sector
 * \param[out] buff    The buffer
 * \param[in]  count   The count
 *
 * \return     Operation result status
 * \retval     0 Operation done success
 * \retval     ERR_INVALID_ARG Argument error
 * \retval     <0 Other errors
 */
int32_t msc_diskio_read(uint32_t sector, uint8_t *buff, uint8_t count)
{
	struct mschf *    msc = &USB_HOST_MSC_0_inst;
	struct mschf_lun *lun;
	int32_t           rc;
	rc = mschf_read(msc, sector, buff, count, NULL);
	if (rc) {
		return rc;
	}
	while (mschf_is_busy(msc))
		;
	lun = mschf_curr_lun(msc);
	if (mschf_lun_need_check(lun)) {
		return ERR_NOT_READY;
	}
	return ERR_NONE;
}

/**
 * \brief Write sectors to selected LUN
 *
 * \param[in]  sector  The sector
 * \param[in]  buff    The buffer
 * \param[in]  count   The count
 *
 * \return     Operation result status
 * \retval     0 Operation done success
 * \retval     ERR_INVALID_ARG Argument error
 * \retval     <0 Other errors
 */
int32_t msc_diskio_write(uint32_t sector, uint8_t *buff, uint8_t count)
{
	struct mschf *    msc = &USB_HOST_MSC_0_inst;
	struct mschf_lun *lun;
	int32_t           rc;
	rc = mschf_write(msc, sector, buff, count, NULL);
	if (rc) {
		return rc;
	}
	while (mschf_is_busy(msc))
		;
	lun = mschf_curr_lun(msc);
	if (mschf_lun_need_check(lun)) {
		return ERR_NOT_READY;
	}
	return ERR_NONE;
}

/**
 * \brief Initialize a disk.
 *
 * \param drv Physical drive number (0..).
 *
 * \return 0 or disk status in combination of DSTATUS bits
 *         (STA_NOINIT, STA_PROTECT).
 */
dstatus_t msc_disk_initialize(uint8_t drv)
{
	struct mschf_lun *lun = msc_diskio_select(drv, false);
	if (!lun) {
		return STA_NOINIT;
	}
	if (!mschf_lun_is_present(lun)) {
		return STA_NODISK;
	}
	if (mschf_lun_is_protected(lun)) {
		return STA_PROTECT;
	}
	return 0;
}

/**
 * \brief  Return disk status.
 *
 * \param drv Physical drive number (0..).
 *
 * \return 0 or disk status in combination of DSTATUS bits
 *         (STA_NOINIT, STA_NODISK, STA_PROTECT).
 */
dstatus_t msc_disk_status(uint8_t drv)
{
	struct mschf_lun *lun = msc_diskio_select(drv, true);
	if (!lun) {
		return STA_NOINIT;
	}
	if (!mschf_lun_is_present(lun)) {
		return STA_NOINIT | STA_NODISK;
	}
	if (mschf_lun_is_protected(lun)) {
		return STA_PROTECT;
	}
	return 0;
}

/**
 * \brief  Read sector(s).
 *
 * \param drv Physical drive number (0..).
 * \param buff Data buffer to store read data.
 * \param sector Sector address (LBA).
 * \param count Number of sectors to read (1..255).
 *
 * \return 0 for success, otherwise DRESULT error code.
 */
dstatus_t msc_disk_read(uint8_t drv, uint8_t *buff, uint32_t sector, uint8_t count)
{
	struct mschf_lun *lun = msc_diskio_select(drv, false);
	int32_t           rc;
	if (!lun) {
		return RES_NOTRDY;
	}
	if (!mschf_lun_is_present(lun)) {
		return RES_NOTRDY;
	}
	rc = msc_diskio_read(sector, buff, count);
	if (rc == ERR_INVALID_ARG) {
		return RES_PARERR;
	} else if (rc) {
		return RES_ERROR;
	}
	return 0;
}

/**
 * \brief  Write sector(s).
 *
 * The FatFs module will issue multiple sector transfer request (count > 1) to
 * the disk I/O layer. The disk function should process the multiple sector
 * transfer properly. Do not translate it into multiple sector transfers to the
 * media, or the data read/write performance may be drastically decreased.
 *
 * \param drv Physical drive number (0..).
 * \param buff Data buffer to store read data.
 * \param sector Sector address (LBA).
 * \param count Number of sectors to read (1..255).
 *
 * \return 0 for success, otherwise DRESULT error code.
 */
dstatus_t msc_disk_write(uint8_t drv, uint8_t const *buff, uint32_t sector, uint8_t count)
{
	struct mschf_lun *lun = msc_diskio_select(drv, false);
	int32_t           rc;
	if (!lun) {
		return RES_NOTRDY;
	}
	if (!mschf_lun_is_present(lun)) {
		return RES_NOTRDY;
	}
	rc = msc_diskio_write(sector, (uint8_t *)buff, count);
	if (rc == ERR_INVALID_ARG) {
		return RES_PARERR;
	} else if (rc) {
		if (mschf_lun_is_protected(lun)) {
			return RES_WRPRT;
		}
		return RES_ERROR;
	}
	return 0;
}

/**
 * \brief  Miscellaneous functions, which support the following commands:
 *
 * CTRL_SYNC    Make sure that the disk drive has finished pending write
 * process. When the disk I/O module has a write back cache, flush the
 * dirty sector immediately.
 * In read-only configuration, this command is not needed.
 *
 * GET_SECTOR_COUNT    Return total sectors on the drive into the DWORD variable
 * pointed by buffer.
 * This command is used only in f_mkfs function.
 *
 * GET_BLOCK_SIZE    Return erase block size of the memory array in unit
 * of sector into the DWORD variable pointed by Buffer.
 * When the erase block size is unknown or magnetic disk device, return 1.
 * This command is used only in f_mkfs function.
 *
 * GET_SECTOR_SIZE    Returns sector size of the device into the WORD variable
 * pointed by buffer.
 *
 * \param drv Physical drive number (0..).
 * \param ctrl Control code.
 * \param buff Buffer to send/receive control data.
 *
 * \return 0 for success, otherwise DRESULT error code.
 */
dstatus_t msc_disk_ioctl(uint8_t drv, uint8_t ctrl, void *buff)
{
	struct mschf_lun *lun = msc_diskio_select(drv, false);
	dstatus_t         res = 0;

	if (!lun) {
		return RES_NOTRDY;
	}

	switch (ctrl) {
	/* Get erase block size (DWORD) */
	case GET_BLOCK_SIZE:
		*(uint32_t *)buff = 1; /* non flash memory media */
		break;

	/* Get the number of sectors on the disk (DWORD) */
	case GET_SECTOR_COUNT:
		*(uint32_t *)buff = mschf_lun_size_blocks(lun);
		break;

	/* Get sector size (WORD) */
	case GET_SECTOR_SIZE: {
		uint16_t sector_size = mschf_lun_block_size(lun);
		if ((sector_size != 512) && (sector_size != 1024) && (sector_size != 2048) && (sector_size != 4096)) {
			/* The sector size is not supported by the FatFS */
			res = RES_PARERR;
		} else {
			*(uint16_t *)buff = sector_size;
		}
	} break;

	/* Make sure that data has been written */
	case CTRL_SYNC:
		break;

	default:
		res = RES_PARERR;
	}

	return res;
}
