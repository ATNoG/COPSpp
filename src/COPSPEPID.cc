/*
 * COPSPEPID.cc 
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

#include "COPSPEPID.h"

COPSPEPID::COPSPEPID(const COPSPEPID &p) : COPSObj(p) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSPEPID::COPSPEPID(const COPSPEPID &p)" << endl;
	#endif
	id = p.id;
};

COPSPEPID &COPSPEPID::operator=(const COPSPEPID &p) 
{
	#ifdef DEBUG_MAX
	cerr << "COPSPEPID &COPSPEPID::operator=(const COPSPEPID &p)" << endl;
	#endif
	COPSObj::operator=(p);
	id = p.id;
	return *this;
};

COPSPEPID::COPSPEPID() : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSPEPID::COPSPEPID() " << endl;
#endif
	id = "";
}

COPSPEPID::COPSPEPID(std::string identifier) : COPSObj()
{
#ifdef DEBUG_MAX
	cerr << "COPSPEPID::COPSPEPID(std::string identifier)" << endl;
#endif
	size = calculate_padding(sizeof(struct COPSobj_data)+identifier.size()+1);  //obj hdr + (identifier + \0)
	id = identifier;
	data= new char[size];
	memset(data,0,size);
	
	struct COPSobj_data *obj;
	obj = ((struct COPSobj_data *) data);
	obj->c_num = PEPID;
	obj->c_type = 1;
	obj->obj_len = htons(size);
	
	memcpy(&data[4],id.c_str(),id.size());
	char null_char  = '\0';
	memcpy(&data[4+id.size()],&null_char,1);
}

COPSPEPID::~COPSPEPID()
{

}
