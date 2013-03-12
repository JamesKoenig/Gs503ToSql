#include "server.h"
#include "serverFns.h"
#include "servLoop.h"

int servLoop(Server * srv)
{
    int thread_fd;
    struct sockaddr_storage conn_addr;
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    //accept an incoming connection (since the socket is set to non-blocking)
    //it waits until a connection is formed
    thread_fd = accept(srv->socket, (struct sockaddr *)&conn_addr, &sin_size);
    //if the thread has failed to be spawned
    if(!new_fd == -1)
    {
        //print an error through stderr
        perror("accept");
        //return to the function that calls the loop
        return 0;
    }
    else
    {
        addSocket
    }
    return 1;
}
