/* 
 * COPSPDPRedirAddr.h
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

#ifndef _COPSPDPREDIRADDR_H_
#define _COPSPDPREDIRADDR_H_

#include <Copspp/COPSObj.h>

class COPSPDPRedirAddr : public COPSObj
{
	public:
		enum AddressType {
			IP4 = 1,
			IP6 = 2
		};
		COPSPDPRedirAddr(in_addr *ip4, unsigned short int port);
		COPSPDPRedirAddr(in6_addr *ip6, unsigned short int port);
		COPSPDPRedirAddr(const COPSPDPRedirAddr &);
		COPSPDPRedirAddr& operator=(const COPSPDPRedirAddr &);
		~COPSPDPRedirAddr();
		in_addr getIPv4();
		in6_addr getIPv6();
		bool isIPv4() const;
		bool isIPv6() const;
		unsigned int getPort() {return port; };
	
	protected:
		unsigned short int port;
		union {
			in_addr ipv4;
			in6_addr ipv6;
		};
	
};


#endif	//_COPSPDPREDIRADDR_H_
