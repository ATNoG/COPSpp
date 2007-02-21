/*
 * COPSClientSI.cc 
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

#include "COPSClientSI.h"

COPSClientSI::COPSClientSI(const COPSClientSI &obj) : COPSObj(obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSClientSI::COPSClientSI(const COPSClientSI &obj)" << endl;
	#endif
	type = obj.type; 
};

COPSClientSI &COPSClientSI::operator=(const COPSClientSI &obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSClientSI &COPSClientSI::operator=(const COPSClientSI &obj)" << endl;
	#endif
	COPSObj::operator=(obj);
	type = obj.type;
	return *this;
};

COPSClientSI::COPSClientSI(C_Type t)
{
	size = calculate_padding(sizeof(struct COPSobj_data));  
	type = t;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = ClientSI;
	obj->c_type = (unsigned short int) type;
	obj->obj_len = htons(size);
}

COPSClientSI::COPSClientSI(C_Type t,const COPSObj &cops_obj)
{
	size = calculate_padding(sizeof(struct COPSobj_data)+cops_obj.size);  
	type = t;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = ClientSI;
	obj->c_type = (unsigned short int) type;
	obj->obj_len = htons(size);
	
	memcpy(&data[4],cops_obj.data,cops_obj.size);
	
}

COPSObj COPSClientSI::getCOPSObj() const
{
	COPSObj obj;
	// 4 is COPSClientSI header we need to read COPSObj header
	obj.size = size-4;
	obj.data = new char[obj.size];
	memcpy(obj.data,&data[4],obj.size);

	return obj;
}

COPSClientSI::COPSClientSI(C_Type t, char *client_data, unsigned int client_data_size) : COPSObj()
{
	#ifdef DEBUG_MAX
	cerr << "COPSClientSI::COPSClientSI(C_Type t, char *client_data, unsigned int client_data_size)" << endl;
	#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+client_data_size);  //obj hdr + 4
	type = t;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = ClientSI;
	obj->c_type = (unsigned short int) type;
	obj->obj_len = htons(size);
	
	
	memcpy(&data[4],client_data,client_data_size);
}

COPSClientSI &operator+(COPSClientSI &csi, const COPSObj &obj) {
	unsigned size = csi.getObjSize()+obj.getObjSize();
	char *tmp;
	tmp = new char[size];
	memcpy(tmp,csi.data,csi.getObjSize());
	memcpy(tmp+csi.size,obj.getObjData(), obj.getObjSize());
	csi.size=size;
	delete [] csi.data;
	csi.data = tmp;
	((struct COPSobj_data *) csi.data)->obj_len = htons(size);
	return csi;
}

COPSClientSI::~COPSClientSI()
{
}
