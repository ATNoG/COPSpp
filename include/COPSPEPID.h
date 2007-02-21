/*
 * COPSPEPID.h 
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

#ifndef _COPSPEPID_H_
#define _COPSPEPID_H_

#include "COPSObj.h"

/**
2.2.11 PEP Identification Object (PEPID)

   The PEP Identification Object is used to identify the PEP client to
   the remote PDP. It is required for Client-Open messages.
\code
           C-Num = 11, C-Type = 1
\endcode
   Variable-length field. It is a NULL terminated ASCII string that is
   also zero padded to a 32-bit word boundary (so the object length is a
   multiple of 4 octets). The PEPID MUST contain an ASCII string that
   uniquely identifies the PEP within the policy domain in a manner that
   is persistent across PEP reboots. For example, it may be the PEP's
   statically assigned IP address or DNS name. This identifier may
   safely be used by a PDP as a handle for identifying the PEP in its
   policy rules.
 */
class COPSPEPID : public COPSObj
{
	public:
		COPSPEPID();
		/**
		 * Identification of the PEP
		 * @param identifier this is a std::string containing usually the name of the host where the pep is running
		 */
		COPSPEPID(std::string identifier);
		COPSPEPID(const COPSPEPID &);
		COPSPEPID &operator=(const COPSPEPID &);
		~COPSPEPID();
		/**
		 * Retrieves the identification string from the COPSPEPID
		 * @return string containing the indentification of the PEP
		 */
		std::string getId() {return id;};
	
	protected:
		std::string id;
	
};


#endif	//_COPSPEPID_H_
