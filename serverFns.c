#include <stdlib.h>
#include "serverFns.h"
#include "server.h"
#include "thread.h"
#include "socket.h"
#include "servMain.h"

unsigned servOn(Server * srv)
{
    return srv->controls.vals.power = 1;
}

unsigned servOff(Server * srv)
{
    return srv->controls.vals.power = 0;
}

unsigned servStatus(Server * srv)
{
    return srv->controls.vals.power;
}

void * serverThread(void * args)
{
    Server * me = (Server *) args;

    while(servStatus(me))
    {
        servLoop(me);
    }

    return NULL;
}

Server * makeServer(unsigned short port)
{
    //start with the empty server
    Server * srv = NULL;

    srv = malloc(sizeof(srv));
    if(!srv) return NULL;
    
    servOff(srv);
    srv->socket       = listenOnPort(port);
    srv->port         = port;
    srv->errOut       = stderr;
    srv->serverThread = (pthread_t) 0;
    
    if(!(srv->socket)) return NULL;

    return srv;
}

int startServer(Server * srv)
{
    //tell the server it's on, this has to be done first because
    //the server thread checks for it
    servOn(srv);
    srv->serverThread       = makeThread(serverThread, srv);

    //if the server thread has not been made
    if(!(srv->serverThread))
    {
        //turn off the server power flag
        servOff(srv);
        return 0;
    }
    //server is now running, return success
    else return 1;
}

int stopServer(Server * srv)
{
    //turn off the server
    servOff(srv);
    //join with the server thread (wait for it to die)
    //discard its output
    pthread_join(srv->serverThread, NULL);
    return 1;
}

void delServer(Server * srv)
{
    //make sure to stop the server thread
    stopServer(srv);
    close(srv->socket);
    free(srv);
    return;
}

