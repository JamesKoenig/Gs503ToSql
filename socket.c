#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//automatically gets ipv6 or ipv4 information
void * get_in_addr(struct sockaddr *sa)
{
    if(sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr); 
}

/* returns the address info of a connection to localhost on a given port */
struct addrinfo * getLocalAddressInfo(int port)
{
    int addrErr;
    struct addrinfo hint,
                    *localInfo = NULL;

   /* set up the hint, so the computer knows what we want */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    /* populate the linked list */
    if(addrErr = getaddrinfo(NULL, port, &hint, &localInfo))
    {
        fprintf(stdout, "getaddrinfo: %s\n", gai_strerror(addrErr));
        return NULL;
    }
    else
    {
        return localInfo;
    }
}

// returns a socket file descriptor connected to the given port
int listenOnPort(int port)
{
   int sock_fd = 0;

   struct addrinfo *addrInfo, *iter_p;

   addrInfo = getLocalAddressInfo(port);
   if(!addrInfo) return 0;
   
   /* iterate through the linked list and bind to the first socket possble */
   for(iter_p = addrInfo; iter_p != NULL; iter_p = iter_p->next)
   {
       /* try to set up the socket file descriptor */
       if((sock_fd = socket(iter_p->ai_family, iter_p->ai_socktype, iter_p->ai_protocol)) == -1)
       {
           perror("listenOnPort: socket");
           continue;
       }
       /* */
   }

   return sock_fd;
}
