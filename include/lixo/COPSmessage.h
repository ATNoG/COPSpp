/* 
 * COPSmessage.h
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

#ifndef _COPSMESSAGE_H_
#define _COPSMESSAGE_H_

#include <iostream>

#include <Copspp/Socket.h>

#include <Copspp/COPSCommonHeader.h>

#include <Copspp/COPSObj.h>
#include <Copspp/COPSPEPID.h>
#include <Copspp/COPSKATimer.h>
#include <Copspp/COPSError.h>
#include <Copspp/COPSHandle.h>
#include <Copspp/COPSContext.h>
#include <Copspp/COPSDecision.h>
#include <Copspp/COPSReportType.h>
#include <Copspp/COPSReason.h>
#include <Copspp/COPSClientSI.h>
#include <Copspp/COPSINInt.h>
#include <Copspp/COPSOUTInt.h>
#include <Copspp/COPSAcctTimer.h>
#include <Copspp/COPSPDPRedirAddr.h>
#include <Copspp/COPSLastPDPAddr.h>

#define COPS_IP_PROTOCOL "3288"

class COPSmessage_Exception
{
	public:
		COPSmessage_Exception(const char *str);
		COPSmessage_Exception(COPSError::Error_Code err_code);
		~COPSmessage_Exception() {};
		const std::string what() {return error_msg; };
		COPSError::Error_Code error() {return error_code; };
	private:
		std::string error_msg;
		COPSError::Error_Code error_code;
		
};

class COPSmessage
{
	public:
		COPSmessage();
		COPSmessage(const COPSmessage &msg);
		~COPSmessage();

		void Append(const COPSCommonHeader &);
		void Append(const COPSObj &);
		
		friend COPSmessage &operator+(COPSmessage &c, const COPSCommonHeader &h);
		friend COPSmessage &operator+(COPSmessage &c, const COPSObj &obj);

		friend COPSmessage &operator << (COPSmessage &, const COPSCommonHeader &);
		friend COPSmessage &operator << (COPSmessage &, const COPSObj &);

		friend Socket &operator<<(Socket &socket, const COPSmessage &m);
		friend Socket &operator>>(Socket &socket, COPSmessage &m);
		
		COPSCommonHeader::OpCode getMessageType();	
		bool isValid() {if(size) return true; else return false;};
		operator int() {return getMessageType();};	
		bool operator==(COPSCommonHeader::OpCode code);
		bool operator!=(COPSCommonHeader::OpCode code);
		COPSmessage& operator=(const COPSmessage&);

		COPSCommonHeader getCOPSCommonHeader() const;
		COPSPEPID getCOPSPEPID() const;
		COPSKATimer getCOPSKATimer() const; 
		COPSHandle getCOPSHandle() const; 
		COPSContext getCOPSContext(unsigned int n=1) const;
		COPSDecision getCOPSDecision(COPSDecision::C_Type t) const;
		COPSDecision getCOPSDecision(unsigned int n=1) const;		
		COPSReportType getCOPSReportType() const; 
		COPSReason getCOPSReason() const;
		COPSError getCOPSError() const;
		COPSClientSI getCOPSClientSI(unsigned int n=1) const;
		COPSINInt getCOPSINInt() const;
		COPSOUTInt getCOPSOUTInt() const;
		COPSAcctTimer getCOPSAcctTimer() const; 
		COPSPDPRedirAddr getCOPSPDPRedirAddr() const;
		COPSLastPDPAddr getCOPSLastPDPAddr() const;
		
		
		COPSmessage &ClientOpen(unsigned int clientT, std::string PepID);
		COPSmessage &ClientAccept(unsigned int clientT, unsigned int KATime, unsigned int AcctTime=0);
		COPSmessage &ClientClose(unsigned int clientT, COPSError::Error_Code error);
		COPSmessage &Request(unsigned int clientT, unsigned int handle, COPSContext::R_Type rtype, unsigned short int mtype=0);
		COPSmessage &Decision(unsigned int clientT,unsigned int handle, COPSContext::R_Type rtype, unsigned short int mtype, COPSDecision::Command_Code decision, bool solicited=true);
		COPSmessage &KeepAlive();
		COPSmessage &ReportState(unsigned int clientT, unsigned int handle, COPSReportType::Report_Type type);
		COPSmessage &DeleteRequestState(unsigned int clientT,unsigned int handle, COPSReason::Reason_Code code);
		COPSmessage &SynchronizeStateRequest(unsigned int clientT);
		COPSmessage &SynchronizeStateRequest(unsigned int clientT, unsigned int handle);
		COPSmessage &SynchronizeStateComplete(unsigned int clientT);
		COPSmessage &SynchronizeStateComplete(unsigned int clientT, unsigned int handle);
	
		friend std::ostream &operator<<(std::ostream &o, const COPSmessage &cops);
		
		char *data() {return _data;};	
		unsigned lenght() {return size;};
		
	protected:
		char *_data;
		unsigned size;
	
	private:
		struct COPSobj_data * FindObject(COPSObj::C_num object, unsigned int pos=1) const;
};


#endif	//_COPSMESSAGE_H_
