/* 
 * COPSReportType.h
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

#ifndef _COPSREPORTTYPE_H_
#define _COPSREPORTTYPE_H_

#include <Copspp/COPSObj.h>

class COPSReportType : public COPSObj
{
	public:
		enum Report_Type {
			Success = 1,		// Decision was successful at the PEP
			Failure = 2,		// Decission could not be completed by PEP
			Accounting = 3,
		};
		COPSReportType(Report_Type t);
		COPSReportType(const COPSReportType &);
		COPSReportType &operator=(const COPSReportType &);
		~COPSReportType();
		Report_Type getType() {return type;};
	
	protected:
		Report_Type type;
};


#endif	//_COPSREPORTTYPE_H_
