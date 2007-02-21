/*
 * PDP.h 
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

#ifndef _PDP_H_
#define _PDP_H_

#include <PThreadsmm/Thread.h>
#include <PThreadsmm/Guard.h>
#include <PThreadsmm/MonitoredQueue.h>
#include <PThreadsmm/Exception.h>

#include <iostream>
using namespace std;
#include <map>
#include <queue>

#include "SocketServer.h"
#include "COPSmessage.h"
#include "color.h"
#include "config.h"

#include "PDPchild.h"

#define SERVER_TYPE 0xff01
#define KA_TIME 80
#define ACCT_TIME 120

class PDP : public Thread
{
	public:
		/**
		 * Constructor
		 * @param dbgLevel Debug Level, to produce DEBUG messages to the std error out put use a value >0
		 * @param address_hook the address in which the PDP will listen for connections (can use dns record or IP)
		 * @param ka_time the Keep Alive time used with the PEP's
		 * @param acct_time the Accounting time used with the PEP's
		 */
		PDP(int dbgLevel, std::string address_hook, unsigned int ka_time=KA_TIME, unsigned int acct_time=ACCT_TIME);
		virtual ~PDP() throw();
		/**
		 * This functions starts the PDP thread
		 */
		virtual void run() throw();
		/**
		 * This function accepts the new pep by exchange of COPS Client Open and COPS Client Accept
		 * @param client Socket where the pep is connected
		 * @return the PEPid according to the COPSClientOpen message
		 */
		std::string ClientAccept(Socket *client);
		/**
		 * List of PDPchild's containing PEP that are not being taken care of
		 * This Function accts as a temporary repository between acceptance of the pep and processing of the pep.
		 */
		MonitoredQueue<PDPchild *> unatendedPEP;	
		unsigned int getClientType() {return clientT;};
	
	protected:
		SocketServer socket;
		std::string Address;
		std::map<std::string,PDPchild *> ConnectedPEP;
		unsigned int clientT;
		unsigned int ka_timeout;
		unsigned int acct_time;
	
	private:
		int debugLevel;
		/**
		 * This will iterate the list of connected PEP and will close all inactive sessions
		 */
		void CleanDeadPEPs();
		/**
		 * This will close a given PEP connection
		 */
		void CleanPEP(std::map<std::string, PDPchild *>::iterator p);
};


#endif	//_PDP_H_
