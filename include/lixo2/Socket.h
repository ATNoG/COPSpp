/* 
 * Socket.h
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

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sys/poll.h>

#include <PThreadsmm/Mutex.h>
#include <PThreadsmm/Guard.h>

#include <iostream>

class Socket_Exception
{
	public:
		Socket_Exception(const char *str) {error_msg = str;}
		~Socket_Exception() {};
		const std::string what() {return error_msg; };
	private:
		std::string error_msg;
};

class Socket
{
	public:
	
		enum Domain
		{
			UNIX = PF_UNIX,
			LOCAL = PF_LOCAL,  // Local communication
			INET = PF_INET,		// IPv4 Internet protocols   
			INET6 = PF_INET6,    // IPv6 Internet protocols
			NETLINK = PF_NETLINK, // Kernel user interface device     
			PACKET = PF_PACKET,   // Low level packet interface       
			UNSPEC = PF_UNSPEC	// not specified
		};
		
		enum Type
		{
			STREAM = SOCK_STREAM,
			DGRAM = SOCK_DGRAM,
			SEQPACKET = SOCK_SEQPACKET,
			RAW = SOCK_RAW,
			RDM = SOCK_RDM,
			S_PACKET = SOCK_PACKET
		};

		Socket() throw (Socket_Exception);
		Socket(int socket) throw (Socket_Exception);
		 ~Socket();
		int Receive(std::string &Buffer, unsigned size) throw(Socket_Exception);
		int Receive(void *Buffer, unsigned size) throw(Socket_Exception);
		int TryReceive(void *Buffer, unsigned size) throw(Socket_Exception);
		int Send(std::string Buffer) throw(Socket_Exception);
		int Send(void *Buffer, unsigned size) throw(Socket_Exception);
		bool Poll(int timeout);		//in miliseconds
		bool Close() throw(Socket_Exception);
			
	protected:

		Domain domain;
		Type type;
		int protocol;
		Mutex mutex,destroy;
		int sockfd;

};


#endif	//_SOCKET_H_
