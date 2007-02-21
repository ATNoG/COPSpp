/*
 * COPSReason.cc 
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

#include "COPSReason.h"

COPSReason::COPSReason(const COPSReason &obj) : COPSObj(obj)
{
#ifdef DEBUG_MAX
cerr << "COPSReason::COPSReason(const COPSReason &obj)" << endl;
#endif
	code = obj.code;
	sub_code = obj.sub_code;
}

COPSReason &COPSReason::operator=(const COPSReason &obj)
{
#ifdef DEBUG_MAX
cerr << "COPSReason &COPSReason::operator=(const COPSReason &obj)" << endl;
#endif
	COPSObj::operator=(obj);
	code = obj.code;
	sub_code = obj.sub_code;
	return *this;
}

COPSReason::COPSReason(Reason_Code cod, unsigned int sub_cod) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSReason::COPSReason(Reason_Code cod, unsigned int sub_cod)" << endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+4);  //obj hdr + 4
	code = cod;
	sub_code = sub_cod;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data * obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = Reason;
	obj->c_type = 1;
	obj->obj_len = htons(size);
	
	unsigned short int ReasonCode,ReasonSubCode;
	ReasonCode = htons(code);
	ReasonSubCode = htons(sub_code);
	memcpy(&data[4],&ReasonCode,sizeof(ReasonCode));
	memcpy(&data[6],&ReasonSubCode,sizeof(ReasonSubCode));
}


COPSReason::~COPSReason()
{
	// TODO: put destructor code here
}
