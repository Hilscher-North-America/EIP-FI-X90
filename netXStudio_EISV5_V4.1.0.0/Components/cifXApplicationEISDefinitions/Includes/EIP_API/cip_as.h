/**************************************************************************************
Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.
***************************************************************************************
$Id: cip_as.h 111376 2023-01-12 22:00:06Z OKompani $:

Description:
  EtherNet/IP - Assembly Object
**************************************************************************************/

#ifndef CIP_AS_H
#define CIP_AS_H

#include <stdint.h>
#include <stdbool.h>

#include "Hil_Compiler.h"
#include "Hil_Results.h"
#include "cip_common.h"

  /*#####################################################################################*/

  /*
    ************************************************************
    *  Object specific defines
    ************************************************************
    */

  #define CIP_AS_CLASS_NUMBER       0x04
  #define CIP_AS_REVISION           0x02
  #define CIP_AS_MAX_INSTANCE       0xFFFFF

  /* attributes of identity object */
  #define CIP_AS_ATTR_1_NUM_MEMBER                  1
  #define CIP_AS_ATTR_2_MEMBER_LIST                 2
  #define CIP_AS_ATTR_3_DATA                        3
  #define CIP_AS_ATTR_4_SIZE                        4

  #define CIP_AS_ATTR_769_PARAMETER                 769 /* 0x301 */
  #define CIP_AS_ATTR_770_STATUS                    770 /* 0x302 */

  #define CIP_AS_MAX_NUM_OF_INSTANCE_ATTR           6

  #define CIP_AS_MAX_MEMBER_PATH_SIZE               9 /* 3 segments a 9 bytes (support of 16bit segments) */

  /* Hilscher services specific to the Assembly Object */
  #define CIPHIL_CMD_AS_CREATE                      0x0401
  #define CIPHIL_CMD_AS_DELETE                      0x0402
  #define CIPHIL_CMD_AS_ADD_MEMBER                  0x0403

  #define CIPHIL_CMD_AS_REMOVE_MEMBER               0x0404
  #define CIPHIL_CMD_AS_UPDATE_CONSUMING            0x0405   /* update consuming data to host application  */

  #define CIPHIL_CMD_AS_UPDATE_PRODUCING            0x0406   /* update producing data to host application  */
  #define CIPHIL_CMD_AS_GET_CONSUMING               0x0407   /* get consuming data from bus                */
  #define CIPHIL_CMD_AS_BIND                        0x0408   /* bind assembly to a connection              */
  #define CIPHIL_CMD_AS_UNBIND                      0x0409   /* unbind assembly to a connection            */
  #define CIPHIL_CMD_AS_CHANGE_PRODUCER_RUNIDLE     0x040A   /* Change state of assembly to RUN or IDLE    */
  #define CIPHIL_CMD_AS_UPDATE_CONSUMING_RXTRIGGER  0x040B   /* update consuming data to host application on next data receive */
  #define CIPHIL_CMD_AS_CANCEL_PENDING_RXTRIGGER    0x040D   /* invoke/resolve the potentially pending RX trigger (triggering handshake flag togglign back to host) */

  #define CIPHIL_CMD_AS_ENABLE_RXTRIGGER            0x040E
  #define CIPHIL_CMD_AS_DISABLE_RXTRIGGER           0x040F

  #define CIPHIL_CMD_AS_GET_RUNIDLE_STATE           0x0410   /* Gets the Run/Idle state of an assembly instance */
  #define CIPHIL_CMD_AS_VALIDATE_3                  0x0411   /* Perform a set of attribute 3 without actually setting the data (just verify) */
  #define CIPHIL_CMD_AS_GET_CONNECTION_PARAMETERS   0x0412   /* Get conenction parameters constant as specified in the creation of the assembly */


  #define CIP_AS_PARAM_TYPE_MSK                     0xF000   /* Assembly type bitmask */
  #define CIP_AS_PARAM_TYPE_CONSUMER                0x0000   /* Type Consumer      - Consumes data from the bus */
  #define CIP_AS_PARAM_TYPE_PRODUCER                0x1000   /* Type Producer      - Produces data towards the bus */
  #define CIP_AS_PARAM_TYPE_HB_LISTENONLY           0x2000   /* Type Listen-only   - Monitors data of established connection */
  #define CIP_AS_PARAM_TYPE_HB_INPUTONLY            0x4000   /* Type Input-only    - Provides read access of produced data */
  #define CIP_AS_PARAM_TYPE_CONFIG                  0x8000   /* Type Configuration - Processes configuration data provided in connection path */

  #define CIP_AS_PARAM_TRIG_MODE_MSK                0x0001   /* Assembly trigger mode bitmask */
  #define CIP_AS_PARAM_TRIG_MODE_NONE               0x0000   /* Trigger mode: None - Data is provided to the application synchronously on request */
  #define CIP_AS_PARAM_TRIG_MODE_RXTRIGGER          0x0001   /* Trigger mode: RX   - Data is provided to the application asynchronously on reception */

  #define CIP_AS_PARAM_RETRIEVE_MODE_MSK            0x000E   /* Assembly retrieval mode bitmask */
  #define CIP_AS_PARAM_RETRIEVE_MODE_PURE           0x0000   /* Retrieval mode: Pure I/O data - Only raw process data is presented to the user */
  #define CIP_AS_PARAM_RETRIEVE_MODE_HOLDLASTSTATE  0x0002   /* Retrieval mode: Do not clear I/O input data when the connection is closed/run-bit not set */
  #define CIP_AS_PARAM_RETRIEVE_MODE_RUNIDLE        0x0004   /* Retrieval mode: Run/Idle Header included - 4-byte run/idle header precedes the data (if received). Can be combined with CIP_AS_PARAM_RETRIEVE_MODE_SEQCOUNT */
  #define CIP_AS_PARAM_RETRIEVE_MODE_SEQCOUNT       0x0008   /* Retrieval mode: Data sequence count field included - 4-bytes preceding the data whose lower significant two bytes contain the data's sequence count value. Can be combined with CIP_AS_PARAM_RETRIEVE_MODE_RUNIDLE */

  #define CIP_AS_PARAM_RT_FORMAT_MSK                0x0F00   /* Expected class connection realtime data format */
  #define CIP_AS_PARAM_RT_FORMAT_PURE               0x0000   /* RT format: pure     - Data is modeless */
  #define CIP_AS_PARAM_RT_FORMAT_NULL               0x0100   /* not implemented */
  #define CIP_AS_PARAM_RT_FORMAT_HB                 0x0200   /* not implemented */
  #define CIP_AS_PARAM_RT_FORMAT_RUNIDLE            0x0400   /* RT format: run/idle - Data contains a 4-byte run-idle header */

  #define CIP_AS_ACCESS_PERMISSION_MSK              0x0010   /* Assembly access permission */
  #define CIP_AS_ACCESS_PERMISSION_UNRESTRICTED     0x0000   /* No access restrictions */
  #define CIP_AS_ACCESS_PERMISSION_NOEXPLICITBUS    0x0010   /* Reject explicit access from the network (only implicit access permitted) */

  #define CIP_AS_PARAM_OPTION_MAP_FLAGS_MSK         0x0020   /* Option map bit mask */
  #define CIP_AS_PARAM_OPTION_MAP_FLAGS_NONE        0x0000   /* Option map flags: no additional option field will be added to the assembly's data */
  #define CIP_AS_PARAM_OPTION_MAP_FLAGS_PRODUCING   0x0020   /* Option map flags: producing flag field (4 bytes) precedes the data. */

  /* Definition of the Producing flag field:
   *
   * This flag field (if enabled) belongs to the producing assembly data image. It must be updated by the application together with the assembly's process data.
   * It enables the application to control certain things related to the assembly data itself or
   * the way the assembly data gets processed within the protocol stack.
   *
   * Bit number 0:    Controls whether or not the process data frame shall be updated with the provided assembly data.
   *
   *                  For I/O connections of type "cyclic":
   *
   *                      Value 0: the assembly is updated with the new producing data,
   *                               but the data will not be copied into the process data frame.
   *                               This means the data will not be sent to the originator of the connection.
   *                      Value 1: the assembly is updated with the new producing data,
   *                               and the data will be copied into the process data frame.
   *
   *                            Note: for I/O connections of type "cyclic" this bit should be set to value 1 all the time.
   *
   *                  For I/O connections of type "application triggered":
   *                      By using this bit, the application can control whether or not the protocol
   *                      stack shall send a new process data frame on the network.
   *                      Usually, when running an application controlled connection, each update of the
   *                      assembly's process data will trigger a new process data frame on the network.
   *                      However, there might be applications that do not want to trigger a process data frame on every update (e.g. CIP Safety).
   *
   *                      Value 0: the assembly is updated with the new producing data,
   *                               but the data will not be copied into the process data frame.
   *                               No process data frame will be sent.
   *                      Value 1: the assembly is updated with the new producing data,
   *                               and the data will be copied into the process data frame.
   *                               Process data frame will be sent right away.
   *
   * Bit 1-31: Reserved for future use.
   * */

  /* Producing flag field bits: */
  #define CIP_AS_PRODUCING_FLAG_TRIGGER_PROCESS_DATA_UPDATE   0x00000001    /* Bit number 0 */


  #define CIP_AS_PARAM_MASK                         (CIP_AS_PARAM_TYPE_MSK | CIP_AS_PARAM_TRIG_MODE_MSK | CIP_AS_PARAM_RETRIEVE_MODE_MSK | CIP_AS_PARAM_RT_FORMAT_MSK | CIP_AS_ACCESS_PERMISSION_MSK | CIP_AS_PARAM_OPTION_MAP_FLAGS_MSK)

  #define CIP_AS_STATUS_CONNECTED                   0x0001
  #define CIP_AS_STATUS_RUN                         0x0002

  #define CIPAS_INSTANCE_STATUS_ISCONNECTED(usStatus)        (((usStatus) & CIP_AS_STATUS_CONNECTED) != 0)
  #define CIPAS_INSTANCE_STATUS_ISRUNNING(usStatus)          (((usStatus) & CIP_AS_STATUS_RUN) != 0)

  /* MAcros for convenient testing of assembly instance parameters */

  #define CIP_AS_IS_TYPE_CONSUMER(usParam)                   (((usParam) & CIP_AS_PARAM_TYPE_MSK) == CIP_AS_PARAM_TYPE_CONSUMER)
  #define CIP_AS_IS_TYPE_PRODUCER(usParam)                   (((usParam) & CIP_AS_PARAM_TYPE_MSK) == CIP_AS_PARAM_TYPE_PRODUCER)
  #define CIP_AS_IS_TYPE_HB_LISTENONLY(usParam)              (((usParam) & CIP_AS_PARAM_TYPE_MSK) == CIP_AS_PARAM_TYPE_HB_LISTENONLY)
  #define CIP_AS_IS_TYPE_HB_INPUTONLY(usParam)               (((usParam) & CIP_AS_PARAM_TYPE_MSK) == CIP_AS_PARAM_TYPE_HB_INPUTONLY)
  #define CIP_AS_IS_TYPE_CONFIG(usParam)                     (((usParam) & CIP_AS_PARAM_TYPE_MSK) == CIP_AS_PARAM_TYPE_CONFIG)

  #define CIP_AS_IS_TYPE_HB(usParam)                         (CIP_AS_IS_TYPE_HB_LISTENONLY(usParam) || CIP_AS_IS_TYPE_HB_INPUTONLY(usParam))

  #define CIP_AS_IS_TRIG_MODE_NONE(usParam)                  (((usParam) & CIP_AS_PARAM_TRIG_MODE_MSK) == CIP_AS_PARAM_TRIG_MODE_NONE)
  #define CIP_AS_IS_TRIG_MODE_RXTRIGGER(usParam)             (((usParam) & CIP_AS_PARAM_TRIG_MODE_MSK) == CIP_AS_PARAM_TRIG_MODE_RXTRIGGER)

  #define CIP_AS_IS_RETRIEVE_MODE_PURE(usParam)              (((usParam) & CIP_AS_PARAM_RETRIEVE_MODE_MSK) == CIP_AS_PARAM_RETRIEVE_MODE_PURE)
  #define CIP_AS_IS_RETRIEVE_MODE_HOLDLASTSTATE(usParam)     (((usParam) & CIP_AS_PARAM_RETRIEVE_MODE_MSK) & CIP_AS_PARAM_RETRIEVE_MODE_HOLDLASTSTATE)
  #define CIP_AS_IS_RETRIEVE_MODE_RUNIDLE(usParam)           (((usParam) & CIP_AS_PARAM_RETRIEVE_MODE_MSK) & CIP_AS_PARAM_RETRIEVE_MODE_RUNIDLE)
  #define CIP_AS_IS_RETRIEVE_MODE_SEQCOUNT(usParam)          (((usParam) & CIP_AS_PARAM_RETRIEVE_MODE_MSK) & CIP_AS_PARAM_RETRIEVE_MODE_SEQCOUNT)

  #define CIP_AS_IS_OPTION_MAP_FLAGS_PRODUCING(usParam)      (((usParam) & CIP_AS_PARAM_OPTION_MAP_FLAGS_MSK) & CIP_AS_PARAM_OPTION_MAP_FLAGS_PRODUCING)

  #define CIP_AS_IS_RT_FORMAT_PURE(usParam)                  (((usParam) & CIP_AS_PARAM_RT_FORMAT_MSK) == CIP_AS_PARAM_RT_FORMAT_PURE)
  #define CIP_AS_IS_RT_FORMAT_NULL(usParam)                  (((usParam) & CIP_AS_PARAM_RT_FORMAT_MSK) == CIP_AS_PARAM_RT_FORMAT_NULL)
  #define CIP_AS_IS_RT_FORMAT_HB(usParam)                    (((usParam) & CIP_AS_PARAM_RT_FORMAT_MSK) == CIP_AS_PARAM_RT_FORMAT_HB)
  #define CIP_AS_IS_RT_FORMAT_RUNIDLE(usParam)               (((usParam) & CIP_AS_PARAM_RT_FORMAT_MSK) == CIP_AS_PARAM_RT_FORMAT_RUNIDLE)

  #define CIP_AS_IS_ACCESS_PERMISSION_UNRESTRICTED(usParam)  (((usParam) & CIP_AS_ACCESS_PERMISSION_MSK) == CIP_AS_ACCESS_PERMISSION_UNRESTRICTED)
  #define CIP_AS_IS_ACCESS_PERMISSION_NOEXPLICITBUS(usParam) (((usParam) & CIP_AS_ACCESS_PERMISSION_MSK) == CIP_AS_ACCESS_PERMISSION_NOEXPLICITBUS)


  /* valid value ranges */

  /* Limitation to 504 bytes actual data size because of
   * regular ForwardOpen(511 bytes max)
   *   minus 4 bytes run/idle
   *   minus 2 bytes sequence count
   *   minus 1 to have a multiple of four
   */
  #define CIP_AS_DATA_SIZE_MAXIMUM_VALUE (504)

  /*#####################################################################################*/

  /*
    ************************************************************
    *  Object specific types
    ************************************************************
    */

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_PACK_1(CIP_AS)
#endif

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_RUNIDLE_REQ_Ttag
  {
    #define CIP_AS_IDLE 0x00
    #define CIP_AS_RUN  0x01

    uint8_t bRunIdle;

  } CIPHIL_AS_RUNIDLE_REQ_T;

  /*#####################################################################################*/

  struct EIP_RESOURCES_Ttag;  /* forward declaration */
  struct CIP_CC_RSC_Ttag;     /* forward declaration */

  typedef void (* CIP_AS_EVENT_UPDATE_CB)( struct EIP_RESOURCES_Ttag* hEip,
                                           uint32_t ulHandle,
                                           uint8_t* pbData,
                                           uint16_t usDataLen,
                                           uint32_t ulDataSeqCount);

  typedef void (* CIP_AS_EVENT_STATE_CB)( void* hHandle, bool fValid );

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_BIND_REQ_Ttag
  {
    uint32_t                    ulHandle; /* Handle to the connection */
    CIP_AS_EVENT_UPDATE_CB      fnUpdate; /* Update function for process data . Used only for producing assemblies. */
    uint16_t                    usParam;  /* CIP_AS_PARAM_TYPE_... */
    uint16_t                    usSize;   /* Connection size for the assembly as requested from the network (without sequence count size, but including run/idle size). */


    void (*fnCCValidDataSendOrReceivedBitfieldManipulate)(struct CIP_CC_RSC_Ttag* pvArg, uint32_t ulInstanceId, bool fSet);     /* Scanner only, set to zero otherwise.
                                                                                                                 For consuming/producing assemblies of originated connections:
                                                                                                                  Call the function when assembly switches between RUN and IDLE states.
                                                                                                               */
    void *pvCCValidDataSendOrReceivedBitfieldManipulateArg;            /* Scanner only, pvArg of teh above function */
    uint32_t ulCCInstanceId;                                           /* Scanner only: CC instance Id of the connection for which we are the consuming assembly */

  } CIPHIL_AS_BIND_REQ_T;

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_CREATE_REQ_Ttag
  {
    uint32_t ulInstanceId;
    uint16_t usMinSize;
    uint16_t usMaxSize;
    uint16_t usParam;
  } CIPHIL_AS_CREATE_REQ_T;

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_ADD_MEMBER_REQ_Ttag
  {
    uint16_t usDataSize;     /* size of the adressed attribute, TODO: We have this in bytes, specification says in bits, probably mistaken in the spec. Check other vendors */
    uint16_t usPathSize;     /* path size in [bytes] */
    uint8_t  abPath[CIP_AS_MAX_MEMBER_PATH_SIZE];  /* path to another assembly instance's attribute */
  } CIPHIL_AS_ADD_MEMBER_REQ_T;

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_DEL_MEMBER_REQ_Ttag
  {
    uint16_t usPathSize;
    uint8_t  abPath[CIP_AS_MAX_MEMBER_PATH_SIZE];
  } CIPHIL_AS_REMOVE_MEMBER_REQ_T;

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIP_AS_MEMBER_LIST_ENTRY_Ttag
  {
    uint16_t usDataSize;
    uint16_t usPathSize;
    uint8_t  abPath[CIP_AS_MAX_MEMBER_PATH_SIZE];
  } CIP_AS_MEMBER_LIST_ENTRY_T;

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_VALIDATE_3_REQ_Ttag
  {
    uint8_t *pbNewData;
    uint32_t ulNewDataLen;
  } CIPHIL_AS_VALIDATE_3_REQ_T;

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_VALIDATE_3_CNF_Ttag
  {
    bool fEffective;
  } CIPHIL_AS_VALIDATE_3_RES_T;

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_GET_CONSUMING_REQ_Ttag
  {
    uint8_t* pbData;
    uint16_t usDataLen;
    uint16_t usDataSeqCnt;
  } CIPHIL_AS_GET_CONSUMING_REQ_T;

  /*#####################################################################################*/

  typedef __HIL_PACKED_PRE struct __HIL_PACKED_POST CIPHIL_AS_GET_RUNIDLE_STATE_RES_Ttag
  {
    #define CIPHIL_AS_GET_RUNIDLE_STATE_RUN   0x01
    #define CIPHIL_AS_GET_RUNIDLE_STATE_IDLE  0x02

    uint8_t bState;

  } CIPHIL_AS_GET_RUNIDLE_STATE_RES_T;

#ifdef __HIL_PRAGMA_PACK_ENABLE
#pragma __HIL_PRAGMA_UNPACK_1(CIP_AS)
#endif

  typedef struct CIP_AS_INIT_PARAM_Ttag
  {
    uint16_t usMaxInstance;     /* maximal number of assemblies */
    uint16_t usMaxMembers;      /* maximal number of members (sum of all members) */
    uint16_t usMaxConnections;  /* maximal number of IO connections */
  } CIP_AS_INIT_PARAM_T;


  struct EIP_RESOURCES_Ttag;  /* forward declaration */

  uint32_t
  CipAs_ObjectCreate(struct EIP_RESOURCES_Ttag* hEip, void*  ptParam );

  bool
  CipAs_GetRunStatus(struct EIP_RESOURCES_Ttag* hEip, uint32_t  ulInstance );
#endif /* CIP_AS_H */
