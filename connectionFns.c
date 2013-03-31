#include <stdio.h>          //for perror
#include <stdlib.h>
#include "socket.h"
#include "connectionFns.h"
#include "connection.h"
#include "thread.h"
#include "connectLoop.h"
#include "config.h"

void stopConnection(Connection * con)
{
    con->status.is_alive = 0;
    pthread_join(con->thread, NULL);
    return;
}

void delConnection(Connection * con)
{
    stopConnection(con);
    close(con->socket);
    free(con);
    return;
}

void * connectionThread(void * data)
{
    Connection * con = (Connection *) data;
    while(con->status.is_alive)
    {
        connectionLoop(con);
    }
    //TODO delete the connection object here
    DEBUGOUT("connection is %s\n", con->status.is_alive ? "alive": "dead");
    return NULL;
}

Connection * makeConnection(int socket)
{
    Connection * retVal;
    retVal = malloc(sizeof(Connection));

    if(!retVal)
    {
        perror("malloc");
        return NULL;
    }

    retVal->socket = socket;
    pthread_mutex_init(&(retVal->mtx), NULL);
    retVal->status.is_alive = 1;
    retVal->thread = makeThread(connectionThread, retVal);
    return retVal;
}
