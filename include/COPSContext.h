/*
 * COPSContext.h 
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

#ifndef _COPSCONTEXT_H_
#define _COPSCONTEXT_H_

#include "COPSObj.h"

struct COPScontext_data {
	unsigned int r_type:16;
	unsigned int m_type:16;
};

//! 2.2.2 Context Object (Context)
/**
 *
 * Specifies the type of event(s) that triggered the query. Required for
 * request messages. Admission control, resource allocation, and
 * forwarding requests are all amenable to client-types that outsource
 * their decision making facility to the PDP. For applicable client-
 * types a PEP can also make a request to receive named configuration
 * information from the PDP. This named configuration data may be in a
 * form useful for setting system attributes on a PEP, or it may be in
 * the form of policy rules that are to be directly verified by the PEP.
 *
 * Multiple flags can be set for the same request. This is only allowed,
 * however, if the set of client specific information in the combined
 * request is identical to the client specific information that would be
 * specified if individual requests were made for each specified flag.
*\code
 *        - C-num = 2, C-Type = 1
*
 *            0             1               2               3
 *     +--------------+--------------+--------------+--------------+
 *     |            R-Type           |            M-Type           |
 *     +--------------+--------------+--------------+--------------+
* 
 *         R-Type (Request Type Flag)
*
 *           - 0x01 = Incoming-Message/Admission Control request
 *           - 0x02 = Resource-Allocation request
 *           - 0x04 = Outgoing-Message request
 *           - 0x08 = Configuration request
*
 *         M-Type (Message Type)
*
 *           - Client Specific 16 bit values of protocol message types
 * \endcode
 */

class COPSContext : public COPSObj
{
	public:
		enum R_Type {
			IncomingMessage = 0x01,			// Incoming-Message/Admission Control request
			ResourceAllocation = 0x02,		// Resource Allocation request
			OutgoingMessage = 0x04,			// OutgoingMessage request
			ConfigurationRequest = 0x08		// Configuration request
		};
		COPSContext(unsigned short int r_type,unsigned short int m_type=0);
		COPSContext(const COPSContext &);
		COPSContext &operator=(const COPSContext &);
		~COPSContext();
		COPSContext::R_Type getRType() const;
		unsigned short int getMType() const;
		bool operator==(COPSContext::R_Type type);

	
	protected:
		R_Type r_type;
		unsigned short int m_type;
};


#endif	//_COPSCONTEXT_H_
