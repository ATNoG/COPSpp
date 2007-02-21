/*
 * PEP.cc 
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

#include "PEP.h"
#include "color.h"

PEPKeepAlive::PEPKeepAlive(int debugLvl,Socket *s, unsigned short int ka) : Thread() {
	socket = s;
	katime = ka;
	debugLevel = debugLvl;
	finnished=false;
};

void PEPKeepAlive::run() throw()
{
	try {
		do {
				int vsleep = (int) (katime*(0.25+(0.5*rand()/(RAND_MAX+1.0))));			
				#ifdef DEBUG
				if(debugLevel>=9)
					cerr << "Going to bed (KA)" << endl;
				#endif
				try {
					c.timedwait(vsleep);
				} catch (...) {
					#ifdef DEBUG
					if(debugLevel>=9)
						cerr << "sleep in KA aborted" << endl;
					return;
					#endif
				}
			
				if(finnished) break;	// no point in continuing if this task is over
				#ifdef DEBUG
				if(debugLevel>=9)
					cerr << "Sending Keep Alive after " << vsleep << " seconds" << endl;	
				#endif
				COPSmessage ka;
				*socket << ka.KeepAlive();
				if(KAreply.wait(katime*250)) continue; //wait 1/4 of katime for the reply
				#ifdef DEBUG
				if(debugLevel>=5)
					cerr << FG_GREEN << "No reply to Keep Alive sent to the PDP" << RESET << endl;
				#endif
				throw COPSmessage_Exception(COPSError::CommunicationFailure);
			
		} while(!isCanceled());
	} catch (Exception e) {
		#ifdef DEBUG
		if(debugLevel>=5)
			cerr << FG_RED << e.what() << RESET << endl;
		#endif
	} catch (COPSmessage_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=5)
			cerr << FG_GREEN << e.what() << RESET << endl;
		#endif
	} catch (Socket_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=5)
			cerr << FG_RED << e.what() << RESET << endl;
		#endif
	} catch (...) {
		std::cerr << "Something wrong in PEPKeepAlive::Run()" << std::endl;
	}
	#ifdef DEBUG
	if(debugLevel>=5)
		cerr << "PEPKeepAlive::doRun() as left the building" << endl;
	#endif
}

void PEPKeepAlive::gotKA() 
{ 
	try {
		KAreply.signal();
		#ifdef DEBUG
		if(debugLevel>=9)
			cerr << "KAreply.signal()"<< endl;
		#endif 
	} catch (...) {
		std::cerr << "Something wrong in PEPKeepAlive::gotKA()" << std::endl;
	}
};

PEP::PEP(int dbgLevel,std::string id, std::string pdp_name) : Thread()
{
	debugLevel = dbgLevel;
	clientT = CLIENT_TYPE;
	PepID = id;
	pdp = pdp_name;
	katime = -1;		//infinitum
	accttime = -1;		//infinitum
	keepalive_task=NULL;
	connected = false;

	try {
		Connect(pdp.c_str());
		cerr << "PEP::PEP Connected" << endl;
		start();
	} catch (Socket_Exception e) {
		throw e;
	} catch (COPSmessage_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "PEP::PEP() " << e.what() << endl;
		#endif
		throw Exception(e.what().c_str());
	} catch (...) {
		std::cerr << FG_RED << "Something wrong in PEP::PEP()" << RESET << std::endl;	
	}
	cancel_flag = false;
}


PEP::~PEP() throw()
{
	cancel_flag = true;
	Guard g(&destroy_send);
	if(!solicited.isCanceled())
		solicited.cancel();
	Guard g2(&solicited_receive);
	if(!unsolicited.isCanceled())
		unsolicited.cancel();
	Guard g3(&unsolicited_receive);
	#ifdef DEBUG
	if(debugLevel>=10)
		cerr << "PEP::~PEP()" << endl;
	#endif
	
	try {
		
		if(keepalive_task!=NULL) {	//some times we need to force a clean...
		#ifdef DEBUG
		if(debugLevel>=15)
			cerr << "PEP::~PEP() canceling keepalive_task!" << endl;
		#endif
			keepalive_task->finnished=true;
			keepalive_task->cancel();
			keepalive_task->join();
			delete keepalive_task;
		}
		
		if(connected) {		//clean Exit throw a CLIENT-CLOSE
			#ifdef DEBUG
			if(debugLevel>=10)
				cerr << "PEP::~PEP() Sending CLIENT-CLOSE" << endl;
			#endif
			connected=false;
			COPSmessage close;	
			socket << close.ClientClose(clientT,COPSError::ShuttingDown);
			if(!solicited.isCanceled())
				solicited.cancel();
			if(!unsolicited.isCanceled())
				unsolicited.cancel();
			socket.Close();
		}

	} catch (Exception e) {
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "PEP::~PEP() Exception: " << e.what() << endl;
		#endif	
	} catch (...) {
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "PEP::~PEP() problems...." << endl;	
		#endif	
	}
	#ifdef DEBUG
	if(debugLevel>=16)
		cerr << "PEP::~PEP() DONE" << endl;
	#endif
}

void PEP::run() throw() {
	try {
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "PEP Started (1 sec offset)" << endl;
		sleep(1);	
		#endif	
		while(socket.Poll(katime*1000) && connected && !cancel_flag) {		// we should at least receive a message each KA time or we must be dead :D
			#ifdef DEBUG
			if(debugLevel>=7)
				cerr << "got something" << endl;
			#endif
			COPSmessage msg;
			socket >> msg;
		
			switch(msg) {
				case COPSCommonHeader::KEEP_ALIVE: 
					#ifdef DEBUG
					if(debugLevel>=3)
						cerr << "Received KEEP ALIVE from PDP" << endl;
					#endif
					keepalive_task->gotKA();
					break;	
				case COPSCommonHeader::CLIENT_CLOSE:
					#ifdef DEBUG
					if(debugLevel>=3)
						cerr << "Received CLIENT CLOSE from PDP" << endl;
					#endif
					connected = false;
					throw Socket_Exception("PDP closed remote socket");
					break;
				case COPSCommonHeader::DECISION:
					#ifdef DEBUG
					if(debugLevel>=3)
						cerr << "Received DECISION from PDP" << endl;
					#endif
					if(msg.getCOPSCommonHeader().isUnsolicited()) {
						#ifdef DEBUG
						if(debugLevel>=5)
							cerr << "Unsolicited DECISION" << endl;
						#endif
						unsolicited.add(new COPSmessage(msg));
					}
					else {
						#ifdef DEBUG
						if(debugLevel>=5)
							cerr << "Solicited DECISION" << endl;
						#endif
						solicited.add(new COPSmessage(msg));
					}
					break;
				default:
					solicited.add(new COPSmessage(msg));
				
			}		
		}
		#ifdef DEBUG
		if(debugLevel>=10)
			cerr << "Clean Exit from PEP main loop" << endl;
		#endif
	} catch (COPSmessage_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=3)
			cerr << "Exception " << e.what() << " lead to Close Client" << endl;
		#endif
		COPSmessage report;
       		report.ReportState(CLIENT_TYPE, 0, COPSReportType::Failure);
	        socket << report;	
	} catch (Socket_Exception e) {
		#ifdef DEBUG
		if(debugLevel>=5)
			cerr << FG_RED << "PEP::run() : " <<  e.what() << RESET << endl;
		#endif	
		connected = false;
		try {
			if(!keepalive_task->finnished) {
				keepalive_task->finnished=true;	// no more need for keep alives
				keepalive_task->join();
				if(keepalive_task!=NULL)
					delete keepalive_task;
				keepalive_task = NULL;
			}
			#ifdef DEBUG
			if(debugLevel>=9)
				cerr << FG_RED << " (done)" << RESET << endl;
			#endif	
		} catch (Exception e) {
			#ifdef DEBUG
			if(debugLevel>=5)
				cerr << "Exception trying to cancel keep alive : " << e.what() << endl;
			#endif
		} catch (...) {
			#ifdef DEBUG
			if(debugLevel>=5)
				cerr << "Unknown Exception trying to cancel keep alive : " << endl;
			#endif
		}
		solicited.cancel();
		unsolicited.cancel();
	} catch (...) {
		std::cerr << FG_RED << "Something wrong in PEP::run()" << RESET << std::endl;	
	}
	#ifdef DEBUG
	if(debugLevel>=9)	
		cerr << FG_RED << "PEP::run() all done" << RESET << endl;
	#endif
}

void PEP::Connect(std::string server) 
{
	socket.Connect(server.c_str(),COPS_IP_PROTOCOL);
	
	COPSmessage open;
	socket << open.ClientOpen(clientT,PepID);
	#ifdef DEBUG
	if(debugLevel>=2)
		cerr << "Opened connection with " << server << endl;
	#endif
	COPSmessage reply;
	socket >> reply;
	
	if(reply==COPSCommonHeader::CLIENT_ACCEPT) {
		#ifdef DEBUG
		if(debugLevel>=2)
			cerr << "We were Accepted" << endl;
		#endif
		katime = reply.getCOPSKATimer().getKAtime();
		accttime = reply.getCOPSAcctTimer().getACCTtime();
		keepalive_task = new PEPKeepAlive(debugLevel,&socket,katime);
		keepalive_task->start();
		connected = true;
	} else throw COPSmessage_Exception(COPSError::BadMessageFormat);
}

void PEP::Abort(inbox box) {
	#ifdef DEBUG
	if(debugLevel>=2)
		cerr << "Aborting an inbox!" << endl;
	#endif
	if(box==solicited_msg) {
		solicited.add(NULL);
	} else {
		solicited.add(NULL);
	}
}

COPSmessage PEP::Receive(inbox box) 
{
	COPSmessage *t = NULL;

	try {
		if(box==solicited_msg) {
			Guard g(&solicited_receive);
			t = solicited.next();	//next() leaves you charge of freeing t
			if(t!=NULL) {
				COPSmessage m(*t);
				delete t;		//soo lets clean it ;)
				return m;
			}
		} else {
			Guard g(&unsolicited_receive);
			t = unsolicited.next();
			if(t!=NULL) {
				COPSmessage m(*t);
				delete t;		//soo lets clean it ;)
				return m;
			}
		}
		throw COPSmessage_Exception("Invalid message in inbox");
	} catch(Cancel_Exception e) {
		throw COPSmessage_Exception("Connection with PDP lost");
		/* we expect this since it means PEP exited without leaving any message in inbox */
	} catch(Exception e) {
		std::cerr << FG_RED << e.what() << RESET << std::endl;
	} catch (COPSmessage_Exception e) {
		std::cerr << FG_RED << e.what() << RESET << std::endl;
	} catch (...) {
		std::cerr << FG_RED << "PEP::Receive() big mess" << RESET << std::endl;
	}
	#ifdef DEBUG
	if(debugLevel>=5)
		cerr << "PEP::Receive did not find any message in his inbox" << endl;
	#endif
	return COPSmessage();
}

void PEP::Send(COPSmessage &msg)
{
	Guard g(&destroy_send);
	try {
		socket << msg;
	} catch (Socket_Exception e) {
		std::cerr << e.what() << std::endl;
	}
}
