#ifndef FI_H_
#define FI_H_

#include "cip_objects.h"

#define FI_DEFAULT_IP_ADDR IP_ADDRESS(192,168,9,1)
#define FI_DEFAULT_NETMASK IP_ADDRESS(255,255,255,0)

#define MKS_MAC_ADDRESS 0x170B00

#define RESETKEY	0x4655

#define FLASHBOOT 0x4254
#define FLASHAPP 0x4150

#define OBRKEY  0x4F42
#define NVSAVE 	0x4E56  // NV save without rebooting

#define CLEARKEY 0

#ifndef SWAPLONG
#define SWAPLONG(x) 	((uint32_t)((uint32_t)(x)<<16)|((uint32_t)(x)>>16))  //swap word order only, no byte swap in high and low word
#endif

// manufacturer functions prototype
int 	    MfgSetKey(uint16_t ui16Key);
uint16_t  MfgGetKey(void);
void      MfgInitParams();
void      MfgSaveParams(uint8_t obr);
void      MfgDelParams();

void		  UtilDoMfg(void);
int32_t   GetFwUpdateStatus(void);

#endif /* FI_H_ */
