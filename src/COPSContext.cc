/*
 * COPSContext.cc 
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

#include "COPSContext.h"

COPSContext::COPSContext(const COPSContext &obj) : COPSObj(obj)
{
	#ifdef DEBUG_MAX
	cerr << "COPSContext::COPSContext(const COPSContext &obj)" << endl;
	#endif
	
	r_type = obj.r_type;
	m_type = obj.m_type;
}

COPSContext &COPSContext::operator=(const COPSContext &obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSContext &COPSContext::operator=(const COPSContext &obj)" << endl;
	#endif
	COPSObj::operator=(obj);
	r_type = obj.r_type;
	m_type = obj.m_type;
	return *this;
}

COPSContext::COPSContext(unsigned short int r_t,unsigned short int m_t) : COPSObj()
{
	#ifdef DEBUG_MAX
	cerr << "COPSContext::COPSContext(unsigned short int r_t,unsigned short int m_t)" << endl;
	#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+4);  //obj hdr + 4
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Context;
	obj->c_type = 1;
	obj->obj_len = htons(size);
	
	unsigned short int t_r_type;
	t_r_type=htons(r_t);
	m_type=htons(m_t);
	memcpy(&data[4],&t_r_type,sizeof(t_r_type));
	memcpy(&data[6],&m_type,sizeof(m_type));
	r_type=(R_Type) r_t;
	m_type=m_t;
}


COPSContext::~COPSContext()
{
	// TODO: put destructor code here
}

COPSContext::R_Type COPSContext::getRType() const {
	return r_type;
}

unsigned short int COPSContext::getMType() const {
	return m_type;
}

bool COPSContext::operator==(COPSContext::R_Type type) {
	if(getRType()&type==type) return true;
	return false;
}
