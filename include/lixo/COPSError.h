/* 
 * COPSError.h
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

#ifndef _COPSERROR_H_
#define _COPSERROR_H_

#include <Copspp/COPSObj.h>

class COPSError : public COPSObj
{
	public:
		enum Error_Code {
			BadHandle = 1,
			InvalidHandleReference = 2,
			BadMessageFormat = 3,
			UnableToProcess = 4,			// Server gives up on query
			MandatoryClientSpecificInfoMissing = 5,
			UnsupportedClienType = 6,
			MandatoryCOPSObjectMissing = 7,
			ClientFailure = 8,
			CommunicationFailure = 9,
			Unspecified = 10,
			ShuttingDown = 11,
			RedirectPreferredServer = 12,
			UnknownCOPSObject = 13,			//Sub-code octet 2 contains unknown objects C-Num and octet 3 contains unknown object's C-Type
			AuthenticationFailure = 14,
			AuthenticationRequired = 15
		};
		COPSError(Error_Code code, unsigned short int sub_code=0);
		COPSError(const COPSError &);
		COPSError &operator=(const COPSError &);
		~COPSError();
		COPSError::Error_Code code();
	protected:
		Error_Code err;
		unsigned short int sub_err;
	
};
std::string strErrorCode(COPSError::Error_Code);

#endif	//_COPSERROR_H_
