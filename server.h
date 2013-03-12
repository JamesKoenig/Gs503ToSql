#ifndef _SERVER_HEADER_
#define _SERVER_HEADER_

#include <pthread.h>
#include <stdio.h>

/* although this would be a perfect occasion for anonymous unions, I'm rather partial *
 * to avoiding them, I've had bad experiences with compiler implementations of        *
 * anonymous unions and structures in the past                                        * 
 */
/* union to contain all the server control flags */
typedef union ctlVals_u
{
    unsigned char ctl;          //contains all the flags, can be manually used with |, &
    struct                      //contains the flags that are more abstractly indexable
    {
        unsigned power  : 1;    //whether or not the server should be running
        unsigned others : 7;    //other bits that have not yet been implemented
    } vals;
} ctlVals;


/* Encapsulates all server information, so that it can be easily accessible           *
 */
typedef struct Server_s
{
    unsigned short port;        //port the server is listening on
    int socket;                 //file descriptor to the socket
    int connections;            //number of sockets
    int queueLen;               //max queue length
//    FILE * errOut;              //where the server is to give error output
    ctlVals controls;           //server control flags
    pthread_t serverThread;     //the thread the server is on 
    pthread_mutex_t servMutex;  //prevent race conditions within this struct
    Connection * conList;       //array of connections of size (queueLen * Connection)
    //TODO
} Server;

#endif /* _SERVER_HEADER_ */
