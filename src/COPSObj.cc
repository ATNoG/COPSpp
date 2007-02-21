/*
 * COPSObj.cc 
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

#include "COPSObj.h"


COPSObj::COPSObj()
{
	#ifdef DEBUG_MAX
	std::cerr << "COPSObj::COPSObj() = "<< std::hex << this << std::dec << std::endl;
	#endif
	size=0;
	data = NULL;
}

COPSObj::COPSObj(const COPSObj &obj)
{
	#ifdef DEBUG_MAX
	std::cerr << "COPSObj::COPSObj(const COPSObj &) "<< std::hex << this <<  "= "<< &obj << std::dec << std::endl;
	#endif
	size=obj.size;
	if(size>0) {
		data = new char[size];
		memcpy(data,obj.data,size);	
	} else {
		data = NULL;
	};
}

COPSObj &COPSObj::operator=(const COPSObj &obj)
{
	#ifdef DEBUG_MAX
	std::cerr << "COPSObj &COPSObj::operator=(const COPSObj &obj) "<< std::hex << this <<  "= "<< &obj << std::dec << std::endl;
	#endif
	if(this!=&obj) {	// avoid self assignment
		size=obj.size;
		if(data!=NULL) data=NULL;
		//if(data!=NULL) delete [] data; //TODO see below
		data = new char[size];
		memcpy(data,obj.data,size);
	}
	return *this;
}


COPSObj::~COPSObj()
{
	#ifdef DEBUG_MAX
	std::cerr << "COPSObj::~COPSObj()= "<< std::hex << this << std::dec << std::endl;
	#endif
	if(data!=NULL)
//		data=NULL;
		delete [] data;  //TODO figure whats woring here...
	data = NULL;
}

int COPSObj::calculate_padding(int size) {
	int d;
	if((d=size%4)==0) {
		return size;
	} else {
		return size+(4-d);
	}
}

unsigned int COPSObj::gSize()
{ 
	return (size - sizeof(struct COPSobj_data));
}

char *COPSObj::gData()
{ 
	return ((char *)data+ sizeof(struct COPSobj_data)); 
}

