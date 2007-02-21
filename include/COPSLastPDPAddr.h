/*
 * COPSLastPDPAddr.h 
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

#ifndef _COPSLASTPDPADDR_H_
#define _COPSLASTPDPADDR_H_

#include "COPSObj.h"

/**
 2.2.14 Last PDP Address (LastPDPAddr)

   When a PEP sends a Client-Open message for a particular client-type
   the PEP SHOULD specify the last PDP it has successfully opened
   (meaning it received a Client-Accept) since the PEP last rebooted.
   If no PDP was used since the last reboot, the PEP will simply not
   include this object in the Client-Open message.
\code
       C-Num = 14,

       C-Type = 1, IPv4 Address (Same format as PDPRedirAddr)

       C-Type = 2, IPv6 Address (Same format as PDPRedirAddr)
       \endcode
 */

class COPSLastPDPAddr : public COPSObj
{
	public:
		enum AddressType {
			IP4 = 1,
			IP6 = 2
		};
		/**
		 * Constructor for IPv4 addresses
		 * @param ip4 in_addr containing the server ip where we are being redirected
		 * @param port TCP port where the new PDP is listenning
		 */		
		COPSLastPDPAddr(in_addr *ip4, unsigned short int port);
		/**
		 * Constructor for IPv6 addresses
		 * @param ip6 in6_addr containing the server ip where we are being redirected
		 * @param port TCP port where the new PDP is listenning
		 */		
		COPSLastPDPAddr(in6_addr *ip6, unsigned short int port);
		COPSLastPDPAddr(const COPSLastPDPAddr &);
		COPSLastPDPAddr &operator=(const COPSLastPDPAddr &);
		~COPSLastPDPAddr();

		/**
		 * retrieves the IPv4 address of the new PDP. ATTENTION! You should first test if the object contains this type of address using isIPv4()
		 * return in_addr of new PDP
		 */
		in_addr getIPv4();
		/**
		 * retrieves the IPv6 address of the new PDP. ATTENTION! You should first test if the object contains this type of address using isIPv6()
		 * return in6_addr of new PDP
		 */
		in6_addr getIPv6();
		/**
		 * @return indication if object contains a IPv4 address or not
		 */
		bool isIPv4() const;
		/**
		 * @return indication if object contains a IPv6 address or not
		 */
		bool isIPv6() const;
		/**
		 * @return the TCP port where the new PDP lies
		 */
		unsigned int getPort() {return port; };
	
	protected:
		unsigned short int port;
		union {
			in_addr ipv4;
			in6_addr ipv6;
		};
	
};


#endif	//_COPSLASTPDPADDR_H_
