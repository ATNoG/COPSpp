/*
 * COPSCommonHeader.cc 
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

#include <COPSCommonHeader.h>

COPSCommonHeader::COPSCommonHeader(OpCode code, unsigned int clientType, bool solicited)
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader::COPSCommonHeader(OpCode code, unsigned int clientType) " << this << endl;
#endif
	_data = new char [(sizeof(struct COPScommon_hdr))];
	//((struct COPScommon_hdr *)_data) = new char [(sizeof(struct COPScommon_hdr))];
	((struct COPScommon_hdr *)_data)->version = COPS_VERSION;
	if(solicited)
		((struct COPScommon_hdr *)_data)->flags = SOLICITED;
	else 
		((struct COPScommon_hdr *)_data)->flags = UNSOLICITED;
	((struct COPScommon_hdr *)_data)->op_code = code;
	((struct COPScommon_hdr *)_data)->client_type = htons(clientType);
	((struct COPScommon_hdr *)_data)->message_len = htonl(sizeof(struct COPScommon_hdr));
	size = sizeof(struct COPScommon_hdr);
}

unsigned int COPSCommonHeader::getClientT() 
{
	return ntohs(((struct COPScommon_hdr *) _data)->client_type);
}

COPSCommonHeader::OpCode COPSCommonHeader::getOpCode() 
{
	return (OpCode) ((struct COPScommon_hdr *)_data)->op_code;
};

unsigned int COPSCommonHeader::message_len() 
{
	return ntohl(((struct COPScommon_hdr *)_data)->message_len);
};

bool COPSCommonHeader::isUnsolicited()
{
	if(((struct COPScommon_hdr *)_data)->flags == UNSOLICITED) return true;
	return false;
}

bool COPSCommonHeader::valid() 
{
	if(((struct COPScommon_hdr *)_data)->version!= COPS_VERSION) return false;
	if(((struct COPScommon_hdr *)_data)->flags!=SOLICITED && ((struct COPScommon_hdr *)_data)->flags!=UNSOLICITED) return false;
	if(((struct COPScommon_hdr *)_data)->op_code<REQUEST || ((struct COPScommon_hdr *)_data)->op_code>SYNCHRONIZE_COMPLETE) return false;
	if(ntohl(((struct COPScommon_hdr *)_data)->message_len)%4!=0) return false;
	return true;
}

COPSCommonHeader::COPSCommonHeader()
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader::COPSCommonHeader() "<< this << endl;
#endif
	//((struct COPScommon_hdr *)_data) = new struct COPScommon_hdr[1];
	_data = new char [(sizeof(struct COPScommon_hdr))];
	memset(_data,0,sizeof(struct COPScommon_hdr));
	size = sizeof(struct COPScommon_hdr);
}

COPSCommonHeader::COPSCommonHeader(char *data)
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader::COPSCommonHeader(char *data) "<< this  << endl;
#endif
	if(data!=NULL) {
		_data = new char [(sizeof(struct COPScommon_hdr))];
	//	((struct COPScommon_hdr *)_data) = new struct COPScommon_hdr[1];
		memcpy(_data,data,sizeof(struct COPScommon_hdr));
		size = sizeof(struct COPScommon_hdr);	
	} else {
		_data = new char [(sizeof(struct COPScommon_hdr))];
	//	((struct COPScommon_hdr *)_data) = new struct COPScommon_hdr[1];
		memset(_data,0,sizeof(struct COPScommon_hdr));
		size = sizeof(struct COPScommon_hdr);
	}
}

COPSCommonHeader::COPSCommonHeader(const COPSCommonHeader &hdr)
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader::COPSCommonHeader(const COPSCommonHeader &hdr) " << this << endl;
#endif
	size=hdr.size;
	_data = new char[size];
	memcpy(_data,hdr._data,size);
}

COPSCommonHeader &COPSCommonHeader::operator=(const COPSCommonHeader &hdr)
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader &COPSCommonHeader::operator=(const COPSCommonHeader &hdr)" << this << " = " << &obj << endl;
#endif
	if(this!=&hdr) {		//avoid self assignment
		size=hdr.size;
		if(_data!=NULL)
			delete _data;	
		_data = new char[size];
		memcpy(_data,hdr._data,size);
	}
	return *this;
}

COPSCommonHeader::~COPSCommonHeader()
{
#ifdef DEBUG_MAX
	cerr << "COPSCommonHeader::~COPSCommonHeader() " << this << endl;
#endif
//	delete [] ((struct COPScommon_hdr*)_data);
	if(_data!=NULL)
		delete [] _data;
}
