/*
 * COPSOUTInt.h 
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

#ifndef _COPSOUTINT_H_
#define _COPSOUTINT_H_

#include "COPSObj.h"
/**
 * 2.2.4 Out-Interface Object (OUT-Int)
 *
   The Out-Interface is used to identify the outgoing interface to which
   a specific request applies and the address for where the forwarded
   message is to be sent. For flows or messages destined to the PEP's
   local host, the loop back address and ifindex are used.  The Out-
   Interface has the same formats as the In-Interface Object.

   This Interface object is also used to identify the outgoing
   (forwarding) interface via its ifindex. The ifindex may be used to
   differentiate between sub-interfaces and unnumbered interfaces (see
   RSVP's LIH for an example). When SNMP is supported by the PEP, this
   ifindex integer MUST correspond to the same integer value for the
   interface in the SNMP MIB-II interface index table.

   Note: The ifindex specified in the Out-Interface is typically
   relative to the flow of the underlying protocol messages. The ifindex
   is the one on which a protocol message is about to be forwarded.

\code           C-Num = 4
\endcode
\code
           C-Type = 1, IPv4 Address + Interface

   Same C-Type format as the In-Interface object. The IPv4 address
   specifies the IP address to which the outgoing message is going. The
   ifindex is used to refer to the MIB-II defined local outgoing
   interface on the PEP.
\endcode
\code
           C-Type = 2, IPv6 Address + Interface

   Same C-Type format as the In-Interface object. For this type of the
   interface object, the IPv6 address specifies the IP address to which
   the outgoing message is going. The ifindex is used to refer to the
   MIB-II defined local outgoing interface on the PEP.
\endcode
 *
*/
class COPSOUTInt : public COPSObj
{
	public:
		enum AddressType {
			IP4 = 1,
			IP6 = 2
		};
		/**
		 * Constructor for IPv4 addresses
		 * @param ip4 IPv4 address
		 * @param ifind interface index
		 */
		COPSOUTInt(const in_addr *ip4, unsigned int ifind);
		/**
		 * Constructor for IPv6 addresses
		 * @param ip6 IPv6 address
		 * @param ifind interface index
		 */
		COPSOUTInt(const in6_addr *ip6, unsigned int ifind);
		COPSOUTInt(const COPSOUTInt &obj); 
		COPSOUTInt &operator=(const COPSOUTInt &obj);
		~COPSOUTInt();
		in_addr getIPv4(); /**< returns the IPv4 address of the object, should be tested with isIPv4() previously */
		in6_addr getIPv6(); /**< returns the IPv6 address of the object, should be tested with isIPv6() previously */
		bool isIPv4() const; /**< tests if the Object carries and IPv4 address */
		bool isIPv6() const; /**< tests if the Object carries and IPv6 address */
		/**
		 * Returns the Interface Index
		 * @return Interface Index
		 */
		unsigned int getIfIndex() {return ifindex; };
	
	protected:
		unsigned int ifindex;
		union {
			in_addr ipv4;
			in6_addr ipv6;
		};
};
#endif	//_COPSOUTINT_H_
