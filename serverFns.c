#include <stdlib.h>
#include "serverFns.h"
#include "server.h"
#include "thread.h"
#include "socket.h"
#include "servLoop.h"
#include "config.h"

unsigned servSet(Server * srv, unsigned val)
{
    pthread_mutex_lock(&(srv->servMutex))
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
    return srv->controls.vals.power;
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
        servLoop(me);
    }

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
    srv->serverThread = setup.serverThread;
    srv->queueLen     = setup.queueLen;

    return srv;
}

Server * makeServer(unsigned short port)
{
    //start with the empty server
    Server srv;

    servOff(&srv);
    srv.socket       = listenOnPort(port);
    srv.port         = port;
    //srv.errOut       = stderr;
    srv.serverThread = (pthread_t) 0;
    srv.queueLen     = QUEUELEN;
    
    if(!(srv.socket)) return NULL;

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

