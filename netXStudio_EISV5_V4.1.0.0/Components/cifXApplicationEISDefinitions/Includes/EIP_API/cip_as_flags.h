/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: eip_eif_packetdefinitions_assembly.h 73908 2018-01-30 09:15:51Z MarcBommert $:

Description:
 This header defines flags which are used in the configuration of assembly instances.
 They control the behavior of an assembly towards the bus and towards the user.
 We distinguish type flags to determine the type of an assembly instance and option
 flags which optionally allow for control of the assembly instance's behavior.
 The definitions in this header contribute to the LFW API of the EtherNet/IP stack and
 are applicable to the DPM packet interface.

**************************************************************************************/


#ifndef _CIP_AS_FLAGS_H_
#define _CIP_AS_FLAGS_H_

/* Assembly configuration flags */

#define EIP_AS_TYPE_OUTPUT                 0x00000000  /*!< Assembly type: This flag decides whether the newly registered assembly instance is
                                                            a pure output assembly which will only transmit data towards the network. Assemblies not
                                                            explicitly flagged with a assembly type flag are treated as output assemblies.
                                                            Input and output assemblies are also referred to as input and output connection points,
                                                            respectively. Connection points of types input and output will automatically be mapped
                                                            into the DPM input and output data areas, respectively. */
#define EIP_AS_TYPE_INPUT                  0x00000001  /*!< Assembly type: This flag decides whether the newly registered assembly instance is
                                                            a pure input assembly which will only receive data from the network. Assemblies not
                                                            explicitly flagged with a assembly type flag are treated as output assemblies exclusively
                                                            transmitting data towards the network. Input and output assemblies are also referred to as
                                                            input and output connection points, respectively. Connection points of types input and output
                                                            will automatically be mapped into the DPM input and output data areas, respectively. */

#define EIP_AS_TYPE_CONFIG                 0x00000020  /*!< Assembly type: This flag is used to declare this assembly a configuration assembly. */
#define EIP_AS_TYPE_INPUTONLY              0x40000000  /*!< Assembly type: Setting this flag declares an assembly of type "input only" */
#define EIP_AS_TYPE_LISTENONLY             0x80000000  /*!< Assembly type: Setting this flag declares an assembly of type "listen only" */

#define EIP_AS_OPTION_NO_RUNIDLE           0x00000008  /*!< Assembly option: If set, the assembly data is considered modeless (i.e. it does not
                                                            contain run/idle information). This parameter has to be consistent with your device's EDS. */

#define EIP_AS_OPTION_MAP_RUNIDLE          0x00000100  /*!< Assembly option: For input assemblies that receive the run/idle header,
                                                            this flag decides whether the run/idle header shall remain in the IO data when
                                                            being written into the triple buffer or DPM. This way the host application
                                                            has the possibility to evaluate the run/idle information on its own.
                                                            If the bit is set the run/idle header will be part of the IO data image. */
#define EIP_AS_OPTION_MAP_SEQCOUNT         0x00000400  /*!< Assembly option: For input assemblies,
                                                            this flag decides whether the 2-byte sequence count field of the EtherNet/IP PDU
                                                            shall be mapped into the I/O area with each receive. This way the host application
                                                            has the possibility to detect which assemblies have been updated recently, if any telegrams
                                                            have been missed and to optionally interpolate between process data snapshots.
                                                            If the bit is set the sequence count field will be part of the input data
                                                            image. The sequence counter will wrap-around to zero each 65536 receives. */
#define EIP_AS_OPTION_INVISIBLE            0x00000200  /*!< Assembly option: This flag decides whether the assembly instance can be accessed
                                                            via explicit services from the network.
                                                            Flag is set: the assembly instance is visible.
                                                            Flag is not set: the assembly instance is invisible. */

#define EIP_AS_OPTION_RXTRIGGER            0x00000800  /*!< Assembly option: If this flag is set for an input connection point and the DPM handshake
                                                            mode was set to the EtherNet/IP-specific mode "Receive (RX) Triggered Handshake Mode",
                                                            then each receive of valid I/O process data telegrams via a class 1 exclusive owner
                                                            connection towards this connection point will toggle the DPM handshake bits, promptly
                                                            presenting the received data to the application. For further information on the handshake
                                                            configuration, see packet HIL_SET_TRIGGER_TYPE_REQ.*/

#define EIP_AS_OPTION_MAP_PRODUCING_FLAGS  0x00001000  /*!< Assembly option: For output (producing) assemblies,
                                                            this bit decides whether an additional 4-byte flag field is available preceding the assembly's output data.
                                                            The producing flag field offers the host application additional control related to the output data.
                                                            If this bit is set, the 4-byte flag field will be part of the output data image.
                                                            See for details on what functionality comes with this flag field. */

#define EIP_AS_OPTION_RESERVED             0x00000010  /*!< Reserved: The "new data" flag is used internally and must be set to 0. */
#define EIP_AS_OPTION_HOLDLASTSTATE        0x00000040  /*!< Assembly option: This flag decides whether the data that is mapped into the
                                                            corresponding DPM memory area is cleared upon closing of
                                                            the connection or whether the last sent/received data is left
                                                            unchanged in the memory.
                                                            If the bit is set, the data will be left unchanged. */
#define EIP_AS_OPTION_FIXEDSIZE            0x00000080  /*!< Assembly option: This flag decides whether the assembly instance allows
                                                            connections to be established with a smaller connection size than
                                                            specified for the assembly. If it is not set, any connection size up to the
                                                            specified size will be accepted.
                                                            This flag is not meaningful for assemblies of types input only, listen only
                                                            and configuration. */


/*  Assembly data layout depending on configuration:
 *
 * ---------------------------------------------------------------------------------------------------------------
 *  Data layout of PRODUCING assembly instances (CIP_AS_PARAM_TYPE_PRODUCER is set --> process data that is sent to the network ):
 *
 *
 *               |<------------------------------- .tData.ulSize ---------------------------->|
 *
 *               |-----------------|-----------------|-----------------|-------------------...|
 *               | Producing Flags | Run/Idle        | Sequence Count  |  Process data        |
 *               | (optional)      | (optional)      | (optional)      |                      |
 *               |-----------------|-----------------|-----------------|-------------------...|
 *  Byte-Offset  0                 4                 8                 16
 *
 *                Producing Flags:    Present, if EIP_AS_OPTION_MAP_PRODUCING_FLAGS is set in .tData.ulFlags
 *                Run/Idle:           Present, if EIP_AS_OPTION_MAP_RUNIDLE         is set in .tData.ulFlags
 *                Sequence Count:     Present, if EIP_AS_OPTION_MAP_SEQCOUNT        is set in .tData.ulFlags
 *
 *
 * ---------------------------------------------------------------------------------------------------------------
 *  Data layout of CONSUMING assembly instances (CIP_AS_PARAM_TYPE_CONSUMER is set --> process data that is received from the network ):
 *
 *
 *               |<------------- .tData.ulSize ---------------------------->|
 *
 *               |-----------------|-----------------|-------------------...|
 *               | Run/Idle        | Sequence Count  |  Process data        |
 *               | (optional)      | (optional)      |                      |
 *               |-----------------|-----------------|-------------------...|
 *  Byte-Offset  0                 4                 8
 *
 *                Run/Idle:           Present, if EIP_AS_OPTION_MAP_RUNIDLE  is set in .tData.ulFlags
 *                Sequence Count:     Present, if EIP_AS_OPTION_MAP_SEQCOUNT is set in .tData.ulFlags
 *
 */

#endif
