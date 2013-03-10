#ifndef _CONNECTION_HEADER_
#define _CONNECTION_HEADER_
#include <pthreads.h>
#include <stdio.h>

typedef struct Connection_s
{
    int socket;     //file descriptor 
} Connection;


#endif /* _CONNECTION_HEADER_ */
