/*
 * COPSObj.h 
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

#ifndef _COPSOBJ_H_
#define _COPSOBJ_H_

#include <netinet/in.h>
#include <string>
#include "config.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#ifdef DEBUG_MAX
#include <iostream>
using namespace std;
#endif

class COPSObj_Exception
{
	public:
		enum Errors {
			Invalid_Copy_Constructor_Conversion = 0,
			Invalid_Parameters = 1,
			NoObjectAvailable = 2
		};
		COPSObj_Exception(Errors e) {err = e;};
		~COPSObj_Exception() {};
		
		const std::string what() {
			switch(err) {
				case Invalid_Copy_Constructor_Conversion:
					return "Invalid Copy Constructor Conversion";
					break;
				case Invalid_Parameters:
					return "Invalid Parameters used";
					break;
				case NoObjectAvailable:
					return "Current Object contains no Object";
					break;
				default:
					return "Unknown";
					break;
			}
		};
	private:
		Errors err;
};

struct COPSobj_data {
	unsigned int obj_len:16;
	unsigned int c_num:8;
	unsigned int c_type:8;
};

class COPSmessage;
class COPSClientSI;
class COPSDecision;
class COPSLPDPDecision;

//! COPS Objects base class
/**
 * This Class provides the basic mechanisms for all COPS Objects, such as
 * - Default Constructor
 * - Copy Constructor
 * - Available Objects enum
 * - the DATA of the object
 */

class COPSObj
{	
	public:
		/** Enumeration of available Objects */
		enum C_num {
			Handle = 1,
			Context = 2,
			INInt = 3,
			OUTInt = 4,
			Reason = 5,
			Decision = 6,
			LPDPDecision = 7,
			Error = 8,
			ClientSI = 9,
			KATimer = 10,
			PEPID = 11,
			ReportType = 12,
			PDPRedirAddr = 13,
			LastPDPAddr = 14,
			AcctTimer = 15,
			Integrity = 16
		};
		
		COPSObj(); /**< The Default Constructor MUST be explicitly called by all it's derived classes */
		COPSObj(const COPSObj &obj); /**< The Default Copy Constructor MUST be explicitly called by all it's derived classes */
		~COPSObj();
		
		/** The operaotr+ provides the means to create COPS messages by adding ("+") COPS Objects to a COPSmessage objec 
		 * @param c COPSmessage to where we wish to append the object to
		 * @param obj COPSObj we wish to append
		 * @return COPSmessage with the newly appended COPS object*/
		friend COPSmessage &operator+(COPSmessage &c, const COPSObj &obj); 
		COPSObj &operator=(const COPSObj &); /**< The operator= behaves in a similar way to the copy constructor but for already created objects */
		const unsigned getObjSize() const {return size;}; /**< Returs the size of the COPS Object */
		const char * getObjData() const {return data;}; /**< Returns a const pointer to the COPS Object Data. This function should only be used to peek inside objects or for debug proposes. */
	friend class COPSClientSI; /**< It is declared friend in order to access the data of objects when dealing with Client Specific Information */	
	friend class COPSDecision; /**< It is declared friend in order to access the data of objects when dealing with Client Specific Information */ 
	friend class COPSLPDPDecision; /**< It is declared friend in order to access the data of objects when dealing with Client Specific Information */ 
	protected:
		char *data; /**< byte order pointer to the contents of COPS Object */
		unsigned size; /**< size of char *data; */
			
		/**
		 * This function template returns a pointer to a object requested through a enum of the desired object class and it's position (default = 1)
		 *  @param object Enum of objects of Type
		 *  @param pos position or the order number of the desired object inside the COPSObj
		 *  @return the desired COPSObj
		 *  @see COPSClientSI
		 *  @see COPSDecision
		 */ 
		template<class Type> struct COPSobj_data *  FindObject(Type object, unsigned int pos) const throw(COPSObj_Exception)
		{
			struct COPSobj_data * tmp_i = (struct COPSobj_data *) ((char *)data + sizeof(struct COPSobj_data));
			struct COPSobj_data * tmp = tmp_i;
			char *end = ((char *)data + size);
			unsigned short int offset = 0;
			for(unsigned i=1; i<=pos; i++) {
				if((char *)tmp==end) throw COPSObj_Exception(COPSObj_Exception::NoObjectAvailable); //TODO create other error message
				while(tmp->c_num != (unsigned int) object) {
					offset += ntohs(tmp->obj_len); //calculate offset taking into account current object lenght
					tmp = (struct COPSobj_data *) ((char *)tmp_i + offset); //move to next object
					if((char *)tmp >= end)
						throw COPSObj_Exception(COPSObj_Exception::Invalid_Parameters);
					#ifdef DEBUG
						cerr << "Found an Object, but not the right object " << endl;
					#endif
				}
				if(i==pos) 
					return tmp;
				else {
					#ifdef DEBUG
					cerr << "Found desired object but not in right position, got " << i << " looking for " << pos << endl;
					#endif
					offset += ntohs(tmp->obj_len); //calculate offset taking into account current object lenght
					tmp = (struct COPSobj_data *) ((char *)tmp_i + offset); //move to next object
				}
			}
			#ifdef DEBUG
			cerr << "Did not find Object" << endl;
			#endif
			return tmp;
		}
		/** Calculates the necessary alignment of the message in order to maintain the RFC request to align all objects to 4 bytes 
		 * @param size original size of the object
		 * @return size calculated through padding to 4 bytes
		 * */
		int calculate_padding(int size); 

		unsigned int gSize(); /**< unprotected version of getObjSize() */
		char *gData(); /**< unprotected version of getObjData() */
		
		friend class COPSmessage;
};


#endif	//_COPSOBJ_H_
