/* 
 * COPSReason.h
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

#ifndef _COPSREASON_H_
#define _COPSREASON_H_

#include <Copspp/COPSObj.h>

class COPSReason : public COPSObj
{
	public:
		enum Reason_Code {
			Unspecified = 1,
			Management = 2,
			Preempted = 3, 	// Another request state takes precedence
			Tear = 4, 		// Used to communicate a signaled state removal
			Timeout = 5, 	// Local State has timed-out
			RouteChange = 6, 	// Change invalidates request state
			InsufficientResources = 7,  //No local resource available
			PDPDirective = 8, 	//PDP decision caused the delete
			UnsupportedDecision = 9, //PDP decision not supported
			SynchronizeHandleUnknown = 10,
			TransientHandle = 11, //stateless event
			MalformedDecision = 12, 	//could not recover
			UnknownCOPSObjectFromPDP = 13 //Sub-code (octet 2) contains unknown object's C-Num and octet 3 contains unknown objects C-Type
		};
		COPSReason(Reason_Code code, unsigned int subcode=0);
		COPSReason(const COPSReason &);
		COPSReason &operator=(const COPSReason &);
		~COPSReason();
		Reason_Code getCode() { return code; };
	
	protected:
		Reason_Code code;
		unsigned short int sub_code;
	
};


#endif	//_COPSREASON_H_
