#include "server.h"
#include "serverFns.h"
#include "thread.h"
#include "socket.h"

unsigned servOn(Server * srv)
{
    return srv->ctlVals.vals.power = 1;
}

unsigned servOff(Server * srv)
{
    return srv->ctlVals.vals.power = 0;
}

unsigned servStatus(Server * srv)
{
    return srv->ctlVals.vals.power;
}

void * serverThread(void * args)
{
    Server * me = (Server *) args;

    while (servStatus(me))
    {
        //Do nothing
    }
    //for now this doesn't return anything
    return NULL;
}

Server * makeServer(int port)
{
    //start with the empty server
    Server * srv = NULL;

    srv = malloc(sizeof(srv));
    if(!srv) return NULL;
    
    servOff(srv);
    srv->port   = port;
    srv->socket = listenOnPort(port);
    srv->errOut = stderr;
    
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
    pthread_join(srv->serverThread);
    return 1;
}
