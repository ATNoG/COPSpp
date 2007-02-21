/*
 * COPSClientSI.h 
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

#ifndef _COPSCLIENTSI_H_
#define _COPSCLIENTSI_H_

#include <COPSObj.h>
#include <utility>

/**
2.2.9 Client Specific Information Object (ClientSI)

   The various types of this object are required for requests, and used in reports and opens when required. It contains client-type specific information.

   \code
   C-Num = 9,
   \endcode

   \code
   C-Type = 1, Signaled ClientSI.
   \endcode

   Variable-length field. All objects/attributes specific to a client's signaling protocol or internal state are encapsulated within one or more signaled Client Specific Information Objects. The format of the data encapsulated in the ClientSI object is determined by the client-type.
   \code 
   C-Type = 2, Named ClientSI.
   \endcode
   Variable-length field. Contains named configuration information useful for relaying specific information about the PEP, a request, or configured state to the PDP server.
 */

class COPSClientSI : public COPSObj
{
	public:
		enum C_Type {
			Signaled = 1,		//!< Objects/Attributes specific to a client's signaling protocol or internal state
			Named = 2			//!< Configuration Information useful for relaying specific information about the PEP, a request, or configured state to the PDP server
		};
		/**
		 * Constructor
		 * @param type Indicates the type of COPSClientSI we are dealing with
		 * @param client_data Byte align information. This is usually a COPSObj child.
		 * @param client_data_size The size of client_data information
		 */
		COPSClientSI(C_Type type, char *client_data, unsigned int client_data_size);
		/**
		 * Constructor
		 * This constructor produces a ClientSI taking as Client Specific Information a COPSObj
		 * @param type Indicates the type of COPSClientSI we are dealing with
		 * @param obj Client Specific COPS Object
		 */
		COPSClientSI(C_Type type,const COPSObj &obj);
		/**
		 * Constructor
		 * This constructor creates a COPSClientSI with a specific type but with no data
		 * @param type Indicates the type of COPSClientSI we are dealing with
		 */
		COPSClientSI(C_Type type);
		~COPSClientSI();
		COPSClientSI(const COPSClientSI &obj);
		COPSClientSI &operator=(const COPSClientSI &obj);
		/**
		 * the Type of Client SI we are dealing with (Signaled or Named)
		 * @return Type of Client SI
		 */
		C_Type getType() {return type; };
		/**
		 * Size of the ClientSI Object Information. This information might be usefull to parse ClientSI objects with several objects inside
		 * @return size of ClientSI object
		 */
		unsigned int getSize() { return gSize(); };
		/**
		 * Information contained in Client SI Object
		 * Please note that it returns a pointer to a char * of size equal to the value returned by getSize()
		 * @return information contained in ClientSI
		 */
		char *getData() { return gData(); };
		/**
		 * In case the ClientSi contains a COPSObj, this method will return the object
		 * @return Client Specific Object
		 */
		COPSObj getCOPSObj() const;
		/**
		 * The Constructor of COPSClientSI only enables you to insert 1 COPSObj. With this friend function you can add several more COPSObjs following the example:
		 \code
		 COPSClientSI dec(Named,client_si_obj1);
		 dec + client_si_obj2 + client_si_obj3;
		 \endcode
		 */
		friend COPSClientSI &operator+(COPSClientSI &csi, const COPSObj &obj);
		/**
                 * This function template returns a pointer to a object requested through a enum of the desired object class and it's position (default = 1)
                 *  @param type Enum of objects of COPSClientSI_Type
                 *  @param n position or the order number of the desired object inside the COPSObj
                 *  @return the desired COPSObj
                 *  @see COPSObj
                 */	
		template <class COPSClientSI_Type> COPSObj getCOPSObj(COPSClientSI_Type type, unsigned int n) const {

			COPSObj obj;
			struct COPSobj_data *tmp;
			try {
				tmp = FindObject<COPSClientSI_Type>(type,n);
			} catch (COPSObj_Exception e) {
#ifdef DEBUG
				std::cerr << "Couldn't not find any ClientSI object" << std::endl;
#endif
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
