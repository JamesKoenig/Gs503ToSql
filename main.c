#include <stdio.h>
#include <stdlib.h>
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
    DEBUGOUT("creating server object\n");
    srv = makeServer(SERVPORT);
    if(!srv)
    {
        fprintf(stderr, "could not create server object\n");
        return 1;
    }
    //start the server
    DEBUGOUT("starting server thread\n");
    startServer(srv);
    DEBUGOUT("waiting for hell to freeze over\n");
    pthread_join(srv->serverThread, NULL);
    delServer(srv);
    free(srv);
    return 0;
}
