/* 
 * COPSDecision.h
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

#ifndef _COPSDECISION_H_
#define _COPSDECISION_H_

#include <Copspp/COPSObj.h>

struct COPSDecision_Flags {
	unsigned int Command_Code:16;
	unsigned int Flags:16;
};

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
			NULL_Decision = 0,	// No configuration data available
			Install = 1, 		// Admit request/Install configuration
			Remove = 2, 		// Remove request / Remove configuration
		};
		COPSDecision(C_Type type, char* d_data, unsigned int d_size);
		COPSDecision(Command_Code command, unsigned int flgs=0x01);
		COPSDecision(const COPSDecision &obj);
		COPSDecision &operator=(const COPSDecision &obj);
		COPSDecision(C_Type t,const COPSObj &cops_obj);
		~COPSDecision();
		COPSObj getCOPSObj() const;
		Command_Code getCommand() { return command; };
		C_Type getType() { return type; };
		unsigned int getFlags() { return Flags; };
		unsigned int getSize() { return gSize(); };
		char *getData() { return gData(); };

		friend COPSDecision &operator+(COPSDecision &dcsi, const COPSObj &obj);
		template <class COPSDecisionCSI_Type> COPSObj getCOPSObj(COPSDecisionCSI_Type type, unsigned int n) const {
	
			COPSObj obj;
			struct COPSobj_data *tmp;
			try {
				tmp = FindObject<COPSDecisionCSI_Type>(type,n);
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
		Command_Code command;
		unsigned short Flags;
};


#endif	//_COPSDECISION_H_
