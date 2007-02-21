/*
 * COPSReportType.cc 
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

#include "COPSReportType.h"

COPSReportType::COPSReportType(const COPSReportType &obj) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSReportType::COPSReportType(const COPSReportType &obj)" << endl;
#endif
	type = obj.type;
}

COPSReportType &COPSReportType::operator=(const COPSReportType &obj) 
{
#ifdef DEBUG_MAX
	cerr << "COPSReportType &COPSReportType::operator=(const COPSReportType &obj)" << endl;
#endif
	COPSObj::operator=(obj);
	type = obj.type;
	return *this;
}

COPSReportType::COPSReportType(Report_Type t) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSReportType::COPSReportType(Report_Type t)"<< endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+4);  //obj hdr + 4
	type = t;
	data = new char[size];
	memset(data,0,size);

	struct COPSobj_data *obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = ReportType;
	obj->c_type = 1;
	obj->obj_len = htons(size);

	type = (Report_Type) htons(t);
	memcpy((&data[4]),&type,sizeof(type));
	type = t;
}


COPSReportType::~COPSReportType()
{

}
