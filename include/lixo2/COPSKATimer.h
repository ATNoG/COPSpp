/* 
 * COPSKATimer.h
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

#ifndef _COPSKATIMER_H_
#define _COPSKATIMER_H_

#include <Copspp/COPSObj.h>

class COPSKATimer : public COPSObj
{
	public:
		COPSKATimer(unsigned short time);
		COPSKATimer(const COPSKATimer &);
		COPSKATimer &operator=(const COPSKATimer &);
		~COPSKATimer();
		unsigned short getKAtime() {return KAtime;};
	
	protected:
		unsigned short KAtime;
	
};


#endif	//_COPSKATIMER_H_
