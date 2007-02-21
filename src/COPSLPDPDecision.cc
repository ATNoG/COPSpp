/*
 * COPSLPDPDecision.h 
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

#include "COPSLPDPDecision.h"

COPSLPDPDecision::COPSLPDPDecision(const COPSLPDPDecision &obj) : COPSObj(obj) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSLPDPDecision::COPSLPDPDecision(const COPSLPDPDecision &obj)" << endl;
	#endif
	type=obj.type; 
	command=obj.command; 
	Flags=obj.Flags;
};

COPSLPDPDecision &COPSLPDPDecision::operator=(const COPSLPDPDecision &obj)
{
	#ifdef DEBUG_MAX
	cerr << "COPSLPDPDecision &COPSLPDPDecision::operator=(const COPSLPDPDecision &obj)" << endl;
	#endif
	COPSObj::operator=(obj);
	type=obj.type; 
	command=obj.command; 
	Flags=obj.Flags;
	return *this;
}

COPSLPDPDecision::COPSLPDPDecision(C_Type t,const COPSObj &cops_obj)
{
	size = calculate_padding(sizeof(struct COPSobj_data)+cops_obj.size);
	type = t;
	data= new char[size];
	memset(data,0,size);

	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Decision;
	obj->c_type = (unsigned short int) type;
	obj->obj_len = htons(size);

	memcpy(&data[4],cops_obj.data,cops_obj.size);

}

COPSObj COPSLPDPDecision::getCOPSObj() const
{
	COPSObj obj;
	// 4 is COPSClientSI header we need to read COPSObj header
	if(size<=4) throw COPSObj_Exception(COPSObj_Exception::NoObjectAvailable);	
	obj.size = size-4;
	obj.data = new char[obj.size];
	memcpy(obj.data,&data[4],obj.size);

	 return obj;
}

COPSLPDPDecision::COPSLPDPDecision(C_Type t, char* d_data, unsigned int d_size) : COPSObj()
{
	#ifdef DEBUG_MAX
	cerr << "COPSLPDPDecision::COPSLPDPDecision(C_Type t, char* d_data, unsigned int d_size)" << endl;
	#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+d_size);  //obj hdr + 4
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Decision;
	obj->c_type = t;
	obj->obj_len = htons(size);
	
	type = t;
	command = INVALID;
	Flags = 0;
	memcpy(&data[4],d_data,d_size);
}

COPSLPDPDecision::COPSLPDPDecision(Command_Code c, unsigned int flgs) : COPSObj()
{
	#ifdef DEBUG_MAX
	cerr << "COPSLPDPDecision::COPSLPDPDecision(Command_Code c, unsigned int flgs)" << endl;
	#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+4);  //obj hdr + 4
	if(data!=NULL) delete data;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Decision;
	obj->c_type = Decision_Flags;
	obj->obj_len = htons(size);
	
	command = (Command_Code) htons(c);
	Flags = htons(flgs);
	memcpy(&data[4],&command,sizeof(command));
	memcpy(&data[6],&Flags,sizeof(Flags));
	
	type = Decision_Flags;
	command = c;
	Flags = flgs;
	
}

COPSLPDPDecision::~COPSLPDPDecision()
{

}

COPSLPDPDecision &operator+(COPSLPDPDecision &csi, const COPSObj &obj) {
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

