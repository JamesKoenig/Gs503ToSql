#ifndef _SOCKET_HEADER_
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//returns the proper (ipv6 or 4) address
//from a given socket address
void *  getInAddr(struct sockaddr * saddr);

//returns a socket on success, 0 on failure
//just remember to close() it
int     listenOnPort(int port);

#endif /* _SOCKET_HEADER_ */
