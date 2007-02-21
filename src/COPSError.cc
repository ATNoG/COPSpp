/*
 * COPSError.cc 
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

#include "COPSError.h"

std::string strErrorCode(COPSError::Error_Code err) {
	switch (err) {
		case COPSError::BadHandle: return "Bad Handle";
		case COPSError::InvalidHandleReference: return "Invalid handle reference";
		case COPSError::BadMessageFormat: return "Bad Message Format (Malformed Message)";
		case COPSError::UnableToProcess: return "Unable to process (server gives up on query)";
		case COPSError::MandatoryClientSpecificInfoMissing: return "Mandatory client-specific info missing";
		case COPSError::UnsupportedClienType: return "Unsupported client-type";
		case COPSError::MandatoryCOPSObjectMissing: return "Mandatory COPS object missing";
		case COPSError::ClientFailure: return "Client Failure";
		case COPSError::CommunicationFailure: return "Communication Failure";
		case COPSError::Unspecified: return "Unspecified";
		case COPSError::ShuttingDown: return "Shutting Down";
		case COPSError::RedirectPreferredServer: return "Redirect to Preferred Server";
		case COPSError::UnknownCOPSObject: return "Unknown COPS Object";
		case COPSError::AuthenticationFailure: return "Authentication Failure";
		case COPSError::AuthenticationRequired: return "Authentication Required";
		case COPSError::ObjectNotFound: return "Requested Object could not be found in the message";

	};
	return "Unspecified";
}

COPSError::COPSError(const COPSError &obj) : COPSObj(obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSError::COPSError(const COPSError &obj)" << endl;
	#endif
	err = obj.err;
	sub_err = obj.sub_err;
}

COPSError &COPSError::operator=(const COPSError &obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSError &COPSError::operator=(const COPSError &obj)" << endl;
	#endif
	COPSObj::operator=(obj);
	err = obj.err;
	sub_err = obj.sub_err;
	return *this;
}

COPSError::COPSError(Error_Code code, unsigned short int sub_code) : COPSObj()
{
	#ifdef DEBUG_MAX
	cerr << "COPSError::COPSError(Error_Code code, unsigned short int sub_code)" << endl;
	#endif
	
	size = calculate_padding(sizeof(struct COPSobj_data)+4);  //obj hdr + 4
	err = code;
	sub_err = sub_code;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Error;
	obj->c_type = 1;
	obj->obj_len = htons(size);
	
	unsigned short int nError;
	unsigned short int nSubError;
	nError= htons(err);
	nSubError= htons(sub_err);
	memcpy(&data[4],&nError,sizeof(err));
	memcpy(&data[6],&nSubError,sizeof(sub_err));
}

COPSError::Error_Code COPSError::code() 
{
	return err;
}

COPSError::~COPSError()
{
	// TODO: put destructor code here
}
