/*
 * Socket.cc 
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

#include "Socket.h"

Socket::Socket() throw (Socket_Exception)
{
	struct protoent *proto = getprotobyname("ip");
	protocol = proto->p_proto;
	domain=INET6;
	type=STREAM;
	sockfd=0;
	
	Guard g(&mutex);
	if((sockfd=socket(domain,type,protocol))==-1) throw Socket_Exception(strerror(errno));
}

Socket::Socket(int socket) throw (Socket_Exception)
{
	struct protoent *proto = getprotobyname("ip");
	protocol = proto->p_proto;
	domain=INET6;
	type=STREAM;
	sockfd=socket;
}

int Socket::Receive(std::string &Buffer, unsigned size)  throw(Socket_Exception)
{
	char buffer[size];
	int recv_size = 0;
	
	Guard g(&mutex);
	if(sockfd!=0)
	if((recv_size=recv(sockfd,buffer,size,0))==-1) throw Socket_Exception(strerror(errno));
	Buffer = std::string(buffer,recv_size);
	
	return recv_size;
}

int Socket::Receive(void *Buffer, unsigned size)  throw(Socket_Exception)
{
	int c_recv = 0;
	Guard g(&mutex);
	if(sockfd!=0)
	if((c_recv=recv(sockfd,Buffer,size,0))==-1) throw Socket_Exception(strerror(errno));
	return c_recv;
}

int Socket::TryReceive(void *Buffer, unsigned size)  throw(Socket_Exception)
{
	int c_recv = 0;
	Guard g(&mutex);
	if(sockfd!=0)
	if((c_recv=recv(sockfd,Buffer,size,MSG_PEEK))==-1) throw Socket_Exception(strerror(errno));
	if(c_recv==0) throw Socket_Exception("There is no data in the receive queue");
	return c_recv;
}

int Socket::Send(std::string Buffer)  throw(Socket_Exception)
{
	int c_sent = 0;
	Guard g(&mutex);
	if(sockfd!=0)
	if((c_sent=send(sockfd,Buffer.c_str(),Buffer.size(),0))==-1) throw Socket_Exception(strerror(errno));
	return c_sent;
}

int Socket::Send(void *Buffer, unsigned size)  throw(Socket_Exception)
{
	int c_sent = 0;
	Guard g(&mutex);
	if(sockfd!=0)
	if((c_sent=send(sockfd,Buffer,size,0))==-1) throw Socket_Exception(strerror(errno));
	return c_sent;
}

bool Socket::Poll(int timeout)
{	
//	Guard g(&destroy);
	struct pollfd ufds;
	ufds.fd = sockfd;
	
	ufds.events = POLLIN | POLLPRI;
	ufds.revents = 0;
	int res = -1 ;

        while (res < 0) {
		res = poll(&ufds,1, timeout);
		
		if( res < 0) {
			if(res == EINTR) 
				continue;
			else 
				throw Socket_Exception(strerror(errno));	
		}
	}
	
	#ifdef DEBUG
	std::cerr << "Socket::Poll(" << timeout << ") = " << res << " : events: " << ufds.events << " , revents: " << ufds.revents << std::endl;
	#endif
	
	if((ufds.revents & POLLERR) == POLLERR) throw Socket_Exception("Error Condition");
	if((ufds.revents & POLLHUP) == POLLHUP) throw Socket_Exception("Hung Up");
	if((ufds.revents & POLLNVAL) == POLLNVAL) {
		sockfd = 0;
		throw Socket_Exception("Invalid request: fd not open");
	}
	
	if(res == 0) return false;	
	return true;
}

Socket::~Socket()
{
	Guard g(&destroy);
	if(sockfd!=0)
		Close();
}

bool Socket::Close() throw(Socket_Exception) {
	Guard g(&mutex);
	if(sockfd==-1) throw Socket_Exception("sockfd=-1");
	if(close(sockfd)==0) {
		sockfd=0;
		return true;
	}
	throw Socket_Exception(strerror(errno));
}
