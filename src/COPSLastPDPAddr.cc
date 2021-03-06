/*
 * COPSLastPDPAddr.cc 
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

#include "COPSLastPDPAddr.h"

COPSLastPDPAddr::COPSLastPDPAddr(const COPSLastPDPAddr &obj) : COPSObj(obj)
{
#ifdef DEBUG_MAX
	cerr << "COPSLastDPDAddr::COPSLastPDPAddr(const COPSLastPDP &obj) " << endl;
#endif
	if(obj.isIPv4())
		memcpy(&ipv4,&obj.ipv4,sizeof(in_addr));
	if(obj.isIPv6())
		memcpy(&ipv6,&obj.ipv6,sizeof(in6_addr));
	port = obj.port;
}

COPSLastPDPAddr &COPSLastPDPAddr::operator=(const COPSLastPDPAddr &obj) 
{
#ifdef DEBUG_MAX
	cerr << "COPSLastPDPAddr &COPSLastPDPAddr::operator=(const COPSLastPDPAddr &obj)" << endl;
#endif
	COPSObj::operator=(obj);
	if(obj.isIPv4())
		memcpy(&ipv4,&obj.ipv4,sizeof(in_addr));
	if(obj.isIPv6())
		memcpy(&ipv6,&obj.ipv6,sizeof(in6_addr));
	port = obj.port;
	return *this;
}

COPSLastPDPAddr::COPSLastPDPAddr(in_addr *ip4, unsigned short int prt) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr <<"COPSLastPDPAddr::COPSLastPDPAddr(in_addr *ip4, unsigned short int prt)" << endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+sizeof(in_addr)+4);  //obj hdr + IPv4 Address + 4
	data= new char[size];
	memset(&ipv6,0,sizeof(in6_addr));
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = LastPDPAddr;
	obj->c_type = IP4; //ipv4
	obj->obj_len = htons(size);
	
	memcpy(&data[4],ip4,sizeof(in_addr));
	memcpy(&ipv4,ip4,sizeof(in_addr));
	port = htons(prt);
	memcpy(&data[10],&port,sizeof(unsigned short int));
	port = prt;
}

COPSLastPDPAddr::COPSLastPDPAddr(in6_addr *ip6, unsigned short int prt) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSLastPDPAddr::COPSLastPDPAddr(in6_addr *ip6, unsigned short int prt) " <<endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+sizeof(in6_addr)+4);  //obj hdr + IPv6 Address + 4
	data= new char[size];
	memset(&ipv4,0,sizeof(in_addr));
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = LastPDPAddr;
	obj->c_type = IP6; //ipv6
	obj->obj_len = htons(size);
	
	memcpy(&data[4],ip6,sizeof(in6_addr));
	memcpy(&ipv6,ip6,sizeof(in6_addr));
	port = htons(prt);
	memcpy(&data[22],&port,sizeof(unsigned short int));
	port = prt;
}

in_addr COPSLastPDPAddr::getIPv4() {
	return ipv4;
}

in6_addr COPSLastPDPAddr::getIPv6() {
	return ipv6;
}

bool COPSLastPDPAddr::isIPv4() const {
	if(((struct COPSobj_data *) data)->c_type == IP4) return true;
	return false;
}

bool COPSLastPDPAddr::isIPv6() const {
	if(((struct COPSobj_data *) data)->c_type == IP6) return true;
	return false;
}

COPSLastPDPAddr::~COPSLastPDPAddr()
{
	// TODO: put destructor code here
}
