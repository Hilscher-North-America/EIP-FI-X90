/*
 * Server.h
 *
 *  Created on: Feb 13, 2013
 *      Author: RobertsJ
 */

#ifndef SERVER_H_
#define SERVER_H_

/*! \file Server.h
 *
 *  \brief Header File for Server Library
 */

/*! \def EXCEPTION
 *  \brief Added to Function Code
 */
#define EXCEPTION  0x80

/*! \def EXC_ILL_FUNC
 *  \brief Illegal Function
 */
#define EXC_ILL_FUNC    1

/*! \def EXC_ILL_ADDR
 *  \brief Illegal Data Address
 */
#define EXC_ILL_ADDR    2

/*! \def EXC_ILL_VAL
 *  \brief Illegal Data Value
 */
#define EXC_ILL_VAL     3

/*! \def EXC_DEV_FAIL
 *  \brief  Unrecoverable failure
 */
#define EXC_DEV_FAIL    4

/*! \def EXC_ACK
 *  \brief Acknowledge start of long op
 */
#define EXC_ACK       5

/*! \def EXC_BUSY
 *  \brief Slave Device Busy in long op
 */
#define EXC_BUSY      6

/*! \def EXC_PARITY
 *  \brief Memory Parity Error
 */
#define EXC_PARITY    8

/*! \def EXC_GTW_PATH
 *  \brief Gateway Path Unavailable
 */
#define EXC_GTW_PATH 0x0a

/*! \def EXC_GTW_DEV
 *  \brief Gateway: no response from target
 */
#define EXC_GTW_DEV  0x0b

/*! \def EXC_GTW_ERR
 *  \brief Gateway: comms error. MKS Extension.
 */
#define EXC_GTW_ERR  0x0e

/*! \brief Enumeration of register data types, internally defined.
 *  \remarks Used by registration of a user callback function. Effectively informs the library
 *  which function code(s) will access the callback.
 *
 */
typedef enum
{
  IND_EOF = 0, /*!<End of Table indicator, invalid type */
  IND_IN = 4,  /*!<(R/O) 16-bit register */
  IND_OUT = 3, /*!<(R/W) 16-bit register */
  DISCRETE_OUT = 1,   /*!<(R/W) Discrete Output */
  DISCRETE_IN = 2,   /*!<(R/O) Discrete Input */
  TYPE_FIFO  = 5,   /*!<(R/W) FIFO Queue (FUTURE::NOT CURRENTLY SUPPORTED!) */
  TYPE_FILE  = 6    /*!<(R/W) File */
} data_type_t ;

/*! \enum op_type_t
 *
 * \brief Enumerated type use by library when acessing user callback handlers.
 * \remarks Used by callback handlers to determine the kind of access operation
 * the Server Library is requesting. This is especially relevant when the callback
 * handler is assigned to a data_type_t that supports both read and write operations
 */
typedef enum
{
  IO_GET,       /*!<Read event. */
  IO_SET,       /*!<Write event. */
  IO_LOCK,      /*!<Lock. Used for Write Mask func*/
  IO_UNLOCK     /*!<Unlock. Used for Write Mask func*/
} op_type_t ;

/*! \typedef unsigned short addr_t
 *
 * \brief unsigned short used for data address
 *
 */
typedef unsigned short addr_t ;

/*! \typedef int (* io_func)
 *
 * \brief Callback handler type defintion
 * \remarks Typically used to perform some additional handling
 * before the data buffer is accessed by the library such as to get
 * or set I/O
 *
 */
typedef int ( * io_func) (
  op_type_t  opcode,   /*!<op code*/
  data_type_t  data_type,  /*!<data type*/
  addr_t   offset,   /*!<data address of first unit*/
  unsigned      quantity, /*!<number of data units*/
  void *        data,   /*!<pointer to data buffer*/
  void *        arg     /*!<optional argument*/
  );

/*! \struct handler_t
 *
 *	\brief structure type used to register a callback and data buffer to
 *	the Server via reg_handler()
 *
 */
typedef struct
{
    data_type_t	data_type ;		/*!<type: input, coil, in-reg, h-reg*/
    addr_t		addr_first;		/*!<first data address (not a reference addr!)*/
    addr_t		addr_last;		/*!<last data address (not a reference addr!)*/
    io_func		handler;		/*!<callback to user support function*/
    void *				arg ;			/*!<optional argument to user support function*/
    char				name[32];		/*!<short string description for future use*/
} handler_t;

extern
#ifdef __cplusplus
"C"
#endif

/*!
 * \brief Register a new handler for data object
 *
 * \remarks The reg_handler() func is used to register handlers with the library.
 *	Example: reg_handler(INPUT, 0, 8, inpu_access_func, NULL, "Analog Inputs");
 *	NOTE 1: The Server library accesses discrete coils and inputs as defined by the 
 *  protocol on the wire; in bitmapped notation.
 *	Example: 16 channels contained in a character array buffer, where channels 0,1,3,14 and 15 are ON/Active. 
 *  data_buffer[0] = 0x0b; data_buffer[1] = 0xc0;
 *  NOTE 2:  The Server library accesses 16-bit registers as defined by the 
 *  protocol on the wire; in big endian notation.
 *
 * \param[in] data_type enumerated internal description of data type.
 * The data type should be one of the enumerated types of the type data_type_t
 * \see data_type_t 
 *
 * \param[in] addr
 * start data address of the register(s).
 * The addr is the data address in the protocol, not to be confused with a reference address. 
 * For example holding register 40001 (or using extended range: 400001; or also Modicon 984 notation: 4x00001 ) is located at the data address of 0000.
 * Valid ranges for this param are 0 to 65535. 
 *
 * \param[in] qty
 * number of registers/units.
 * This parameter has a valid range of 1 - 65535.
 *
 * \param[in] handler User callback function. 
 * The handler function will be called by the library when the data type and data address range
 * are contained in a request pdu. The handler must conform to the io_func type. 
 *	\see io_func
 *
 * \param[in] handler_arg
 * argument to pass to handler each time it is executed
 * 
 * \param[in] name
 * This is a future use field.
 * It is a string containing the name or description of the handlers purpose. 
 * Ex "Digital Inputs"
 *
 */
int acyclic_reg_handler(
	data_type_t		data_type,
	unsigned				addr,
	unsigned				qty,
	io_func			handler,
	void *					handler_arg,
	const char *			name
	);

 handler_t *	acyclic_handler_find	(op_type_t opcode, data_type_t* data_type, addr_t addr);

//(re)usable definition for simplifying prototypes
#define DECLARE_HANDLER(x) int x(op_type_t opcode, data_type_t data_type, addr_t offset, unsigned qty, void *data, void * arg)


#endif /* SERVER_H_ */
