/*
 * COPSDecision.h 
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

#ifndef _COPSDECISION_H_
#define _COPSDECISION_H_

#include "COPSObj.h"

struct COPSDecision_Flags {
	unsigned int Command_Code:16;
	unsigned int Flags:16;
};

/**
 *
 * 2.2.6 Decision Object (Decision)

   Decision made by the PDP. Appears in replies. The specific non- mandatory decision objects required in a decision to a particular request depend on the type of client.

   \code
               C-Num = 6
               C-Type = 1, Decision Flags (Mandatory)

               0             1              2             3
       +--------------+--------------+--------------+--------------+
       |        Command-Code         |            Flags            |
       +--------------+--------------+--------------+--------------+

           Commands:
               0 = NULL Decision (No configuration data available)
               1 = Install (Admit request/Install configuration)
               2 = Remove (Remove request/Remove configuration)

           Flags:
               0x01 = Trigger Error (Trigger error message if set)
                Note: Trigger Error is applicable to client-types that
                are capable of sending error notifications for signaled
                messages.
\endcode
       Flag values not applicable to a given context's R-Type or client-type MUST be ignored by the PEP.
\code
              C-Type = 2, Stateless Data
\endcode
       This type of decision object carries additional stateless information that can be applied by the PEP locally. It is a variable length object and its internal format SHOULD be specified in the relevant COPS extension document for the given client-type. This object is optional in Decision messages and is interpreted relative to a given context.

       It is expected that even outsourcing PEPs will be able to make some simple stateless policy decisions locally in their LPDP. As this set is well known and implemented ubiquitously, PDPs are aware of it as well (either universally, through configuration, or using the Client-Open message). The PDP may also include this information in its decision, and the PEP MUST apply it to the resource allocation event that generated the request.
\code
               C-Type = 3, Replacement Data
\endcode
       This type of decision object carries replacement data that is to replace existing data in a signaled message. It is a variable length object and its internal format SHOULD be specified in the relevant COPS extension document for the given client-type. It is optional in Decision messages and is interpreted relative to a given context.
\code
               C-Type = 4, Client Specific Decision Data
\endcode
       Additional decision types can be introduced using the Client Specific Decision Data Object. It is a variable length object and its internal format SHOULD be specified in the relevant COPS extension document for the given client-type. It is optional in Decision messages and is interpreted relative to a given context.
\code
               C-Type = 5, Named Decision Data
\endcode
       Named configuration information is encapsulated in this version of the decision object in response to configuration requests. It is a variable length object and its internal format SHOULD be specified in the relevant COPS extension document for the given client-type. It is optional in Decision messages and is interpreted relative to both a given context and decision flags.
 */
class COPSDecision : public COPSObj
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
		COPSDecision(C_Type type, char* d_data, unsigned int d_size);
		/**
		 * Constructor
		 * @param command of the Decision use Command_Code
		 * @param flgs (default value is 0x01)
		 */
		COPSDecision(Command_Code command, unsigned int flgs=0x01);
		COPSDecision(const COPSDecision &obj);
		COPSDecision &operator=(const COPSDecision &obj);
		/** Constructor
		 * @param t the type of Decision use C_Type Enum
		 * @param cops_obj Client Specific objects go here
		 */
		COPSDecision(C_Type t,const COPSObj &cops_obj);
		~COPSDecision();
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
		 * The Constructor of COPSDecision only enables you to insert 1 COPSObj. With this friend function you can add several more COPSObjs following the example:
		 * \code
		 	COPSDecision dec(Named_Decision_Data,client_si_obj1);
			dec + client_si_obj2 + client_si_obj3;
		\endcode
		 */
		friend COPSDecision &operator+(COPSDecision &dcsi, const COPSObj &obj); 
	
		/**
		 * This function template returns a pointer to a object requested through a enum of the desired object class and it's position (default = 1)
		 * An example of the usage of this method would be the retrieval of a Client SI object contained in a COPSmessage:
		 * \code
		 * COPSDecision decision_obj = messsage.getCOPSDecision(COPSDecision::Client_Specific_Decision_Data); //Retrieve the desired object
		 * COPSClientSpecificObj client_obj(decision_obj.getCOPSObj<ClientSpecific_objs_enum>(ClientSpecificObj,n)); // Retrieves the ClientSpecificObj placed in the n th position
		 * // Please note that ClientSpecific_objs_enum contains ClientSpecificObj value
		 * \endcode
		 *
		 *  @param type Enum of objects of COPSDecisionCSI_Type 
                 *  @param n position or the order number of the desired object inside the COPSObj
		 *  @return the desired COPSObj
                 *  @see COPSObj
                 */
		template <class COPSDecisionCSI_Type> COPSObj getCOPSObj(COPSDecisionCSI_Type type, unsigned int n) const {
	
			COPSObj obj;
			struct COPSobj_data *tmp;
			try {
				tmp = FindObject<COPSDecisionCSI_Type>(type,n);
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
