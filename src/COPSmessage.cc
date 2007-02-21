/*
 * COPSmessage.cc 
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

#include "COPSmessage.h"

COPSmessage_Exception::COPSmessage_Exception(const char *str) : error_code(COPSError::Unspecified) {
	error_msg = str;
}

COPSmessage_Exception::COPSmessage_Exception(COPSError::Error_Code err_code) : error_code(err_code) 
{
	error_msg = strErrorCode(err_code);
}

COPSmessage::COPSmessage()
{
	size=0;
	_data = NULL;
}

COPSmessage::COPSmessage(const COPSmessage &msg)
{
	size=msg.size;
	_data = new char[size];
#ifdef DEBUG_MAX
	std::cerr << "COPSmessage::COPSmessage(const COPSmessage &) " << hex << this << dec << std::endl;
#endif
	memcpy(_data,msg._data,size);
}

COPSmessage::~COPSmessage()
{
	if(_data != NULL)
		delete [] _data;
}

Socket &operator>>(Socket &socket, COPSmessage &m) {
	try {
		COPSCommonHeader hdr;		// First peek the header to see how much room we need for the whole message
		unsigned c_recv=0;
		if(&socket==NULL) throw Socket_Exception("Socket is NULL...");
		
		do {
			c_recv=socket.TryReceive(hdr.data(),sizeof(struct COPScommon_hdr));
		} while(c_recv!=sizeof(struct COPScommon_hdr));
		
		if(c_recv!=sizeof(struct COPScommon_hdr))
			throw COPSmessage_Exception("Message in socket does not have minimum size of a COPS header");
		
		if(!hdr.valid()) {
			  throw COPSmessage_Exception(COPSError::BadMessageFormat);
		}
		
		
		m.size=hdr.message_len();			// Make room based on the hdr and finally receive the message
		if(m._data != NULL)
			delete [] m._data;
			//delete [] (char *) m._data;
		m._data = new char[m.size];

		unsigned int k = 0;
		while (k < m.size) {
			int res = socket.Receive((void *)(m._data + k), m.size - k);
			k += res;
		}

	} catch (Socket_Exception e) {
		throw(e);
	}
	return socket;
}

Socket &operator<<(Socket &socket, const COPSmessage &m) {
	try{
		if((unsigned) socket.Send(m._data,m.size)!=m.size) throw Socket_Exception("Unable to send all data through socket");
	} catch (Socket_Exception e) {
		std::cerr << e.what() << std::endl;
	}
	return socket;
};

COPSmessage &COPSmessage::operator=(const COPSmessage &m) {
#ifdef DEBUG_MAX
	std::cerr << "COPSmessage &COPSmessage::operator=(const COPSmessage &m)  " << hex << this << " = " << m << dec << std::endl;
#endif
	if(this!= &m) {			//ignore self assignment
		if(_data != NULL) 
			delete [] _data;
		_data = new char[size=m.size];
		memcpy(_data,m._data,m.size);
	}
	return *this;
}

COPSmessage &operator+(COPSmessage &c, const COPSCommonHeader &h) {
	c.Append(h);
	return c;
}

COPSmessage &operator << (COPSmessage &c, const COPSCommonHeader &h) {
	c.Append(h);
	return c;
}

void COPSmessage::Append(const COPSCommonHeader &h) {
	if(_data) 
		delete [] _data;
	_data = new char[h.size];
	memcpy(_data, h._data, h.size);
	size = h.size;
}

COPSmessage &operator+(COPSmessage &c, const COPSObj &obj) {
	c.Append(obj);
	return c;
}

COPSmessage &operator << (COPSmessage &c, const COPSObj &obj) {
	c.Append(obj);
	return c;
}

void COPSmessage::Append(const COPSObj &obj) {
	char *tmp;
	char *old = _data;

	tmp = new char[size + obj.size];
	if (_data)
		memcpy(tmp, _data, size);

	memcpy(tmp + size, obj.data, obj.size);
	((struct COPScommon_hdr *)tmp)->message_len = htonl(size + obj.size);

	size += obj.size;
	_data = tmp;

	if (_data)
		delete [] old;
}

COPSCommonHeader COPSmessage::getCOPSCommonHeader() const 
{			
	COPSCommonHeader hdr(_data);
	return hdr;
}

COPSPEPID COPSmessage::getCOPSPEPID() const 
{			
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::PEPID);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);

	char *identifier;
	identifier = (char *) ((char *)tmp+ sizeof(struct COPSobj_data));
	COPSPEPID pepid(identifier);
	return pepid;

}

COPSKATimer COPSmessage::getCOPSKATimer() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::KATimer);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);

	unsigned short int katime;
	katime = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //KA Timer value is in the last 2 octets
	COPSKATimer katimer(ntohs(katime));
	return katimer;
}

COPSHandle COPSmessage::getCOPSHandle() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::Handle);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);

	unsigned int handle;
	handle = *(unsigned int *) ((char *)tmp+ sizeof(struct COPSobj_data));
	COPSHandle Handle(ntohl(handle));
	return Handle;
}

COPSContext COPSmessage::getCOPSContext(unsigned int n) const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::Context,n);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);
	unsigned short int r_type, m_type;
	r_type = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); //first 2 octets contain R-Type
	m_type = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //in the last 2 octets rests the M-Type
	COPSContext Context(ntohs(r_type),ntohs(m_type));
	return Context;
}

COPSDecision COPSmessage::getCOPSDecision(COPSDecision::C_Type t) const
{
	try {
		int n=1;
		while(1) {
			COPSDecision d = (*this).getCOPSDecision(n);
			if(d.getType()==t) return d;
			n++;
		}
	} catch (COPSmessage_Exception e) {
		throw e;
	}
}
COPSDecision COPSmessage::getCOPSDecision(unsigned int n) const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::Decision,n);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_num!= COPSObj::Decision) throw COPSmessage_Exception(COPSError::BadMessageFormat);
	if(tmp->c_type==COPSDecision::Decision_Flags) {
		COPSDecision::Command_Code command;
		unsigned int Flags;
		command = *(COPSDecision::Command_Code *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); //first 2 octets contain Command-Code
		Flags = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //in the last 2 octets rests the Flags
		COPSDecision Decision((COPSDecision::Command_Code) ntohs(command),ntohs(Flags));
		return Decision;
	} else {
		COPSDecision::C_Type decision;
		decision = (COPSDecision::C_Type) tmp->c_type;
		unsigned int size_;
		size_ = ntohs(tmp->obj_len);
		char data_[size_];		
		memcpy(data_,((((char *)tmp)+ sizeof(struct COPSobj_data))),size_-sizeof(struct COPSobj_data));	
		COPSDecision Decision(decision,data_,size_);
		return Decision;
	}
}

COPSReportType COPSmessage::getCOPSReportType() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::ReportType);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);

	unsigned short int type;
	type = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]);
	COPSReportType report((COPSReportType::Report_Type) ntohs(type));
	return report;
}

COPSReason COPSmessage::getCOPSReason() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::Reason);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);
	unsigned short int code, sub_code;
	code = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); //first 2 octets contain Reason-Code
	sub_code = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //in the last 2 octets rests the Reason Sub-code
	COPSReason reason((COPSReason::Reason_Code) ntohs(code),ntohs(sub_code));
	return reason;
}

COPSError COPSmessage::getCOPSError() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::Error);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);
	unsigned short int code, sub_code;
	code = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); //first 2 octets contain Error-Code
	sub_code = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //in the last 2 octets rests the Error Sub-code
	COPSError error((COPSError::Error_Code) ntohs(code),ntohs(sub_code));
	return error;
}

COPSClientSI COPSmessage::getCOPSClientSI(unsigned int n) const throw(COPSmessage_Exception)
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::ClientSI,n);
	} catch (COPSmessage_Exception e) {
		#ifdef DEBUG
		std::cerr << "Couldn't not find any ClientSI object" << std::endl;
		#endif
		throw e;
	}
	
	#ifdef DEBUG
	std::cerr << "ClientSI@ " << hex << (int) tmp << dec << "Type=" << tmp->c_type << "!=" << COPSClientSI::Signaled << " && " << tmp->c_type << "!=" << COPSClientSI::Named << std::endl;
	#endif
	if(tmp->c_type!=COPSClientSI::Signaled && tmp->c_type!=COPSClientSI::Named) throw COPSmessage_Exception(COPSError::BadMessageFormat);
	COPSClientSI::C_Type type;
	type = (COPSClientSI::C_Type) tmp->c_type;
	unsigned int size;
	size = ntohs(tmp->obj_len);
	char data[size];		
	memcpy(data,&(((char *)tmp+ sizeof(struct COPSobj_data))[0]),size-sizeof(struct COPSobj_data));	
	COPSClientSI clientsi(type,data,size);
	return clientsi;	
}


COPSINInt COPSmessage::getCOPSINInt() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::INInt);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	unsigned int ifindex;
	if(tmp->c_type==COPSINInt::IP4) {	
		in_addr *addr;
		addr = (in_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		ifindex = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[4]);	//right after ipv4 address (32bits)
		COPSINInt interf(addr,ntohl(ifindex));
		return interf;
	} else if(tmp->c_type==COPSINInt::IP6){	
		in6_addr *addr;
		addr = (in6_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		ifindex = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[16]);	//right after ipv6 address (128bits)
		COPSINInt interf(addr,ntohl(ifindex));
		return interf;
	} 
	throw COPSmessage_Exception(COPSError::BadMessageFormat);
}

COPSOUTInt COPSmessage::getCOPSOUTInt() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::OUTInt);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	unsigned int ifindex;
	if(tmp->c_type==COPSOUTInt::IP4) {	
		in_addr *addr;
		addr = (in_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		ifindex = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[4]);		//right after ipv4 address (32bits)
		COPSOUTInt interf(addr,ntohl(ifindex));
		return interf;
	} else if(tmp->c_type==COPSOUTInt::IP6){	
		in6_addr *addr;
		addr = (in6_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		ifindex = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[16]);	//right after ipv6 address (128bits)
		COPSOUTInt interf(addr,ntohl(ifindex));
		return interf;
	} 
	throw COPSmessage_Exception(COPSError::BadMessageFormat);
}

COPSAcctTimer COPSmessage::getCOPSAcctTimer() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::AcctTimer);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	if(tmp->c_type!=1) throw COPSmessage_Exception(COPSError::BadMessageFormat);

	unsigned short int accttime;
	accttime = *(unsigned short int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[2]); //KA Timer value is in the last 2 octets
	COPSAcctTimer accttimer(ntohs(accttime));
	return accttimer;
}

COPSPDPRedirAddr COPSmessage::getCOPSPDPRedirAddr() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::PDPRedirAddr);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	unsigned short int port;
	if(tmp->c_type==COPSPDPRedirAddr::IP4) {	
		in_addr *addr;
		addr = (in_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		port = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[6]);	//right after ipv4 address (32bits) + 2octets padding
		COPSPDPRedirAddr redir(addr,ntohs(port));
		return redir;
	} else if(tmp->c_type==COPSPDPRedirAddr::IP6){	
		in6_addr *addr;
		addr = (in6_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		port = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[18]);	//right after ipv6 address (128bits) + 2octets padding
		COPSPDPRedirAddr redir(addr,ntohs(port));
		return redir;
	} 
	throw COPSmessage_Exception(COPSError::BadMessageFormat);
}

COPSLastPDPAddr COPSmessage::getCOPSLastPDPAddr() const
{
	struct COPSobj_data *tmp;
	try {
		tmp = FindObject(COPSObj::LastPDPAddr);
	} catch (COPSmessage_Exception e) {
		throw e;
	}
	
	unsigned short int port;
	if(tmp->c_type==COPSLastPDPAddr::IP4) {	
		in_addr *addr;
		addr = (in_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		port = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[6]);	//right after ipv4 address (32bits) + 2octets padding
		COPSLastPDPAddr redir(addr,ntohs(port));
		return redir;
	} else if(tmp->c_type==COPSLastPDPAddr::IP6){	
		in6_addr *addr;
		addr = (in6_addr *) &(((char *)tmp+ sizeof(struct COPSobj_data))[0]); 
		port = *(unsigned int *) &(((char *)tmp+ sizeof(struct COPSobj_data))[18]);	//right after ipv6 address (128bits) + 2octets padding
		COPSLastPDPAddr redir(addr,ntohs(port));
		return redir;
	} 
	throw COPSmessage_Exception(COPSError::BadMessageFormat);
}

COPSCommonHeader::OpCode COPSmessage::getMessageType() {
	COPSCommonHeader hdr(_data);
	return hdr.getOpCode();
}

bool COPSmessage::operator==(COPSCommonHeader::OpCode code) {
	if(getMessageType()==code) return true;
	return false;
}

bool COPSmessage::operator!=(COPSCommonHeader::OpCode code) {
	if(getMessageType()!=code) return true;
	return false;
}

std::ostream &operator<<(std::ostream &o, const COPSmessage &cops) {
	if(cops._data==NULL) return o;
	if(cops.size==0) return o;
	char *tmp;
	tmp = new char[cops.size];
	memcpy(tmp,cops._data,cops.size);
	o << std::hex;
	for(unsigned i=0; i<cops.size; i+=4)
		o << ((tmp[i]>>4)&0x0f) << ((tmp[i])&0x0f) <<((tmp[i+1]>>4)&0x0f) << ((tmp[i+1])&0x0f) << ((tmp[i+2]>>4)&0x0f) << ((tmp[i+2])&0x0f) <<((tmp[i+3]>>4)&0x0f) << ((tmp[i+3])&0x0f) << "\n";  
	o << std::dec;
	delete [] tmp;
	return o;
}

COPSmessage &COPSmessage::ClientOpen(unsigned int clientT, std::string PepID)
{
	COPSCommonHeader hdr(COPSCommonHeader::CLIENT_OPEN,clientT);
	COPSPEPID pepid(PepID);
	*this+hdr;
	*this+pepid;
	return (*this);
}

COPSmessage &COPSmessage::ClientAccept(unsigned int clientT, unsigned int KATime, unsigned int AcctTime)
{
	COPSCommonHeader hdr(COPSCommonHeader::CLIENT_ACCEPT,clientT);
	COPSKATimer katimer(KATime);
	COPSAcctTimer accttimer(AcctTime);
	*this+hdr+katimer+accttimer;
	return (*this);
}

COPSmessage &COPSmessage::ClientClose(unsigned int clientT, COPSError::Error_Code error)
{
	COPSCommonHeader hdr(COPSCommonHeader::CLIENT_CLOSE,clientT);
	COPSError err(error);
	*this+hdr+err;
	return (*this);
}

COPSmessage &COPSmessage::KeepAlive()
{
	COPSCommonHeader hdr(COPSCommonHeader::KEEP_ALIVE,0);
	*this+hdr;
	return (*this);
}

COPSmessage &COPSmessage::Request(unsigned int clientT, unsigned int handle, COPSContext::R_Type type,unsigned short int mtype)
{
	COPSCommonHeader hdr(COPSCommonHeader::REQUEST,clientT);
	COPSHandle hdl(handle);
	COPSContext cont(type,mtype);		
	*this+hdr+hdl+cont;
	return (*this);
}

COPSmessage &COPSmessage::ReportState(unsigned int clientT, unsigned int handle, COPSReportType::Report_Type rtype)
{
	COPSCommonHeader hdr(COPSCommonHeader::REPORT_STATE,clientT);
	COPSHandle hdl(handle);
	COPSReportType rep(rtype);		
	*this+hdr+hdl+rep;
	return (*this);
}

COPSmessage &COPSmessage::Decision(unsigned int clientT,unsigned int handle, COPSContext::R_Type rtype, unsigned short int mtype, COPSDecision::Command_Code decision, bool solicited)
{
	COPSCommonHeader hdr(COPSCommonHeader::DECISION,clientT,solicited);
	COPSHandle hdl(handle);
	COPSContext context(rtype,mtype);
	COPSDecision dec(decision);
	*this+hdr+hdl+context+dec;
	return (*this);
}

COPSmessage &COPSmessage::DeleteRequestState(unsigned int clientT,unsigned int handle, COPSReason::Reason_Code code)
{
	COPSCommonHeader hdr(COPSCommonHeader::DELETE_REQUEST_STATE,clientT);
	COPSHandle hdl(handle);
	COPSReason res(code);
	(*this)+hdr+hdl+res;
	return (*this);
}


COPSmessage &COPSmessage::SynchronizeStateRequest(unsigned int clientT)
{
	COPSCommonHeader hdr(COPSCommonHeader::SYNCHRONIZE_STATE_REQ,clientT);
	(*this)+hdr;
	return (*this);
}

COPSmessage &COPSmessage::SynchronizeStateRequest(unsigned int clientT, unsigned int handle)
{
	COPSCommonHeader hdr(COPSCommonHeader::SYNCHRONIZE_STATE_REQ,clientT);
	COPSHandle hdl(handle);
	(*this)+hdr+hdl;
	return (*this);
}

COPSmessage &COPSmessage::SynchronizeStateComplete(unsigned int clientT)
{
	COPSCommonHeader hdr(COPSCommonHeader::SYNCHRONIZE_COMPLETE,clientT);
	(*this)+hdr;
	return (*this);
}

COPSmessage &COPSmessage::SynchronizeStateComplete(unsigned int clientT, unsigned int handle)
{
	COPSCommonHeader hdr(COPSCommonHeader::SYNCHRONIZE_COMPLETE,clientT);
	COPSHandle hdl(handle);
	(*this)+hdr+hdl;
	return (*this);
}


struct COPSobj_data *  COPSmessage::FindObject(COPSObj::C_num object, unsigned int pos) const throw(COPSmessage_Exception)
{
	struct COPSobj_data * tmp_i = (struct COPSobj_data *) ((char *)_data + sizeof(struct COPScommon_hdr));
	struct COPSobj_data * tmp = tmp_i;
	char *end = ((char *)_data + size);
	unsigned short int offset = 0;
	for(unsigned i=1; i<=pos; i++) {
		if((char *)tmp==end) throw COPSmessage_Exception(COPSError::ObjectNotFound); //TODO create other error message
		while(tmp->c_num != (unsigned int) object) {
			offset += ntohs(tmp->obj_len); //calculate offset taking into account current object lenght
			tmp = (struct COPSobj_data *) ((char *)tmp_i + offset); //move to next object
			if((char *)tmp >= end)
				throw COPSmessage_Exception(COPSError::MandatoryCOPSObjectMissing);
			#ifdef DEBUG
				cerr << "Found an Object, but not the right object " << endl;
			#endif
		}
		if(i==pos) 
			return tmp;
		else {
			#ifdef DEBUG
			cerr << "Found desired object but not in right position, got " << i << " looking for " << pos << endl;
			#endif
			offset += ntohs(tmp->obj_len); //calculate offset taking into account current object lenght
			tmp = (struct COPSobj_data *) ((char *)tmp_i + offset); //move to next object
		}
	}
	#ifdef DEBUG
	cerr << "Did not find Object" << endl;
	#endif
	return tmp;
}
