/*
 * COPSAcctTimer.h
 *
 * Copyright (C) 2004 Universidade Aveiro - Instituto de Telecomunicações Polo Aveiro 
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

#ifndef _COPSACCTTIMER_H_
#define _COPSACCTTIMER_H_

#include "COPSObj.h"

/**
2.2.15 Accounting Timer Object (AcctTimer)

   Times are encoded as 2 octet integer values and are in units of seconds.  The timer value is treated as a delta.
\code
           C-Num = 15,

           C-Type = 1, Accounting timer value
\endcode
   Optional timer value used to determine the minimum interval between periodic accounting type reports. It is used by the PDP to describe to the PEP an acceptable interval between unsolicited accounting updates via Report messages where applicable. It provides a method for the PDP to control the amount of accounting traffic seen by the network. The range of finite time values is 1 to 65535 seconds represented as an unsigned two-octet integer. A value of zero means there SHOULD be no unsolicited accounting updates.
\code
                0             1              2             3
       +--------------+--------------+--------------+--------------+
       |        --------------       |        ACCT Timer Value     |
       +--------------+--------------+--------------+--------------+
\endcode
 */ 
class COPSAcctTimer : public COPSObj
{
	public:
		/**
		 * Constructor
		 * @param time in seconds
		 */
		COPSAcctTimer(unsigned int time);
		COPSAcctTimer(const COPSAcctTimer &);
		COPSAcctTimer &operator=(const COPSAcctTimer &);
		~COPSAcctTimer();
		/**
		 * Time of the Accounting Timer
		 * @return in seconds
		 */
		unsigned short getACCTtime() {return ACCTtime;};

	protected:
		unsigned short ACCTtime;
};

#endif	//_COPSACCTTIMER_H_
