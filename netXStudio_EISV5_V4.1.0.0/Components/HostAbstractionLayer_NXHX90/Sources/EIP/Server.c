/* Includes */
#include <string.h>

#include "Server.h"

/*######################################################################*/
/* private definitions */

#define MAX_NET_HANDLERS 32

static handler_t     s_network_handlers[MAX_NET_HANDLERS];            /* handlers table */

/*######################################################################*/
/* public functions */

/*######################################################################*/
/* Register network handler */

int acyclic_reg_handler(
	data_type_t	data_type,  
	unsigned			addr,
	unsigned			qty,
	io_func		handler,
	void *				handler_arg,
	const char *		name
	)
	{
	unsigned slot ;
			
	/* find a free slot */
	for( slot = 0; slot < MAX_NET_HANDLERS; slot ++ ){
		if( s_network_handlers[slot].data_type == IND_EOF )
			goto _FillSlot;
	}
	
	//printf("\n%s::Error Locating Free Handler Slot for %u::%u!\n",SERVER_NAME,data_type,addr);
	return -1;
	
_FillSlot:
	/* fill in the slot */
	s_network_handlers[slot].data_type = data_type ;
	s_network_handlers[slot].handler = handler ;
	s_network_handlers[slot].arg = handler_arg ;
	strncpy( s_network_handlers[slot].name, name, sizeof(s_network_handlers[0].name)-1);
	s_network_handlers[slot].name[ sizeof(s_network_handlers[0].name)-1] = '\0';

	/* fill in the handler address range */
	s_network_handlers[slot].addr_first = addr ;
	s_network_handlers[slot].addr_last = (addr + qty - 1);
	return (int)slot;
}

/*######################################################################*/
/* Find network handler */
handler_t * acyclic_handler_find
	(
	op_type_t	opcode,
	data_type_t*	p_data_type,
	addr_t		addr		/* address of first unit */
	)
{
	unsigned ix ;
	
	for(ix = 0; ix < MAX_NET_HANDLERS; ix ++ )
	{
		if( s_network_handlers[ix].data_type == 0 /*EOF*/ )
			return 0 /* NULL */;
		/* BEWARE: negative logic is used here, most probably to fail first */
		if( s_network_handlers[ix].addr_first > addr )
			continue;
		if( s_network_handlers[ix].addr_last < addr )
			continue;

		/* Found our target */
		*p_data_type = s_network_handlers[ix].data_type;
		return (&s_network_handlers[ix]) ;
	}

#if 0 	
	printf("\n0x%X not found! (%d)\n",addr,data_type);
#endif
	return 0 /*NULL*/ ;
}
