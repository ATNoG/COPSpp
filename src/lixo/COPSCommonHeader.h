/*
 * COPSCommonHeader.h 
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

#ifndef _COPSCOMMONHEADER_H_
#define _COPSCOMMONHEADER_H_

#include <netinet/in.h>
#include <string>

#ifdef DEBUG_MAX
#include <iostream>
using namespace std;
#endif

#define COPS_VERSION 1

/**
 * This struct defines the header common to every COPS message
 */
struct COPScommon_hdr {
	unsigned int flags:4;               //!< COPS Flags
	unsigned int version:4;             //!< COPS Version number
	unsigned int op_code:8;             //!< COPS Operation
	unsigned int client_type:16;        //!< COPS Client Type
	unsigned int message_len:32;        //!< COPS Message Lenght
};

class COPSmessage;

/**
 
  2.1 Common Header

   Each COPS message consists of the COPS header followed by a number of
   typed objects.
\code
            0              1              2              3
     +--------------+--------------+--------------+--------------+
     |Version| Flags|    Op Code   |       Client-type           |
     +--------------+--------------+--------------+--------------+
     |                      Message Length                       |
     +--------------+--------------+--------------+--------------+
\endcode
     Global note: "----" implies field is reserved, set to 0.

       The fields in the header are:

         Version: 4 bits
             COPS version number. Current version is 1.

         Flags: 4 bits
             Defined flag values (all other flags MUST be set to 0):
               0x1 Solicited Message Flag Bit
                This flag is set when the message is solicited by another COPS message. This flag is NOT to be set (value=0) unless otherwise specified in section 3.

         Op Code: 8 bits
            The COPS operations:
\code
              1 = Request                 (REQ)
              2 = Decision                (DEC)
              3 = Report State            (RPT)
              4 = Delete Request State    (DRQ)
              5 = Synchronize State Req   (SSQ)
              6 = Client-Open             (OPN)
              7 = Client-Accept           (CAT)
              8 = Client-Close            (CC)
              9 = Keep-Alive              (KA)
              10= Synchronize Complete    (SSC)
\endcode
       Client-type: 16 bits

        The Client-type identifies the policy client. Interpretation of all encapsulated objects is relative to the client-type. Client- types that set the most significant bit in the client-type field are enterprise specific (these are client-types 0x8000 - 0xFFFF). (See the specific client usage documents for particular client-type IDs). For KA Messages, the client-type in the header MUST always be set to 0 as the KA is used for connection verification (not per client session verification).

        Message Length: 32 bits
        Size of message in octets, which includes the standard COPS header and all encapsulated objects. Messages MUST be aligned on 4 octet intervals.
 */
class COPSCommonHeader
{
	
	public:
		/**
		 * The Flags
		 */
		enum Flags {
			UNSOLICITED = 0x0,
			SOLICITED = 0x1		
		};
		/** 
		 * The COPS Operations
		 */
		enum OpCode {
			INVALID = 0,				//!< This code does not exist in the RFC and is only for internal exceptions
			REQUEST = 1,				//!< REQ
			DECISION = 2,				//!< DEC
			REPORT_STATE = 3,			//!< RPT
			DELETE_REQUEST_STATE = 4,	//!< DRQ
			SYNCHRONIZE_STATE_REQ = 5,	//!< SSQ
			CLIENT_OPEN = 6,			//!< OPN
			CLIENT_ACCEPT = 7,			//!< CAT
			CLIENT_CLOSE = 8,			//!< CC
			KEEP_ALIVE = 9,				//!< KA
			SYNCHRONIZE_COMPLETE = 10 	//!< SSN
		};
	
		COPSCommonHeader();
		/**
		 * Constructor
		 * This constructor should not be need...
		 * @param data byte aligned information that will be placed imediatly after the COPSCommonHeader
		 */
		COPSCommonHeader(char *data);
		COPSCommonHeader(const COPSCommonHeader &hdr);
		/**
		 * Constructor
		 * Creates a COPSCommonHeader with apropriate OpCode, Client Type and Flags
		 * @param code should be choose accordingly
		 * @param clientType shoud indicate the type of client (IANA defined)
		 * @param solicited since the only defined flag is solicited message, it is set by default
		 */
		COPSCommonHeader(OpCode code, unsigned int clientType, bool solicited = true);
		 ~COPSCommonHeader();

		COPSCommonHeader &operator=(const COPSCommonHeader &hdr);
		/**
		 * Sanity check
		 * will test the coherance of the values provided from the network
		 */
		bool valid();
		/**
		 * Prodived a pointer to the information that follows the header. Use together with message_len() to know the size of the char * returned
		 * @return pointer to information
		 */
		char *data() {return _data;};
		/**
		 * Lenght of the COPS message
		 * @return size in bytes
		 */
		unsigned int message_len();
		OpCode getOpCode();
		bool isUnsolicited();
		unsigned int getClientT();
	
		/**
		 * This operator provides the means to create a message by "+" to the COPSmessage
		 */
		friend COPSmessage &operator+(COPSmessage &c, const COPSCommonHeader &hdr);
		
	protected:
		char *_data;
		unsigned size;			

		friend class COPSmessage;
		
};


#endif	//_COPSCOMMONHEADER_H_
