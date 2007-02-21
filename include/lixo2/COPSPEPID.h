/* 
 * COPSPEPID.h
 *
 * Copyright (C) 2004 Universidade Aveiro - Instituto de Telecomunicacoes Polo A
veiro
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
 * Authors:     Diogo Gomes, <dgomes@av.it.pt>
 */

#ifndef _COPSPEPID_H_
#define _COPSPEPID_H_

#include <Copspp/COPSObj.h>

class COPSPEPID : public COPSObj
{
	public:
		COPSPEPID();
		COPSPEPID(std::string identifier);
		COPSPEPID(const COPSPEPID &);
		COPSPEPID &operator=(const COPSPEPID &);
		~COPSPEPID();
		std::string getId() {return id;};
	
	protected:
		std::string id;
	
};


#endif	//_COPSPEPID_H_
