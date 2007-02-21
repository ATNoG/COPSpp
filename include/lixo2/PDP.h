/* 
 * PDP.h
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

#ifndef _PDP_H_
#define _PDP_H_

#include <PThreadsmm/Thread.h>
#include <PThreadsmm/Guard.h>
#include <PThreadsmm/MonitoredQueue.h>
#include <PThreadsmm/Exception.h>

#include <iostream>
#include <map>
#include <queue>

#include <Copspp/SocketServer.h>
#include <Copspp/COPSmessage.h>

#include <Copspp/PDPchild.h>

#define SERVER_TYPE 0xff01
#define KA_TIME 20
#define ACCT_TIME 60

class PDP : public Thread
{
	public:
		PDP(int dbgLevel, std::string address_hook, unsigned int ka_time=KA_TIME, unsigned int acct_time=ACCT_TIME);
		virtual ~PDP() throw();
		virtual void run() throw();
		std::string ClientAccept(Socket *client);
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
		void CleanDeadPEPs();
		void CleanPEP(std::map<std::string, PDPchild *>::iterator p);
};


#endif	//_PDP_H_
