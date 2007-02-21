/* 
 * COPSCommonHeader.h
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

#ifndef _COPSCOMMONHEADER_H_
#define _COPSCOMMONHEADER_H_

#include <netinet/in.h>
#include <string>

#define COPS_VERSION 1

struct COPScommon_hdr {
	unsigned int flags:4;               // COPS Flags
	unsigned int version:4;             // COPS Version number
	unsigned int op_code:8;             // COPS Operation
	unsigned int client_type:16;        // COPS Client Type
	unsigned int message_len:32;        // COPS Message Lenght
};

class COPSmessage;

class COPSCommonHeader
{
	
	public:
		enum Flags {
			UNSOLICITED = 0x0,
			SOLICITED = 0x1		
		};
		
		enum OpCode {
			INVALID = 0,				// This code does not exist in the RFC and is only for internal exceptions
			REQUEST = 1,				// REQ
			DECISION = 2,				// DEC
			REPORT_STATE = 3,			// RPT
			DELETE_REQUEST_STATE = 4,	// DRQ
			SYNCHRONIZE_STATE_REQ = 5,	// SSQ
			CLIENT_OPEN = 6,			// OPN
			CLIENT_ACCEPT = 7,			// CAT
			CLIENT_CLOSE = 8,			// CC
			KEEP_ALIVE = 9,				// KA
			SYNCHRONIZE_COMPLETE = 10 	// SSN
		};
	
		COPSCommonHeader();
		COPSCommonHeader(char *data);
		COPSCommonHeader(const COPSCommonHeader &hdr);
		COPSCommonHeader(OpCode code, unsigned int clientType, bool solicited = true);
		 ~COPSCommonHeader();

		COPSCommonHeader &operator=(const COPSCommonHeader &hdr);
		bool valid();
		char *data() {return _data;};
		unsigned int message_len();
		OpCode getOpCode();
		bool isUnsolicited();
		unsigned int getClientT();
		
		friend COPSmessage &operator+(COPSmessage &c, const COPSCommonHeader &hdr);
		
	protected:
		char *_data;
		unsigned size;			

		friend class COPSmessage;
		
};


#endif	//_COPSCOMMONHEADER_H_
