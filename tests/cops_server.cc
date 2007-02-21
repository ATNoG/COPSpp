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

void Cserver(int d) {
	try {
		PDP s(d,"::1");
		PDPchild *pep;
		while(1) {
			std::cerr << "main: Pick up a new PEP in main" << std::endl;
			pep	= s.unatendedPEP.next();
			std::cerr << "main: Got a new PEP!!" << std::endl;
			bool exit = false;
			while(1) {
				std::cerr << "-> Wait for a msg from PEP!" << std::endl;
				if(exit) break;
				COPSmessage msg(pep->Receive());
				std::cerr << "-> Got a msg from PEP!" << std::endl;
				if(!msg.isValid()) break;
				switch(msg) {
					case COPSCommonHeader::REQUEST:
					{
						std::cerr << FG_MAGENTA << pep->getID() << RESET <<"	PDP: REQUEST" << std::endl;
						if(msg.getCOPSINInt().isIPv6())
							std::cerr << "IN address: " << gethostbyaddr((char *)&msg.getCOPSINInt().getIPv6(),sizeof(in6_addr),AF_INET6)->h_name << " in ifindex=" << msg.getCOPSINInt().getIfIndex() << std::endl;
						if(msg.getCOPSOUTInt().isIPv4())
							std::cerr << "OUT address: " << gethostbyaddr((char *)&msg.getCOPSOUTInt().getIPv4(),sizeof(in_addr),AF_INET)->h_name << " in ifindex=" << msg.getCOPSOUTInt().getIfIndex() << std::endl;
						COPSmessage decide;
						decide.Decision(s.getClientType(),msg.getCOPSHandle().getHandle(),msg.getCOPSContext().getRType(),msg.getCOPSContext().getMType(),COPSDecision::Install);
						long int telm = 917315161;
						COPSDecision decision2(COPSDecision::Client_Specific_Decision_Data,(char *) &telm,sizeof(telm));
						pep->Send(decide+decision2);
						break;
						
					}
					case COPSCommonHeader::REPORT_STATE:
					{
						std::cerr << FG_MAGENTA << pep->getID() << RESET << "	PDP: REPORT" <<std::endl;
						unsigned long int telf;
						COPSClientSI t =	msg.getCOPSClientSI();
						telf = *(unsigned long int *) t.getData();
						std::cerr << "Report: we called " << telf << std::endl;
						break;
					}
					case COPSCommonHeader::DELETE_REQUEST_STATE:
					{
						std::cerr << FG_MAGENTA << pep->getID() << RESET << "	PDP: DELETE REQUEST" <<std::endl;
						COPSmessage ssq;
						ssq.SynchronizeStateRequest(s.getClientType(),1);
						pep->Send(ssq);
						break;
					}
					case COPSCommonHeader::SYNCHRONIZE_COMPLETE:
					{
						std::cerr << FG_MAGENTA << pep->getID() << RESET << "	PDP: SYNCHRONIZE STATE COMPLETE" <<std::endl;
						break;
					}
					case COPSCommonHeader::CLIENT_CLOSE:
						std::cerr << FG_MAGENTA << pep->getID() << RESET << FG_RED << "	PDP: CLOSE" << RESET <<std::endl;
						exit = true;
						break;
					default:
						std::cerr << msg;
				};
			};
			std::cerr << "main: All done! ... let's delete pep" << std::endl;
			delete pep;
		}
		s.join();	
	} catch (Exception e) {
		std::cerr << "main: " << e.what() << std::endl;
	} catch (COPSError e) {
		std::cerr << FG_RED << strErrorCode(e.code()) << RESET << std::endl;
	} catch (...) {
		std::cerr << "something wrong in main" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	Cserver(20);
	return EXIT_SUCCESS;
}
