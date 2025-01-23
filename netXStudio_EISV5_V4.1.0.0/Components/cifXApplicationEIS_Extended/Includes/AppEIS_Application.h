/**************************************************************************************
Exclusion of Liability for this software:
  The following software is intended for and must only be used for reference and in an
  evaluation laboratory environment. It is provided without charge and is subject to
  alterations. There is no warranty for the software, to the extent permitted by
  applicable law. Except when otherwise stated in writing the copyright holders and/or
  other parties provide the software "as is" without warranty of any kind, either
  expressed or implied.
  Please refer to the Agreement in README_DISCLAIMER.txt, provided together with this file!
  By installing or otherwise using the software, you accept the terms of this Agreement.
  If you do not agree to the terms of this Agreement, then do not install or use the
  Software!
**************************************************************************************/

/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************/

#ifndef COMPONENTS_CIFXAPPLICATIONEIS_INCLUDES_APPEIS_APPLICATION_H_
#define COMPONENTS_CIFXAPPLICATIONEIS_INCLUDES_APPEIS_APPLICATION_H_

#include "App_Application.h"
#include "Hil_ProductCodeCip.h"
#include "eip_eif_packetdefinitions_packets.h"

/***************************************************************************************/

#define EIP_CHANNEL_INDEX  0

/*************************************************************************************/
/*************************************************************************************/

typedef struct CIP_OBJ_DIR_Ttag
{
  uint32_t ulClass;
  uint32_t ulInstance;
  uint32_t ulAttribute;
  uint32_t ulSize;
  uint8_t* pbData;
} CIP_OBJ_DIR_T;

/*************************************************************************************/
/*************************************************************************************/

typedef struct CIP_ASSEMBLY_DIR_Ttag
{
  uint32_t ulInstance;
  uint32_t ulSize;
  uint32_t ulOffset;
  uint32_t ulFlags;
} CIP_ASSEMBLY_DIR_T;

/*************************************************************************************/
/*************************************************************************************/

typedef struct CIP_SUPP_CONNECTION_DIR_Ttag
{
  uint32_t ulConsumerCP;
  uint32_t ulProducerCP;
  uint32_t ulConfigCP;
  uint32_t  bConnType;
} CIP_SUPP_CONNECTION_DIR_T;

/*************************************************************************************/
/*************************************************************************************/

typedef struct CIP_REGISTER_OBJ_DIR_Ttag
{
    uint32_t ulClass;
    uint32_t ulOptionFlags;
    void (*pf_indication)(APP_DATA_T* ptAppData,
                         EIP_OBJECT_PACKET_CL3_SERVICE_IND_T *ptInd,
                         EIP_OBJECT_PACKET_CL3_SERVICE_RES_T *ptRes,
                         uint32_t *pulResDataSize );

} CIP_REGISTER_OBJ_DIR_T;

/*************************************************************************************/
/*************************************************************************************/

typedef struct CIP_ACTIVATE_ATTRIBUTE_DIR_Ttag
{
  uint32_t ulClass;
  uint32_t ulInstance;
  uint32_t ulAttribute;

} CIP_ACTIVATE_ATTRIBUTE_DIR_T;

/*************************************************************************************/
/*************************************************************************************/

bool     AppEIS_PacketHandler( CIFX_PACKET* ptPacket, void* pvUserData );
uint32_t AppEIS_ConfigureStack( APP_DATA_T* ptAppData );
uint32_t AppEIS_SetMacAddress( APP_DATA_T* ptAppData );
uint32_t AppEIS_SetOemDeviceData( APP_DATA_T* ptAppData );
uint32_t AppEIS_ActivateDdp( APP_DATA_T* ptAppData );
uint32_t AppEIS_SetRemanentData( APP_DATA_T* ptAppData );

#endif /* COMPONENTS_CIFXAPPLICATIONEIS_INCLUDES_APPEIS_APPLICATION_H_ */
