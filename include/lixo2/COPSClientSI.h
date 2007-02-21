/* 
 * COPSClientSI.h
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

#ifndef _COPSCLIENTSI_H_
#define _COPSCLIENTSI_H_

#include <Copspp/COPSObj.h>
#include <utility>

class COPSClientSI : public COPSObj
{
	public:
		enum C_Type {
			Signaled = 1,		// Objects/Attributes specific to a client's signaling protocol or internal state
			Named = 2			// Configuration Information useful for relaying specific information about the PEP, a request, or configured state to the PDP server
		};
		COPSClientSI(C_Type type, char *client_data, unsigned int client_data_size);
		COPSClientSI(const COPSClientSI &obj);
		COPSClientSI &operator=(const COPSClientSI &obj);
		 ~COPSClientSI();
		COPSClientSI(C_Type t,const COPSObj &obj);
		COPSClientSI(C_Type t);
		C_Type getType() {return type; };
		unsigned int getSize() { return gSize(); };
		char *getData() { return gData(); };
		COPSObj getCOPSObj() const;
		friend COPSClientSI &operator+(COPSClientSI &csi, const COPSObj &obj);
		template <class COPSClientSI_Type> COPSObj getCOPSObj(COPSClientSI_Type type, unsigned int n) const {

			COPSObj obj;
			struct COPSobj_data *tmp;
			try {
				tmp = FindObject<COPSClientSI_Type>(type,n);
			} catch (COPSObj_Exception e) {
				throw e;
			}

			obj.size = ntohs(tmp->obj_len);
			obj.data = new char[obj.size];
			memcpy(obj.data,tmp,obj.size);

			return obj;
		}

	protected:
		C_Type type;
};


#endif	//_COPSCLIENTSI_H_
