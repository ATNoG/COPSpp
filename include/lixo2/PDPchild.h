/* 
 * PDPchild.h
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

#ifndef _PDPCHILD_H_
#define _PDPCHILD_H_

#include <PThreadsmm/Thread.h>
#include <PThreadsmm/Guard.h>
#include <PThreadsmm/BlockingQueue.h>

#include <iostream>
#include <map>

#include <Copspp/Socket.h>
#include <Copspp/COPSmessage.h>

typedef BlockingQueue<COPSmessage *> Inbox;

class PDPchild : public Thread
{
	public:
		PDPchild(int debugLevel, Socket *sock, std::string pep,unsigned int clientT, int Timeout);
		virtual ~PDPchild() throw();
		virtual void run() throw();
		virtual void cancel() throw();
		bool isConnected() {return connected;};	
		std::string getID() {return pepid;};
		COPSmessage Receive(unsigned long timeout=0);
		unsigned Send(COPSmessage &msg);
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
