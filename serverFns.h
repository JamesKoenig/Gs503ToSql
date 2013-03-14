#ifndef _SERVER_FUNCTIONS_HEADER_
#define _SERVER_FUNCTIONS_HEADER_
#include "server.h"
#include "connection.h"

/* creates a new server object listening on the given port */
Server * makeServer(unsigned short port);

Server * advMakeServ(Server hint);

int startServer(Server * srv);

int stopServer(Server * srv);

void delServer(Server * srv);

void addConnection(Server * srv, Connection * con);

void reapConnections(Server * srv);

#endif /* _SERVER_FUNCTIONS_HEADER_ */
