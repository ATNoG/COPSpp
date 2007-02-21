/*
 * COPSHandle.h 
 *
 * Copyright (C) 2004 Universidade Aveiro - Instituto de Telecomunicacoes Polo Aveiro 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:	Diogo Gomes, <dgomes@av.it.pt>
 */

#ifndef _COPSHANDLE_H_
#define _COPSHANDLE_H_

#include "COPSObj.h"

//! 2.2.1 Handle Object
/** 
 *
 * The Handle Object encapsulates a unique value that identifies an
 * installed state. This identification is used by most COPS operations.
 * A state corresponding to a handle MUST be explicitly deleted when it
 * is no longer applicable. See Section 2.4 for details.
 *\code
 *	- C-Num = 1
 *\endcode
 *\code
 *      - C-Type = 1, Client Handle.
 *\endcode
 * Variable-length field, no implied format other than it is unique from
 * other client handles from the same PEP (a.k.a. COPS TCP connection)
 * for a particular client-type. It is always initially chosen by the
 * PEP and then deleted by the PEP when no longer applicable. The client
 * handle is used to refer to a request state initiated by a particular
 * PEP and installed at the PDP for a client-type. A PEP will specify a
 * client handle in its Request messages, Report messages and Delete
 * messages sent to the PDP. In all cases, the client handle is used to
 * uniquely identify a particular PEP's request for a client-type.
 *
 * The client handle value is set by the PEP and is opaque to the PDP.
 * The PDP simply performs a byte-wise comparison on the value in this
 * object with respect to the handle object values of other currently
 * installed requests.
 */
class COPSHandle : public COPSObj
{
	public:
		/**
		 * Creates the desired handle object
		 * @param h handle
		 */
		COPSHandle(unsigned int h);
		COPSHandle(const COPSHandle &);
		/**
		 * Copy operator
		 */
		COPSHandle &operator=(const COPSHandle &);
		~COPSHandle();
		/**
		 * Returns the Handle
		 */
		unsigned int getHandle() {return handle;};
	
	protected:
		unsigned int handle;
};


#endif	//_COPSHANDLE_H_
