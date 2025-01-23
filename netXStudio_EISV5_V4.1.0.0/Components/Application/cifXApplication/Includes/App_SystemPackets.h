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

/********************************************************************************
 ********************************************************************************
 *      Copyright (c) Hilscher North America.            All Rights Reserved.
 ********************************************************************************
 *      File Name:           App_SystemPackets.h
 *      Product:             ETHERNET/IP MIDDLEWARE
 *      Author(s):           Jarek Bojanowicz & Andres Torres
 *      Created on:          XX/XX/XXXX
 *      Updated on:          xx/xx/xxxx
 *      Protocol Stack:      ETHERNET/IP ADAPTER V5.4.0.3
 ********************************************************************************
 * Module Source Description:
 *
 * The header contains the general packet header definitions and declarations.
 *
 ********************************************************************************
 ********************************************************************************/

#ifndef COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SYSTEMPACKETS_H_
#define COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SYSTEMPACKETS_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "App_Application.h"
#include "App_PacketCommunication.h"
#include "cifXUser.h"
#include "cifXErrors.h"

/*****************************************************************************/
/*! FORWARD DECLARATIONS                                                     */
/*****************************************************************************/

struct APP_DATA_Ttag;

/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/

void    App_SysPkt_AssembleRegisterAppReq     ( CIFX_PACKET* ptPkt);

void    App_SysPkt_AssembleSetMacAddressReq   ( CIFX_PACKET* ptPkt, uint8_t* abMacAddr );

void    App_SysPkt_AssembleChannelInitReq     ( CIFX_PACKET* ptPkt);

void    App_SysPkt_AssembleStartStopCommReq   ( CIFX_PACKET* ptPkt, bool fStart );

void    App_SysPkt_AssembleFirmwareIdentifyReq( CIFX_PACKET* ptPkt, uint32_t ulChannelId );
void    App_SysPkt_HandleFirmwareIdentifyCnf  ( CIFX_PACKET* ptPkt );

void    App_SysPkt_AssembleHardwareInfoReq    ( CIFX_PACKET* ptPkt );
void    App_SysPkt_HandleHardwareInfoCnf      ( CIFX_PACKET* ptHardwareInfoCnf );

int32_t App_SysPkt_HandleLinkStatusChangeInd  ( struct APP_DATA_Ttag*  ptAppData, int iChannelIdx, CIFX_PACKET* ptPacket );

void App_SysPkt_AssembleFirmwareResetReq( CIFX_PACKET* ptPkt, uint8_t bResetMode, uint8_t bResetParam, uint8_t bDeleteRemanentData);

uint32_t App_SysPkt_ChannelInit(int iChannelIdx);
uint32_t App_SysPkt_DeleteConfig(int iChannelIdx);

uint32_t App_SysPkt_SetWd(uint16_t uiWdValue);

#endif /** COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SYSTEMPACKETS_H_ */
