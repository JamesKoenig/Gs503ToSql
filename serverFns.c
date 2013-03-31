#include <stdlib.h>
#include <string.h>
#include "serverFns.h"
#include "server.h"
#include "thread.h"
#include "socket.h"
#include "connection.h"
#include "connectionFns.h"
#include "servLoop.h"
#include "config.h"

unsigned servSet(Server * srv, unsigned val)
{
    pthread_mutex_lock(&(srv->servMutex));
    switch(val)
    {
        case 1:
            srv->controls.vals.power = 1;
            break;
        default:
            srv->controls.vals.power = 0;
            break;
    }
    pthread_mutex_unlock(&(srv->servMutex));
    return srv->controls.vals.power;
}

unsigned servOn(Server * srv)
{
    return servSet(srv, 1);
}

unsigned servOff(Server * srv)
{
    return servSet(srv, 0);
}

unsigned servStatus(Server * srv)
{
    unsigned retVal;
    pthread_mutex_lock(&(srv->servMutex));
    retVal = srv->controls.vals.power;
    pthread_mutex_unlock(&srv->servMutex);
    return retVal;
}

void delConnections(Server * srv)
{
    Connection * current;
    if(srv->conList != NULL)
    {
        current = srv->conList;
        srv->conList = current->next;
        //stop the connection thread
        delConnection(current);
        
        delConnections(srv);
        return;
    }
    else return;
}

void * serverThread(void * args)
{
    Server * me = (Server *) args;

    //listen on the given port
    if(listen(me->socket, me->queueLen) == -1)
    {
        perror("listen");
        servOff(me);
        return NULL;
    }

    //all clear, run the main loop function
    while(servStatus(me))
    {
        DEBUGOUT("calling servLoop()");
        servLoop(me);
    }

    delConnections(me);

    return NULL;
}

/** 
 *
 * \brief returns an allocated server object
 * \args the value of a server object for setup
 */
Server * advMakeServ(Server setup)
{
    Server * srv = malloc(sizeof(Server));
    if(!srv) return NULL;
    
    srv->controls    = setup.controls;
    srv->socket      = setup.socket;
    srv->port        = setup.port;
//    srv->errOut      = setup.errOut;
    srv->serverThread = (pthread_t) 0;
    srv->queueLen     = setup.queueLen;
    srv->conList      = setup.conList;

    pthread_mutex_init(&(srv->servMutex), NULL);

    DEBUGOUT("advMakeServ successful\n");
    return srv;
}

Server * makeServer(unsigned short port)
{
    //start with the empty server
    Server srv;

    memset(&srv.controls, 0, sizeof(srv.controls));
    srv.socket       = listenOnPort(port);
    srv.port         = port;
    //srv.errOut     = stderr;
    srv.serverThread = (pthread_t) 0;
    srv.queueLen     = QUEUELEN;
    srv.conList      = NULL;

    if(!(srv.socket)) return NULL;

    DEBUGOUT("makeServer successful\n");
    return advMakeServ(srv);
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
        DEBUGOUT("startserver unsuccesful\n");
        servOff(srv);
        return 0;
    }
    //server is now running, return success
    else
    {
        DEBUGOUT("startserver successful\n");
        return 1;
    }
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

void addConnection(Server * srv, Connection * con)
{
    DEBUGOUT("adding connection\n");
    if(con)
    {
            con->next = srv->conList;
            srv->conList = con;
    }
    return;
}

void reapConnections(Server * srv)
{
    void listCheck(Connection * previous, Connection * current);
    DEBUGOUT("in reapConnections\n");
    if(srv->conList && srv->conList->next)
    {
        listCheck(srv->conList, srv->conList->next);
    }
    else if(srv->conList && !(srv->conList->next) && !(srv->conList->status.is_alive))
    {
        delConnection(srv->conList);
        srv->conList = NULL;
    }
    else DEBUGOUT("NOTHING\n");
    DEBUGOUT("reapConnections returning\n");
    return;
}

void listCheck(Connection * previous, Connection * current)
{
    DEBUGOUT("in listCheck");
    if(current && !(current->status.is_alive))
    {
        previous->next = current->next;
        delConnection(current);
        listCheck(previous, previous->next);
    }
    return;
}


