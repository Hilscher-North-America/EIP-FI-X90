#include "sys.h"
#include "nvram.h"

uint8_t *NVCOPY1;
uint8_t *NVCOPY2;
uint8_t RAMCOPY[NVRAM_MAX_SIZE] = {0};
uint8_t *ACTIVECOPY = RAMCOPY;

uint16_t nvsize = 0;
static uint8_t bNvWriteable = 1; //todo review

/*****************************************************************************/
/*! Reverts NVRAM copy.
 * 
 */
/*****************************************************************************/
static void NvRevert(void)
{
	printf("Reverting to original NVRAM copy.\r\n");
	if(ACTIVECOPY == NVCOPY1)
	{
		ACTIVECOPY = NVCOPY2;
	}
	else if(ACTIVECOPY == NVCOPY2)
	{
		ACTIVECOPY = NVCOPY1;
	}
	// bNvWriteable = 0; //todo
}

/*****************************************************************************/
/*! Reads NVRAM parameter from the RAM copy.
 *
 * \param widx  [in]  Index of the NVRAM parameter.
 * \param val   [in]  Pointer to data value.
 * \param dsz   [in]  Sizo of data.
 */
/*****************************************************************************/
int NvReadRam(uint16_t ridx, void *val, uint8_t dsz)
{
	//uint16_t loc=(NVRAM_CHECKSUM_SIZE_LONG * sizeof(long));  //todo chksum and header/version???
	uint16_t loc=0;
	uint8_t *dptr = (uint8_t*)ACTIVECOPY;
	uint16_t tmp;	
	
	if(ACTIVECOPY == NULL)
		return NV_IDX_NOT_FOUND;
	
	// Find the NV location by parsing through the table
	while(loc < NVRAM_MAX_SIZE)
	{
	  /* get data type */
		memcpy(&tmp,&dptr[loc],2);
		
		if(ridx != tmp)
		{
			loc += (dptr[loc+2] + 3);	
		}
		else
			break;
	}
	
	dptr += loc;
	
	//If loc is beyond the entire nvram data, it has not been found
	if(loc >= NVRAM_MAX_SIZE)
		return NV_IDX_NOT_FOUND;
	
	dptr+=2;
	
	//Return error if the requested size does not match
	if(dsz != *dptr++)
		return NV_SIZE_MISMATCH;
	
	memcpy(val,dptr,dsz);
	
	return NV_OK;
}

/*****************************************************************************/
/*! Write NVRAM parameter to the RAM copy.
 *
 * \param widx  [in]  Index of the NVRAM parameter.
 * \param val   [in]  Pointer to data value.
 * \param dsz   [in]  Size of data.
 */
/*****************************************************************************/
int NvWriteRam(uint16_t widx, void * val, uint8_t dsz)
{
	//uint16_t loc=(NVRAM_CHECKSUM_SIZE_LONG * sizeof(long));  //todo chksum and header/version???
	uint16_t loc=0;
	uint8_t *dptr = (uint8_t*)RAMCOPY;
	uint16_t tmp;
	uint8_t hdr[3];

	if(!bNvWriteable)
	{
		printf("NVRAM Read Only\r\n");
		return NV_READONLY;
	}
	
	if((dptr == NULL) || (ACTIVECOPY == NULL))
	{
		printf("NVRAM Prm Index Not Found\r\n");
		return NV_IDX_NOT_FOUND;
	}		

	// Find the NV location by parsing through the table
	while(loc < nvsize)
	{
		memcpy(&tmp,&dptr[loc],2);
		
		if(widx != tmp)
		{
			loc += dptr[loc+2] + 3;	
		}
		else
		{
			printf("Duplicate entry found %04X, %u\r\n", widx, widx);
			NvRevert();
			return NV_EXISTS;
		}
	}
		
	dptr += loc;
	
	//If loc is beyond the entire nvram data, it has not been found
	if(loc >= (NVRAM_MAX_SIZE))
	{
		printf("NVRAM Full! loc %d\r\n", loc);
		NvRevert();
		return NV_FULL;
	}
	
	//If loc is in the table, the index is found. Compare the size
	else if(loc < nvsize)
	{
		dptr+=2;
		
		//Return error if the requested size does not match
		if(dsz != *dptr++)
		{
			printf("NVRAM Prm Size Mismatch\r\n");
			NvRevert();
			return NV_SIZE_MISMATCH;
		}
	}
	
	//Add a new entry
	else
	{
		if((loc+dsz+3) > (NVRAM_MAX_SIZE))
		{
			printf("NVRAM Full Prm cannot be added\r\n");
			NvRevert();
			return NV_FULL;
		}
		
		memcpy(hdr,&widx,2);
		
		hdr[2] = dsz;
#ifndef NO_NVBUFFER
		memcpy(dptr, hdr, 3);
#else
		len = _io_writeflash_file, hdr, 3);
		if(len != 3)
		{
			NvRevert();
			return NV_SIZE_MISMATCH;
		}
#endif
		dptr += 3;
		nvsize += (dsz+3);			
	}
	
#ifndef NO_NVBUFFER
	memcpy(dptr,val, dsz);
#else
	len = _io_writeflash_file, val, dsz);
	if(len != dsz)
	{
		NvRevert();
		return NV_SIZE_MISMATCH;
	}
#endif
	
	return NV_OK;
}

/*****************************************************************************/
/*! Reset NVRAM buffer size to 0.
 *
 */
/*****************************************************************************/
void NvWriteStart()
{
	nvsize = 0;
}
