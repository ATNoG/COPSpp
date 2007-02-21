/*
 * PDPchild.h 
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

#ifndef _PDPCHILD_H_
#define _PDPCHILD_H_

#include <PThreadsmm/Thread.h>
#include <PThreadsmm/Guard.h>
#include <PThreadsmm/BlockingQueue.h>

#include <iostream>
#include <map>

#include "Socket.h"
#include "COPSmessage.h"
#include "color.h"
#include "config.h"

typedef BlockingQueue<COPSmessage *> Inbox;

class PDPchild : public Thread
{
	public:
		/**
		 * Constructor
		 * @param debugLevel Debug Level, to produce DEBUG messages to the std error out put use a value >0
		 * @param sock socket where the PEP is connected
		 * @param pep pep identification
		 * @param clientT client type
		 * @param Timeout amount of time the socket will block looking for new messages (ALL messages)
		 */
		PDPchild(int debugLevel, Socket *sock, std::string pep,unsigned int clientT, int Timeout);
		virtual ~PDPchild() throw();
		virtual void run() throw();
		virtual void cancel() throw();
		bool isConnected() {return connected;};	
		std::string getID() {return pepid;};
		/**
		 * Receives messages from the PEP
		 * This function will block in order to read a full COPSmessage
		 * @param timeout amount of time the socket will block looking for new messages (ALL EXCEPT CLOSE and KEEP ALIVE)
		 * @return COPSmessage received
		 */
		COPSmessage Receive(unsigned long timeout=0);
		/**
		 * Will send a COPSmessage to the PEP
		 * @param msg COPSmessage to be sent
		 */
		unsigned Send(COPSmessage &msg);
		/**
		 * COPSmessage queue
		 * Each received msg by run() will be left in this queue until Receive picks it up
		 */
		Inbox inbox;
		unsigned int getClientType(){return clientT;};

	protected:
		Socket *socket;
		int timeout;
		bool connected;
		bool close;
		unsigned int clientT;
		Mutex destroy_s;
		Mutex destroy_r;
		std::string pepid;
	private:
		int debugLevel;
};


#endif	//_PDPCHILD_H_
