/**************************************************************************************
Exclusion of Liability for this demo software:
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

#ifndef COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_FILESYSTEMPACKETS_H_
#define COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_FILESYSTEMPACKETS_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/

/*****************************************************************************/
/*! FORWARD DECLARATIONS                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*! FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/

/* File System */
uint32_t App_SysPkt_IsFormatted(int iChannelIdx);
uint32_t App_SysPkt_FileSystemInit(int iChannelIdx, bool fFormat);
uint32_t App_SysPkt_FileSystemList(int iChannelIdx);
uint32_t App_SysPkt_FileSystemRead(int iChannelIdx, uint8_t* pbData, uint32_t* pulDataSize);
uint32_t App_SysPkt_FileSystemWrite(int iChannelIdx, uint8_t* pbData, uint32_t ulDataSize);
uint32_t App_SysPkt_FileSystemDelete(int iChannelIdx);

#endif /** COMPONENTS_CIFXAPPLICATIONDEMO_INCLUDES_APP_FILESYSTEMPACKETS_H_ */
