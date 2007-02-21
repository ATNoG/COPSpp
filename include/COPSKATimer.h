/*
 * COPSKATimer.h 
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

#ifndef _COPSKATIMER_H_
#define _COPSKATIMER_H_

#include "COPSObj.h"

/**
2.2.10 Keep-Alive Timer Object (KATimer)

   Times are encoded as 2 octet integer values and are in units of
   seconds.  The timer value is treated as a delta.
\code
           C-Num = 10,

           C-Type = 1, Keep-alive timer value
\endcode
   Timer object used to specify the maximum time interval over which a
   COPS message MUST be sent or received. The range of finite timeouts
   is 1 to 65535 seconds represented as an unsigned two-octet integer.
   The value of zero implies infinity.

\code
               0             1              2             3
      +--------------+--------------+--------------+--------------+
      |     -----------------       |        KA Timer Value       |
      +--------------+--------------+--------------+--------------+
\endcode
 */
class COPSKATimer : public COPSObj
{
	public:
		/**
		 * Constructor
		 * @param time in seconds
		 */
		COPSKATimer(unsigned short time);
		COPSKATimer(const COPSKATimer &);
		COPSKATimer &operator=(const COPSKATimer &);
		~COPSKATimer();
		/**
		 * Time of the Keep Alive 
		 * @return in seconds
		 */
		unsigned short getKAtime() {return KAtime;};
	
	protected:
		unsigned short KAtime;
	
};


#endif	//_COPSKATIMER_H_
