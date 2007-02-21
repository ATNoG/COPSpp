/*
 * COPSError.h
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

#ifndef _COPSERROR_H_
#define _COPSERROR_H_

#include "COPSObj.h"

/**
 
  2.2.8 Error Object (Error)

  This object is used to identify a particular COPS protocol error.  The error sub-code field contains additional detailed client specific error codes. The appropriate Error Sub-codes for a particular client-type SHOULD be specified in the relevant COPS extensions document.

\code
             C-Num = 8, C-Type = 1
	     
	     0             1              2             3
	     +--------------+--------------+--------------+--------------+
	     |          Error-Code         |        Error Sub-code       |
	     +--------------+--------------+--------------+--------------+

	     Error-Code:
	     1 = Bad handle
	     2 = Invalid handle reference
	     3 = Bad message format (Malformed Message)
	     4 = Unable to process (server gives up on query) 
	     5 = Mandatory client-specific info missing
	     6 = Unsupported client-type
	     7 = Mandatory COPS object missing
	     8 = Client Failure
	     9 = Communication Failure
	     10= Unspecified
	     11= Shutting down
	     12= Redirect to Preferred Server
	     13= Unknown COPS Object:
	         Sub-code (octet 2) contains unknown object's C-Num
	          and (octet 3) contains unknown object's C-Type.
	     14= Authentication Failure
	     15= Authentication Required
 \endcode 
 */
class COPSError : public COPSObj
{
	public:
		enum Error_Code {
			BadHandle = 1,
			InvalidHandleReference = 2,
			BadMessageFormat = 3,
			UnableToProcess = 4,			//!<	Server gives up on query
			MandatoryClientSpecificInfoMissing = 5,
			UnsupportedClienType = 6,
			MandatoryCOPSObjectMissing = 7,
			ClientFailure = 8,
			CommunicationFailure = 9,
			Unspecified = 10,
			ShuttingDown = 11,
			RedirectPreferredServer = 12,
			UnknownCOPSObject = 13,			//!<	Sub-code octet 2 contains unknown objects C-Num and octet 3 contains unknown object's C-Type
			AuthenticationFailure = 14,
			AuthenticationRequired = 15,
			ObjectNotFound = 16
		};
		/**
		 * Constructor
		 * @param code one of the available errors
		 * @param sub_code in case of Unknown COPS Object this param should be used to define the error.
		 */
		COPSError(Error_Code code, unsigned short int sub_code=0);
		COPSError(const COPSError &);
		COPSError &operator=(const COPSError &);
		~COPSError();
		/**
		 * retrieves the Error Code from the COPS Object
		 * return Error_Code the error code in the Object
		 */
		COPSError::Error_Code code();
	protected:
		Error_Code err;
		unsigned short int sub_err;
	
};
std::string strErrorCode(COPSError::Error_Code);

#endif	//_COPSERROR_H_
