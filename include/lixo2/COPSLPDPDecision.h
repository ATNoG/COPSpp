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

#ifndef _COPSLPDPDECISION_H_
#define _COPSLPDPDECISION_H_

#include "COPSObj.h"

struct COPSLPDPDecision_Flags {
	unsigned int Command_Code:16;
	unsigned int Flags:16;
};

/**
 2.2.7 LPDP Decision Object (LPDPDecision)

    Decision made by the PEP's local policy decision point (LPDP). May appear in requests. These objects correspond to and are formatted the same as the client specific decision objects defined above.

    \code
	C-Num = 7

	C-Type = (same C-Type as for Decision objects)
    \endcode
 
    see also COPSDecision
 */
class COPSLPDPDecision : public COPSObj
{
	public:
		enum C_Type {
			Decision_Flags = 1,
			Stateless_Data = 2,
			Replacement_Data = 3,
			Client_Specific_Decision_Data = 4,
			Named_Decision_Data = 5
		};
		enum Command_Code {
			INVALID = -1,
			NULL_Decision = 0,	//!< No configuration data available
			Install = 1, 		//!< Admit request/Install configuration
			Remove = 2, 		//!< Remove request / Remove configuration
		};
		/**
		 * Constructor
		 * @param type the type of Decision use C_Type Enum
		 * @param d_data data that will be sent in the decision (can be a COPSObj)
		 * @param d_size size of d_data
		 */
		COPSLPDPDecision(C_Type type, char* d_data, unsigned int d_size);
		/**
		 * Constructor
		 * @param command of the Decision use Command_Code
		 * @param flgs (default value is 0x01)
		 */
		COPSLPDPDecision(Command_Code command, unsigned int flgs=0x01);
		COPSLPDPDecision(const COPSLPDPDecision &obj);
		COPSLPDPDecision &operator=(const COPSLPDPDecision &obj);
		/** Constructor
		 * @param t the type of Decision use C_Type Enum
		 * @param cops_obj Client Specific objects go here
		 */
		COPSLPDPDecision(C_Type t,const COPSObj &cops_obj);
		~COPSLPDPDecision();
		/** Access to the first COPSObj in the Decision
		 * @return returns the first COPSObj in the Decision
		 */
		COPSObj getCOPSObj() const;
		/** 
		 * @return Command Code of the Decision
		 */
		Command_Code getCommand() { return command; };
		/**
		 * @return C_Type of the Decision
		 */
		C_Type getType() { return type; };
		/**
		 * @return Flags of the Decision
		 */
		unsigned int getFlags() { return Flags; };
		/**
		 * @return Size of the Data contained in the Decision
		 */
		unsigned int getSize() { return gSize(); };
		/**
		 * @return Data contained in the Decision, use getSize() to get the size of Data
		 */
		char *getData() { return gData(); };
		/**
		 * The Constructor of COPSLPDPDecision only enables you to insert 1 COPSObj. With this friend function you can add several more COPSObjs following the example:
		 * \code
		 	COPSLPDPDecision dec(Named_Decision_Data,client_si_obj1);
			dec + client_si_obj2 + client_si_obj3;
		\endcode
		 */
		friend COPSLPDPDecision &operator+(COPSLPDPDecision &dcsi, const COPSObj &obj); 
	
		/**
		 * This function template returns a pointer to a object requested through a enum of the desired object class and it's position (default = 1)
		 *  @param type Enum of objects of COPSLPDPDecisionCSI_Type 
                 *  @param n position or the order number of the desired object inside the COPSObj
		 *  @return the desired COPSObj
                 *  @see COPSObj
                 */
		template <class COPSLPDPDecisionCSI_Type> COPSObj getCOPSObj(COPSLPDPDecisionCSI_Type type, unsigned int n) const {
	
			COPSObj obj;
			struct COPSobj_data *tmp;
			try {
				tmp = FindObject<COPSLPDPDecisionCSI_Type>(type,n);
			} catch (COPSObj_Exception e) {
#ifdef DEBUG
				std::cerr << "Couldn't not find any Decision ClientSI object" << std::endl;
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
		Command_Code command;
		unsigned short Flags;
};


#endif	//_COPSDECISION_H_
