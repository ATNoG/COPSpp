/* 
 * PEP.h
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

#ifndef _PEP_H_
#define _PEP_H_

#include <PThreadsmm/Thread.h>
#include <PThreadsmm/Guard.h>
#include <PThreadsmm/Mutex.h>
#include <PThreadsmm/Condition.h>
#include <PThreadsmm/BlockingQueue.h>

#include <iostream>

#include <Copspp/SocketClient.h>
#include <Copspp/COPSmessage.h>

#define CLIENT_TYPE 0xff02

typedef BlockingQueue<COPSmessage *> Inbox;

class PEP_exception {
	public:
		PEP_exception(const char *str);
		~PEP_exception() {};
		const std::string what() {return error_msg; };
	private:
		std::string error_msg;
};

class PEPKeepAlive: public Thread {
	public:
		PEPKeepAlive(int debugLvl,Socket *s, unsigned short int ka);
		virtual ~PEPKeepAlive() throw() {};
  		virtual void run() throw();
		void gotKA();
		bool finnished;
        void cancel() throw() { 
			c.cancel();
		};

	private:
		Condition KAreply;
		Socket *socket;
		unsigned short int katime;		
		int debugLevel;
		Condition c;
};

class PEP : public Thread
{
	public:
		enum inbox {
			solicited_msg=0,
			unsolicited_msg=1
		};
		PEP(int debugLevel,std::string id, std::string pdp_name);
		virtual ~PEP() throw();
		virtual void run() throw();
	
		void Connect(const char *server);
		unsigned int getClientType() {return clientT;};
		
		COPSmessage Receive(inbox box);
		void Abort(inbox box);
		void Send(COPSmessage &msg);
                int getAcctTimer() { return accttime; };
	protected:
		Inbox solicited;
		Inbox unsolicited;
		SocketClient socket;
		long int clientT;
		std::string PepID;
		int katime;
		int accttime;
		PEPKeepAlive *keepalive_task;
		std::string pdp;
		Mutex destroy_send;
		Mutex solicited_receive;
		Mutex unsolicited_receive;
	private:
		int debugLevel;
		bool connected;
};


#endif	//_PEP_H_
