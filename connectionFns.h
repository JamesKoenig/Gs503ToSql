#ifndef _CONNECTION_FUNCTIONS_HEADER_
#define _CONNECTION_FUNCTIONS_HEADER_

#include "connection.h"

Connection * makeConnection(int socket);

void stopConnection(Connection * con);

void delConnection(Connection * con);

#endif /* _CONNECTION_FUNCTIONS_HEADER_ */
