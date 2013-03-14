#include <stdio.h>
#include <pthread.h>

#include "config.h"
#include "packetSQL.h"
#include "socket.h"
#include "thread.h"
#include "server.h"
#include "serverFns.h"
#include "connection.h"
#include "connectionFns.h"

int main()
{
    Server * srv = NULL;
    //create the server object.
    srv = makeServer(SERVPORT);
    
    //start the server
    startServer(srv);
    pthread_join(srv->serverThread, NULL);
    delServer(srv);
    free(srv);
    return 0;
}
