/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include <diskio.h>		/* Declarations of disk functions */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

dstatus_t disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	dstatus_t result = STA_NOINIT;

	switch (pdrv) {
	case DEV_MMC :
		result = sdmmc_disk_status(0);
		break;
	case DEV_USB :
		result = msc_disk_status(0);
		break;
	}
	return result;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

dstatus_t disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	dstatus_t result = STA_NOINIT;

	switch (pdrv) {
	case DEV_MMC :
		result = sdmmc_disk_initialize(0);
		break;
	case DEV_USB :
		result = msc_disk_initialize(0);
		break;
	}
	return result;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

dresult_t disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	dresult_t result = RES_PARERR;

	switch (pdrv) {
	case DEV_MMC :
		result = sdmmc_disk_read(0, buff, sector, count);
		break;
	case DEV_USB :
		result = msc_disk_read(0, buff, sector, count);
		break;
	}

	return result;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

dresult_t disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	dresult_t result = RES_PARERR;

	switch (pdrv) {
	case DEV_MMC :
		result = sdmmc_disk_write(0, buff, sector, count);
		break;
	case DEV_USB :
		result = msc_disk_write(0, buff, sector, count);
		break;
	}

	return result;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

dresult_t disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	dresult_t result = RES_PARERR;

	switch (pdrv) {
	case DEV_MMC :
		result = sdmmc_disk_ioctl(0, cmd, buff);
		break;
	case DEV_USB :
		result = msc_disk_ioctl(0, cmd, buff);
		break;
	}

	return result;
}

