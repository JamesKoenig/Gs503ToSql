#ifndef _CONNECTION_HEADER_
#define _CONNECTION_HEADER_
#include <pthread.h>
#include <stdio.h>

typedef struct Connection_s
{
    int socket;             //file descriptor 
    pthread_t thread;       //conains the connection's thread
    pthread_mutex_t mtx;    //mutexes operations on the connection struct

    struct
    {
        unsigned is_alive : 1;  //whether or not the thread is alive, or is to stay alive
    } status;

    struct Connection_s * next; //yes, this is going to be a linked list

} Connection;


#endif /* _CONNECTION_HEADER_ */
