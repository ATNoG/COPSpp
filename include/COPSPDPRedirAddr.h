/*
 * COPSPDPRedirAddr.h
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

#ifndef _COPSPDPREDIRADDR_H_
#define _COPSPDPREDIRADDR_H_

#include "COPSObj.h"

/**
2.2.13 PDP Redirect Address (PDPRedirAddr)

   A PDP when closing a PEP session for a particular client-type may
   optionally use this object to redirect the PEP to the specified PDP
   server address and TCP port number:
\code
       C-Num = 13,

       C-Type = 1, IPv4 Address + TCP Port
                0             1              2             3
       +--------------+--------------+--------------+--------------+
       |                   IPv4 Address format                     |
       +--------------+--------------+--------------+--------------+
       |  -------------------------  |       TCP Port Number       |
       +-----------------------------+-----------------------------+

       C-Type = 2, IPv6 Address + TCP Port
                0             1              2             3
       +--------------+--------------+--------------+--------------+
       |                                                           |
       +                                                           +
       |                                                           |
       +                    IPv6 Address format                    +
       |                                                           |
       +                                                           +
       |                                                           |
       +--------------+--------------+--------------+--------------+
       |  -------------------------  |       TCP Port Number       |
       +-----------------------------+-----------------------------+
\endcode
 */
class COPSPDPRedirAddr : public COPSObj
{
	public:
		enum AddressType {
			IP4 = 1,	//!< Address used is IPv4
			IP6 = 2		//!< Address used is IPv6
		};
		/**
		 * Constructor for IPv4 addresses
		 * @param ip4 in_addr containing the server ip where we are being redirected
		 * @param port TCP port where the new PDP is listenning
		 */
		COPSPDPRedirAddr(in_addr *ip4, unsigned short int port);
		/**
		 * Constructor for IPv6 addresses
		 * @param ip6 in6_addr containing the server ip where we are being redirected
		 * @param port TCP port where the new PDP is listenning
		 */
		COPSPDPRedirAddr(in6_addr *ip6, unsigned short int port);
		COPSPDPRedirAddr(const COPSPDPRedirAddr &);
		COPSPDPRedirAddr& operator=(const COPSPDPRedirAddr &);
		~COPSPDPRedirAddr();
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


#endif	//_COPSPDPREDIRADDR_H_
