#include <sys/socket.h>
#include <stdio.h>
#include "server.h"
#include "serverFns.h"
#include "connectionFns.h"
#include "servLoop.h"
#include "config.h"

int servLoop(Server * srv)
{
    int thread_fd;
    struct sockaddr_storage conn_addr;
    DEBUGOUT("in server loop\n");
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    //accept an incoming connection (since the socket is set to non-blocking)
    //it waits until a connection is formed, this, potentially, might delay shutdown
    DEBUGOUT("accepting connection\n");
    thread_fd = accept(srv->socket, (struct sockaddr *)&conn_addr, &sin_size);
    //if the thread has failed to be spawned
    if(!thread_fd == -1)
    {
        //print an error through stderr
        perror("accept");
        //return to the function that calls the loop
        return 0;
    }
    else
    {
        DEBUGOUT("spawning thread with %d file descriptor\n", thread_fd);
        addConnection(srv, makeConnection(thread_fd));
        DEBUGOUT("THREAD SPAWNED\n");
    }

    DEBUGOUT("reaping connections!\n");
    reapConnections(srv);
    DEBUGOUT("connections reaped, returning to loop. server is %s\n", srv->controls.vals.power ? "alive": "dead");
    return 1;
}
