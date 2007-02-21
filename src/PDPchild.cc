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

using namespace std;

#include "PDPchild.h"
#include "color.h"

PDPchild::PDPchild(int debugLvl, Socket *s, std::string _pepid, unsigned int cT, int t) : Thread()
{
	pepid = _pepid;
	debugLevel=debugLvl;
	#ifdef DEBUG
	if(debugLevel>=1)
		cerr << "New PEP Connected: " << FG_CYAN << pepid << RESET << endl;
	#endif
	socket = s;
	timeout = t;
	connected = true;
	close = false;
	clientT = cT;
	
}


PDPchild::~PDPchild() throw()
{
	Guard g_s(&destroy_s);
	Guard g_r(&destroy_r);
	#ifdef DEBUG
	if(debugLevel>=5)
		cerr << "PDPchild::~PDPchild()" << endl;
	#endif
	if(!inbox.isCanceled())
		inbox.cancel();
	if(socket!=NULL)
		delete socket;
	socket = NULL;
}

void PDPchild::cancel() throw() 
{ 
	Guard g_s(&destroy_s);
	Guard g_r(&destroy_r);
	inbox.cancel();
	close = true; 
	COPSmessage close;
	#ifdef DEBUG
	if(debugLevel>=5)
		cerr << "PDPchild::cancel()" << endl;
	#endif
	*socket << close.ClientClose(clientT,COPSError::ShuttingDown);
};

void PDPchild::run() throw() {
	try {
		while(socket!=NULL && socket->Poll(timeout*1000) && !close) {		//timeout for Poll is in miliseconds
			try {
				
				COPSmessage msg;
				*socket >> msg;
				switch(msg) {
					case COPSCommonHeader::CLIENT_CLOSE:
						#ifdef DEBUG
						if(debugLevel>=2)
							cerr << "Received a Client Close" << endl;
						#endif
						inbox.add(new COPSmessage(msg));
						connected = false;
						return;
  					case COPSCommonHeader::KEEP_ALIVE:
						{
						COPSmessage ka;
						*socket<< ka.KeepAlive();
						#ifdef DEBUG
						if(debugLevel>=2)
							cerr << "Received a Keep Alive" << endl;
						#endif
						break;
						}
					default:
						inbox.add(new COPSmessage(msg));
						break;
				}
				
			} catch (COPSmessage_Exception e) {
				#ifdef DEBUG
				if(debugLevel>=5)
					cerr << "PDPchild::run() : COPSmessage_Exception : " << e.what() << endl;
				#endif
			} catch (Socket_Exception e) {
				#ifdef DEBUG
				if(debugLevel>=5)
					cerr << "PDPchild::run() : Socket_Exception : " << e.what() << endl;
				#endif
				connected = false;
				return;
			}	
		}
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "PEP " << FG_CYAN << pepid << RESET << " seams to be dead" << endl;
		#endif
		connected = false;
		return;				
	} catch (Socket_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=4)
			cerr << "PDPchild::run() : Socket_Exception 2 : " << e.what() << endl;
		#endif	
		connected = false;
		if(socket!=NULL)
			socket->Close();
	} catch (Exception e) {
		#ifdef DEBUG
		if(debugLevel>=4)
			cerr << "PDPchild::run() : Exception : " << e.what() << endl;	
		#endif
	} catch (...) {
		std::cerr << "PDPchild::run() : not supposed to reach this" << endl;
	}
}

COPSmessage PDPchild::Receive(unsigned long q_timeout) 
{
	Guard g(&destroy_r);
	COPSmessage *t = NULL;
	try {
		if(q_timeout!=0)
			t = inbox.next(q_timeout);	//next() leaves in charge of freeing t
		else	
			t = inbox.next();	//next() leaves in charge of freeing t

		if(t!=NULL) {
			COPSmessage m(*t);
			delete t;		//soo lets clean it ;)
			return m;
		}
		throw COPSmessage_Exception("Invalid message in inbox");
	} catch(BlockingQueue_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=5)
			cerr << "PDPchild::Receive : "<< e.what() << endl;
		#endif
		throw e;
	} catch(Timeout_Exception t) {
		throw t;
	} catch (...) {
		std::cerr << "PDPchild::Receive() : "<<FG_RED << "big mess" << RESET << std::endl;
	}
	throw COPSmessage_Exception("PDPchild::Receive did not find any message in his inbox");
	
}

unsigned PDPchild::Send(COPSmessage &msg)
{
	Guard g(&destroy_s);
	try {
		*socket << msg;
		if(msg==COPSCommonHeader::CLIENT_CLOSE) {
			connected = false;
			close = true;
			inbox.cancel(); // Only this early cancel seams to correctly signal the inbox...
		}
	} catch (Socket_Exception e) {
		std::cerr <<"PDPchild::Send() : "<< e.what() << std::endl;
		return 0;
	}
	return msg.lenght();	
}
