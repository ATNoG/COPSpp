/*
 * COPSOUTInt.cc 
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

#include "COPSOUTInt.h"

COPSOUTInt::COPSOUTInt(const COPSOUTInt &obj) : COPSObj(obj) 
{
#ifdef DEBUG_MAX
	cerr << "OPSOUTInt::COPSOUTInt(const COPSOUTInt &obj) " << endl;
#endif
	ifindex = obj.ifindex; 
	if(obj.isIPv4()) memcpy(&ipv4,&(obj.ipv4),sizeof(in_addr));
	if(obj.isIPv6()) memcpy(&ipv6,&(obj.ipv6),sizeof(in6_addr));
}

COPSOUTInt &COPSOUTInt::operator=(const COPSOUTInt &obj) 
{
#ifdef DEBUG_MAX
	cerr << "COPSOUTInt &COPSOUTInt::operator=(const COPSOUTInt &obj)" << endl;
#endif
	COPSObj::operator=(obj);
	ifindex = obj.ifindex; 
	if(obj.isIPv4()) memcpy(&ipv4,&(obj.ipv4),sizeof(in_addr));
	if(obj.isIPv6()) memcpy(&ipv6,&(obj.ipv6),sizeof(in6_addr));
	return *this;
}

COPSOUTInt::COPSOUTInt(const in_addr *ip4, unsigned int ifind) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSOUTInt::COPSOUTInt(in_addr *ip4, unsigned int ifind) " << endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+sizeof(in_addr)+sizeof(unsigned int));  //obj hdr + 4
	data= new char[size];
	memset(&ipv6,0,sizeof(in6_addr));
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = OUTInt;
	obj->c_type = IP4; //ipv4
	obj->obj_len = htons(size);
	
	memcpy(&data[4],ip4,sizeof(in_addr));
	memcpy(&ipv4,ip4,sizeof(in_addr));
	ifindex = htonl(ifind);
	memcpy(&data[8],&ifindex,sizeof(unsigned int));
	ifindex = ifind;
}

COPSOUTInt::COPSOUTInt(const in6_addr *ip6, unsigned int ifind) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSOUTInt::COPSOUTInt(in6_addr *ip6, unsigned int ifind) " << endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+sizeof(in6_addr)+sizeof(unsigned int));  //obj hdr + 4
	data= new char[size];
	memset(&ipv4,0,sizeof(in_addr));
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = OUTInt;
	obj->c_type = IP6; //ipv6
	obj->obj_len = htons(size);
	
	memcpy(&data[4],ip6,sizeof(in6_addr));
	memcpy(&ipv6,ip6,sizeof(in6_addr));
	ifindex = htonl(ifind);
	memcpy(&data[20],&ifindex,sizeof(unsigned int));
	ifindex = ifind;
}

in_addr COPSOUTInt::getIPv4() {
	return ipv4;
}

in6_addr COPSOUTInt::getIPv6() {
	return ipv6;
}

bool COPSOUTInt::isIPv4() const {
	if(((struct COPSobj_data *) data)->c_type == IP4) return true;
	return false;
}

bool COPSOUTInt::isIPv6() const {
	if(((struct COPSobj_data *) data)->c_type == IP6) return true;
	return false;
}

COPSOUTInt::~COPSOUTInt()
{
	// TODO: put destructor code here
}
