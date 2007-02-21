/* 
 * COPSContext.h
 *
 * Copyright (C) 2004 Universidade Aveiro - Instituto de Telecomunicacoes Polo A
veiro
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
 * Authors:     Diogo Gomes, <dgomes@av.it.pt>
 */

#ifndef _COPSCONTEXT_H_
#define _COPSCONTEXT_H_

#include <Copspp/COPSObj.h>

struct COPScontext_data {
	unsigned int r_type:16;
	unsigned int m_type:16;
};

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
