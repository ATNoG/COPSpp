/*
 * COPSmessage.h
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

#ifndef _COPSMESSAGE_H_
#define _COPSMESSAGE_H_


#include "config.h"

#include "Socket.h"

#include "COPSCommonHeader.h"

#include "COPSObj.h"
#include "COPSPEPID.h"
#include "COPSKATimer.h"
#include "COPSError.h"
#include "COPSHandle.h"
#include "COPSContext.h"
#include "COPSDecision.h"
#include "COPSReportType.h"
#include "COPSReason.h"
#include "COPSClientSI.h"
#include "COPSINInt.h"
#include "COPSOUTInt.h"
#include "COPSAcctTimer.h"
#include "COPSPDPRedirAddr.h"
#include "COPSLastPDPAddr.h"

#ifdef DEBUG 
#include <iostream>
using namespace std;
#endif

#ifdef DEBUG_MAX
#include <iostream>
using namespace std;
#endif

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
		
		/**
		 *	Provides the means to associate a COPSCommonHeader to a COPSmessage
		 *	@param c COPSmessage to which the header will be associated
		 *	@param h COPSCommonHeader to be associated
		 *	See Also next function definition for example
		 */
		friend COPSmessage &operator+(COPSmessage &c, const COPSCommonHeader &h);

		/**
		 * 	Provides the means to incrementaly associate COPS Objects to a COPS message
		 * 	Example:
		  	\code
			COPSmessage msg;
			COPSCommonHeader hdr;
			COPSHandle hdl;
			COPSContext cont;
			msg+hdr+hdl+cont;
			\endcode
		 * 	@param c COPSmessage to which the Objects will be appended
		 * 	@param obj COPSObject to be appended
		 */
		friend COPSmessage &operator+(COPSmessage &c, const COPSObj &obj);

		friend COPSmessage &operator << (COPSmessage &, const COPSCommonHeader &);
		/**
		 * 	Provides the means to incrementaly associate COPS Objects to a COPS message
		 * 	Example:
		  	\code
			COPSmessage msg;
			COPSCommonHeader hdr;
			COPSHandle hdl;
			COPSContext cont;
			msg << hdr << cont;
			\endcode
		 * 	@param c COPSmessage to which the Objects will be appended
		 * 	@param obj COPSObject to be appended
		 */

		friend COPSmessage &operator << (COPSmessage &, const COPSObj &);
		/**	Will send to a previously create TCP socket the COPS message
		 * 	Example:
			\code
			Socket s;
			COPSmessage m;
			s << m;
			\endcode
			@param socket TCP socket
			@param m COPSmessage fully constructed (contains header plus objects)
			*/
		friend Socket &operator<<(Socket &socket, const COPSmessage &m);
		/**	Will receive from a previously create TCP socket the COPS message.
		 * 	Please note that this is a blocking function, it will block until a COPSmessage is received (valid or not)
		 * 	Example:
			\code
			Socket s;
			COPSmessage m;
			s >> m;
			\endcode
			@param socket TCP socket
			@param m COPSmessage as retrieved from network 
			*/
		friend Socket &operator>>(Socket &socket, COPSmessage &m);
		
		COPSCommonHeader::OpCode getMessageType();	
		bool isValid() {if(size) return true; else return false;};
		/**
		 * Enables casting to int in order to use switch mechanims to parse the message based on the message Common Header Op Code
		 * Example:
		 \code
		 COPSmessage m;
		 switch (m) {
			 case REQ:
			 	cerr << "Got a Request Message";
				break;
			 case DEC:
			 	cerr << "Got a Decision Message";
				break;
				}
		\endcode
		*/
		operator int() {return getMessageType();};	
		/** 
		 * Compares a message with a COPSCommonHeader OpCode.
		 * Based on the OpCode of the COPSmessage COPSCommonHeader will return true or false
		 */
		bool operator==(COPSCommonHeader::OpCode code);
		/**
		 * Reverse of operator==
		 */
		bool operator!=(COPSCommonHeader::OpCode code);
		COPSmessage& operator=(const COPSmessage&);

		/**
		 * Retrives the COPSCommonHeader from a COPSmessage
		 */
		COPSCommonHeader getCOPSCommonHeader() const;
		/**
		 * Retrives the COPSPEPID from a COPSmessage
		 */
		COPSPEPID getCOPSPEPID() const;
		/**
		 * Retrieves the COPSKATimer from a COPSmessage
		 */
		COPSKATimer getCOPSKATimer() const; 
		/**
		 * Retrieves the COPSHandle from a COPSmessage
		 */
		COPSHandle getCOPSHandle() const; 
		/**
		 * Retrieves the COPSContext from a COPSmessage
		 * @param n the order number of the Context (we can have several). Default value is 1 (first)
		 */
		COPSContext getCOPSContext(unsigned int n=1) const;
		/**
		 * Retrieves the COPSDecision from a COPSmessage
		 * @param n the order number of the Decision (we can have several). Default value is 1 (first)
		 */
		COPSDecision getCOPSDecision(unsigned int n=1) const;	
		/**
		 * Retrieves the COPSDecision from COPSmessage by the C_Type
		 * @param t Decision C-Type
		 */
		COPSDecision getCOPSDecision(COPSDecision::C_Type t) const;
		/**
		 * Retrieves the COPSReportType from a COPSmessage
		 */
		COPSReportType getCOPSReportType() const; 
		/**
		 * Retrieves the COPSReason from a COPSmessage
		 */
		COPSReason getCOPSReason() const;
		/**
		 * Retrieves the COPSError from a COPSmessage
		 */
		COPSError getCOPSError() const;
		/**
		 * Retrieves the COPSClientSI from a COPSmessage
		 * @param n the order number of the Decision (we can have several). Default value is 1 (first)
		 */
		COPSClientSI getCOPSClientSI(unsigned int n=1) const throw (COPSmessage_Exception);
		/**
		 * Retrieves the COPSCOPSINInt from a COPSmessage
		 */
		COPSINInt getCOPSINInt() const;
		/**
		 * Retrieves the COPSOUTInt from a COPSmessage
		 */
		COPSOUTInt getCOPSOUTInt() const;
		/**
		 * Retrieves the COPSAcctTimer from a COPSmessage
		 */
		COPSAcctTimer getCOPSAcctTimer() const; 
		/**
		 * Retrieves the COPSPDPRedirAddr from a COPSmessage
		 */
		COPSPDPRedirAddr getCOPSPDPRedirAddr() const;
		/**
		 * Retrieves the COPSLastPDPAddr from a COPSmessage
		 */
		COPSLastPDPAddr getCOPSLastPDPAddr() const;
		
		/**
		 * Provides a macro to create a COPSmessage to send a ClientOpen
		 * @param clientType see COPSCommonHeader
		 * @param PepID see COPSPEPID
		 * @return COPSmessage will necessary objects to send out a Client Open
		 */
		COPSmessage &ClientOpen(unsigned int clientType, std::string PepID);
		/**
		 * Provides a macro to create a COPSmessage to send a ClientAccept
		 * @param clientType see COPSCommonHeader
		 * @param KATime see COPSKATimer
		 * @param AcctTime see COPSAcctTimer
		 * @return COPSmessage will necessary objects to send out a Client Accept 
		 */
		COPSmessage &ClientAccept(unsigned int clientType, unsigned int KATime, unsigned int AcctTime=0);
		/**
		 * Provides a macro to create a COPSmessage to send a ClientClose
		 * @param clientType see COPSCommonHeader
		 * @param error see COPSError
		 * @return COPSmessage will necessary objects to send out a Client Close 
		 */
		COPSmessage &ClientClose(unsigned int clientType, COPSError::Error_Code error);
		/**
		 * Provides a macro to create a COPSmessage to send a Request
		 * @param clientType see COPSCommonHeader
		 * @param handle see COPSHandle
		 * @param rtype see COPSContext
		 * @param mtype see COPSContext
		 * @return COPSmessage will necessary objects to send out a Request 
		 */
		COPSmessage &Request(unsigned int clientType, unsigned int handle, COPSContext::R_Type rtype, unsigned short int mtype=0);
		/**
		 * Provides a macro to create a COPSmessage to send a Decision 
		 * @param clientType see COPSCommonHeader
		 * @param handle see COPSHandle 
		 * @param rtype see COPSContext
		 * @param mtype see COPSContext
		 * @param decision see COPSDecision
		 * @param solicited see COPSCommonHeader
		 * @return COPSmessage will necessary objects to send out a Decision 
		 */
		COPSmessage &Decision(unsigned int clientType, unsigned int handle, COPSContext::R_Type rtype, unsigned short int mtype, COPSDecision::Command_Code decision, bool solicited=true);
		/**
		 * Provides a macro to create a COPSmessage to send a KeepAlive 
		 * @return COPSmessage will necessary objects to send out a KeepAlive 
		 */
		COPSmessage &KeepAlive();
		/**
		 * Provides a macro to create a COPSmessage to send a ReportState 
		 * @param clientType see COPSCommonHeader 
		 * @param handle see COPSHandle
		 * @param type COPSReportType
		 * @return COPSmessage will necessary objects to send out a ReportState 
		 */
		COPSmessage &ReportState(unsigned int clientType, unsigned int handle, COPSReportType::Report_Type type);
		/**
		 * Provides a macro to create a COPSmessage to send a Delete Request State 
		 * @param clientType see COPSCommonHeader
		 * @param handle see COPSHandle
		 * @param code see COPSReason
		 * @return COPSmessage will necessary objects to send out a Delete Request State 
		 */
		COPSmessage &DeleteRequestState(unsigned int clientType,unsigned int handle, COPSReason::Reason_Code code);
		/**
		 * Provides a macro to create a COPSmessage to send a Synchronize State Request 
		 * @param clientType see COPSCommonHeader
		 * @return COPSmessage will necessary objects to send out a Synchronize State Request 
		 */
		COPSmessage &SynchronizeStateRequest(unsigned int clientType);
		/**
		 * Provides a macro to create a COPSmessage to send a Synchronize State Request 
		 * @param clientType see COPSCommonHeader
		 * @param handle see COPSHandle
		 * @return COPSmessage will necessary objects to send out a Synchronize State Request 
		 */
		COPSmessage &SynchronizeStateRequest(unsigned int clientType, unsigned int handle);
		/**
		 * Provides a macro to create a COPSmessage to send a Synchronize State Complete 
		 * @param clientType see COPSCommonHeader
		 * @return COPSmessage will necessary objects to send out a Client Open
		 */
		COPSmessage &SynchronizeStateComplete(unsigned int clientType);
		/**
		 * Provides a macro to create a COPSmessage to send a Synchronize State Complete 
		 * @param clientType see COPSCommonHeader
		 * @param handle see COPSHandle
		 * @return COPSmessage will necessary objects to send out a Synchronize State Complete 
		 */
		COPSmessage &SynchronizeStateComplete(unsigned int clientType, unsigned int handle);
	
		friend std::ostream &operator<<(std::ostream &o, const COPSmessage &cops);
	
		/**
		 * Returns a pointer to a char * containing all the COPS message 
		 * see also lenght()
		 */
		char *data() {return _data;};	
		/**
		 * Size of the pointer returned by data()
		 */
		unsigned lenght() {return size;};
		
	protected:
		char *_data;
		unsigned size;
	
	private:
		struct COPSobj_data * FindObject(COPSObj::C_num object, unsigned int pos=1) const throw (COPSmessage_Exception);
};


#endif	//_COPSMESSAGE_H_
