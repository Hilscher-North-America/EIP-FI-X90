#ifndef NVRAM_H_
#define NVRAM_H_

#define NVRAM_KBYTES  32

//todo checksum and header
#define NVRAM_MAX_SIZE (61+36)         /* 60 (total data) + 12 * 3 (12 registers + 3 bytes for each register)\) \
	7 system registers: \
	 NV_HWREV 				(12 bytes)(12)\
	 NV_SERNUMBER 		(4 bytes) (16)\
	 NV_MACADDRESS 		(6 bytes) (22)\
	 NV_PRODUCT_CODE 	(12 bytes)(34)\
	 NV_IPADDRESS 		(4 bytes) (38)\
	 NV_IPMASK 				(4 bytes) (42)\
	 NV_IPGATEWAY 		(4 bytes) (46)\	 
	4 parameters:\
	 NV_BASE_IPADDRESS(4 bytes) (50)\
	 NV_HOT_CPU 			(2 bytes) (52)\
	 NV_TEMP_INTERCEPT(4 bytes) (56)\
	 NV_TEMP_SLOPE		(4 bytes) (60)
	 NV_ANALOG_ENABLED(1 byte)*/	 
	 
#define NVRAM_CHECKSUM_SIZE_LONG  2 // 4 byte checksum + 4 byte header

typedef enum
{
	NV_OK = 0,
	NV_FULL ,
	NV_SIZE_MISMATCH,
	NV_IDX_NOT_FOUND,
	NV_EXISTS,
	NV_READONLY
}NVRAM_ERRS;

typedef enum
{
	NV_UNUSED = 0,
	
	// ENUM values 1 - 0xDFFF are reserved for the application to use.
	
  // Start of platform items
	NV_SLICE_0 = 0xE000,    //PHOEN-172
	NV_SLICE_LAST = 0xE0FF,
	
	NV_ERASED = 0xF000,
	NV_SERNUMBER = 0xF001,					/* active */
	NV_CHECKSUM = 0xF002,	
	NV_MACADDRESS = 0xF003,					/* active */
	NV_IPADDRESS = 0xF004,					/* active */
	NV_BAUDRATE = 0xF005,
	NV_HWREV = 0xF006, 							/* active */
	NV_IPMASK = 0xF007,							/* active */
	NV_IPGATEWAY = 0xF008,					/* active */
	NV_PRODUCT_CODE = 0xF009,				/* active */
	NV_PROTOCOL = 0xF00A,
	NV_ECLREN = 0xF00B,
	NV_BASE_IPADDRESS = 0xF00C,			/* active */
	NV_HOT_CPU = 0xF00D,						/* active */
	NV_TEMP_INTERCEPT = 0xF00E,			/* active */
	NV_TEMP_SLOPE = 0xF00F,					/* active */	
	NV_ANALOG_ENABLED = 0xF010,			/* active */	
	
	NV_COMPRESSED_LZ4 = 0xFFF0,
	NV_UNUSED_ERASED = 0xFFFF
}NVRAM_DEFS;

typedef enum
{
	NV_BYTE = 1,
	NV_SHORT = 2,
	NV_LONG = 4,
	NV_ETHMAC = 6,
	NV_PWTYPE = 9 /*8 + null*/
}NVRAM_SIZES;

/* external public global variables */
extern uint8_t RAMCOPY[NVRAM_MAX_SIZE];

/* public function prototypes */
int NvReadRam(uint16_t ridx, void *val, uint8_t dsz);
int NvWriteRam(uint16_t widx, void * val, uint8_t dsz);
void NvWriteStart();

#endif /*NVRAM_H_*/
