/* Created by Anjuta version 1.1.97 */
/*	This file will not be overwritten */

#include <iostream>
#include <unistd.h>
#include <SocketClient.h>
#include <SocketServer.h>
#include <PDP.h>
#include <PEP.h>
#include "color.h"

using namespace std;

void Cclient(char *id, int d) {
	//lerr<< setDebugLevel(d);
//	while(1) {
	{
		try {
			std::cerr << "client start" << std::endl;
			PEP c( d,id,"::1");	
			
			std::cerr << "Configuration request start" << std::endl;
			COPSmessage reqconf;
			reqconf.Request(c.getClientType(),1,COPSContext::ConfigurationRequest);
			COPSINInt inint((in6_addr *) gethostbyname2("www.kame.net",AF_INET6)->h_addr,69);
			COPSOUTInt outint((in_addr *) gethostbyname2("www.ua.pt",AF_INET)->h_addr,99);
			std::cerr << "Configuration requested" << std::endl;
			c.Send(reqconf+inint+outint);
			std::cerr << "Configuration received" << std::endl;
			bool conn = true;
			while(conn) {
				COPSmessage msg = c.Receive(c.solicited_msg);
				switch(msg) {
					case COPSCommonHeader::DECISION: 
						std::cerr << "Received a Decision" << std::endl;	
						switch(msg.getCOPSDecision().getCommand()) {
							case COPSDecision::Install:
							{					
								std::cerr << "Install" << std::endl;
								long int tmp;
								tmp = * (long int *) msg.getCOPSDecision(2).getData();
								std::cerr << ">"<< tmp << "<" << std::endl;
								COPSmessage report;
								unsigned long int telefone = 289801818;
								COPSClientSI clientsi(COPSClientSI::Named,(char *) &telefone,sizeof(telefone));
								report.ReportState(c.getClientType(),1,COPSReportType::Success);
								c.Send(report+clientsi);
								std::cerr << "Report Sent" << std::endl;
								sleep(5);
								COPSmessage drq;
								drq.DeleteRequestState(c.getClientType(),1,COPSReason::Timeout);
								c.Send(drq);
							}
						}
						break;
					case COPSCommonHeader::SYNCHRONIZE_STATE_REQ:
						{
							std::cerr << "Received a Synchronize State Request" << std::endl;	
							COPSmessage ssc;
							ssc.SynchronizeStateComplete(c.getClientType(),msg.getCOPSHandle().getHandle());
							c.Send(ssc);
							sleep(10);
							conn = false;		
						}
						break;
				}
			}		
			std::cerr << "Closing connection with PDP" <<std::endl;
			//COPSmessage close;
			//c.Send(close.ClientClose(c.getClientType(),COPSError::ShuttingDown));			
			
			
		} catch (Socket_Exception e) {
			std::cerr << FG_RED <<  e.what() << RESET << std::endl;		
		} catch (COPSmessage_Exception e) {
			std::cerr << FG_MAGENTA <<  e.what() << RESET << std::endl;		
		} catch (Exception e) {
			std::cerr<< e.what() << std::endl;
		} catch (...) {
			std::cerr<< "something wrong Cclient" << std::endl;
		}
		std::cerr << "client done" << std::endl;
		sleep(3);	// delay until next reconnect
	}
	//} // while
	std::cerr<< "THE END :)" << std::endl;
}

int main(int argc, char *argv[])
{
	Cclient("copspp_pep",20);
	return EXIT_SUCCESS; 
}
