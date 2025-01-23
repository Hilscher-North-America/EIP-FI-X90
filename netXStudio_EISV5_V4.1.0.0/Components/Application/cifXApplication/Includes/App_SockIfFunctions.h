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

#ifndef COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SOCK_IF_FUNCTIONS_H_
#define COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SOCK_IF_FUNCTIONS_H_

/*****************************************************************************/
/*! General Inclusion Area                                                   */
/*****************************************************************************/
#include <stdint.h>

/*****************************************************************************/
/*! PUBLIC FUNCTION PROTOTYPES                                                      */
/*****************************************************************************/
uint32_t Sock_GetIfAddrs( APP_DATA_T* ptAppData );
void AppSock_InterfaceConfiguration( APP_DATA_T* ptAppData );

#endif /** COMPONENTS_CIFXAPPLICATION_INCLUDES_APP_SOCK_IF_FUNCTIONS_H_ */
