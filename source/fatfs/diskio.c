/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include "../mmc/block.h"

struct block_device * sddevice;

unsigned int drvstatus = STA_NOINIT;

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result = 0;
        
        stat = STA_NOINIT;
        drvstatus = STA_NOINIT;
        
        result = sd_card_init(&sddevice);
        
        if (result != -1) {

                stat = 0;
                drvstatus = 0;
        }

        return stat;

}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	//DSTATUS stat;
	//int result;
        
        return drvstatus;

	//return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res = 0;
	int result;

        result = sd_read(sddevice, buff, count * 512 , sector);
                
        if (result < 0) res = RES_ERROR;
        else res = RES_OK;

        return res;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = 0;
	int result;

        result = sd_write(sddevice, buff, count *512, sector);
        
        if (result < 0) res = RES_ERROR;
        else res = RES_OK;

        return res;
        
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DWORD get_fattime (void) {
    return 1;
}

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	// BYTE n, csd[16];
	// DWORD cs;


	if (disk_status(drv) & STA_NOINIT) return RES_NOTRDY;	/* Check if card is in the socket */

	res = RES_ERROR;
	switch (ctrl) {
		case CTRL_SYNC :		/* Make sure that no pending write process */
			// if (select()) res = RES_OK;
                        res = RES_OK;
			break;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
                    /*
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
				if ((csd[0] >> 6) == 1) {	// SDC ver 2.00 
					cs = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
					*(DWORD*)buff = cs << 10;
				} else {					// SDC ver 1.XX or MMC 
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					cs = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = cs << (n - 9);
				}
				res = RES_OK;
			}
                     */
                                *(DWORD*)buff = 1000;
                                res = RES_OK;
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
			*(DWORD*)buff = 1;
			res = RES_OK;
			break;
                        
                case GET_SECTOR_SIZE :
                        *(DWORD*)buff = 512;
                        res = RES_OK;
                    break;
                    
                case CTRL_ERASE_SECTOR :
                        res = RES_OK;
                    break;

		default:
			res = RES_PARERR;
	}


	return res;
}
#endif
