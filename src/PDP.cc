/*
 * PDP.cc 
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

#include "PDP.h"
#include "color.h"

PDP::PDP(int dbgLevel,std::string address_hook, unsigned int ka_time,unsigned int acct_timeout) : Thread()
{
	debugLevel = dbgLevel;
	Address = address_hook;
	clientT = SERVER_TYPE;
	ka_timeout = ka_time;
	acct_time = acct_timeout;
	start();
}


PDP::~PDP() throw()
{
#ifdef DEBUG
	if(debugLevel>=5)
		cerr << "PDP::~PDP() : Shuting Down..." << endl;
#endif
	unatendedPEP.cancel();
	for(std::map<std::string, PDPchild *>::iterator p = ConnectedPEP.begin(); p!=ConnectedPEP.end(); ++p) {
#ifdef DEBUG
		if(debugLevel>=5)
			cerr << "PDP::~PDP() - Closing " << p->first << endl;
#endif
		p->second->cancel();	
	};
}	

void PDP::CleanDeadPEPs() 
{
	for(std::map<std::string, PDPchild *>::iterator p = ConnectedPEP.begin(); p!=ConnectedPEP.end(); ++p)
		if(!p->second->isConnected()) {
			CleanPEP(p++);
			if(p==ConnectedPEP.end()) return;
		}
}

void PDP::CleanPEP(std::map<std::string, PDPchild *>::iterator p)
{
	try {
#ifdef DEBUG
		if(debugLevel>=5)
			cerr << "Erasing PEP: " << p->first << endl;
#endif
		ConnectedPEP.erase(p);
	} catch (...) {
		std::cerr << FG_RED << "PDP::CleanPEP() has serious problems..." << RESET << std::endl;	
	}
}
	
void PDP::run() throw() {
	try {
		Socket *client;
#ifdef DEBUG
		if(debugLevel>=5)
			cerr<< "PDP Started" << endl;
#endif
		socket.Bind(Address.c_str(),COPS_IP_PROTOCOL);
		socket.Listen();

		while(1) {
			client = socket.Accept();
			CleanDeadPEPs();
			try {
				
				std::string new_pep = ClientAccept(client);
				#ifdef DEBUG
				if(debugLevel>=1)
					cerr<< "PDP ClientAccept" << endl;
				#endif
				
			} catch (COPSmessage_Exception e) {
				std::cerr << "Client: " << client << " : PDP::run(): COPSmessage_Exception : " << e.what() << std::endl;
				delete client;
			} catch (Socket_Exception e) {
				std::cerr << "PDP::run(): Socket_Exception : " <<FG_CYAN << e.what() << RESET <<std::endl;
			}	
		}
	} catch (Socket_Exception e) {
		std::cerr << FG_RED << "PDP::run(): " << e.what() << RESET << std::endl;	
		// o q fazer ?? socket passou-se, reiniciar os clientes todos ??
	} catch (Exception e) {
		std::cerr << FG_RED << "PDP::run(): " << e.what() << RESET << std::endl;	
		// o q fazer ?? socket passou-se, reiniciar os clientes todos ??
	} catch (...) {
		std::cerr << FG_RED << "PDP::run() : this was not supposed to happen..." << std::endl;
	}
}

std::string PDP::ClientAccept(Socket *client)
{
	std::string new_pep;
	COPSmessage msg;
	#ifdef DEBUG
	if(debugLevel>=7)	
		cerr << "Read msg" << endl;
	#endif
	*client >> msg;
	#ifdef DEBUG
	if(debugLevel>=7)	
		cerr << "Got msg" << endl;
	#endif
	try {
		if(msg==COPSCommonHeader::CLIENT_OPEN && unatendedPEP.empty(ka_timeout*1000)) {
			
			new_pep = msg.getCOPSPEPID().getId();
			int pep_clientT = msg.getCOPSCommonHeader().getClientT();
			std::map<std::string, PDPchild *>::iterator p;
			if((p = ConnectedPEP.find(new_pep))!=ConnectedPEP.end())
				CleanPEP(p);
			
			ConnectedPEP[new_pep]= new PDPchild(debugLevel,client,new_pep,pep_clientT,ka_timeout);
			
			unatendedPEP.add(ConnectedPEP[new_pep]);
			#ifdef DEBUG		
			if(debugLevel>=7)
				cerr << "See if we are ready to accept " << new_pep<< endl;
			#endif
			if(unatendedPEP.empty(ka_timeout*1000)) {	//wait here until some one get's the new child
				#ifdef DEBUG
				if(debugLevel>=7)
					cerr << "We can accept "<< new_pep << endl;
				#endif
				COPSmessage accept;
				*client << accept.ClientAccept(clientT,ka_timeout,acct_time);
				#ifdef DEBUG
				if(debugLevel>=3)
					cerr << FG_CYAN << new_pep << RESET << " was Accepted" << endl;	
				#endif	
				ConnectedPEP[new_pep]->start();
			} else {
				std::cerr << "No one picked the unatendedPEP please check upper level code..."<< std::endl;
				unatendedPEP.next(); // someone upstream is too busy soo let me handle the thing
				throw Timeout_Exception("No one picked the unatendedPEP");
			}
			
		} else {
			COPSmessage close;
			*client << close.ClientClose(clientT,COPSError::BadMessageFormat);
			#ifdef DEBUG
				if(debugLevel>=3)
					cerr << FG_CYAN << new_pep << RESET << " connection was Closed doe to invalid CLIENT_OPEN message" << endl;
			#endif
			throw COPSmessage_Exception(COPSError::BadMessageFormat);
		}	
	} catch (Timeout_Exception e) {
		COPSmessage close;
		*client << close.ClientClose(clientT,COPSError::UnableToProcess);
		#ifdef DEBUG
		if(debugLevel>=3)
			cerr << FG_CYAN << new_pep << RESET << " connection Closed doe to the inability of the calling thread to process unatendedPEPs" << endl;
		#endif
		throw COPSmessage_Exception(COPSError::UnableToProcess);
	} catch (Exception e) {
		std::cerr << FG_CYAN << e.what() << RESET << std::endl;
	}catch (...) {
		std::cerr << FG_RED << "PDP::ClientAccept() has serious problems..." << RESET << std::endl;	
	}
	return new_pep;
}
