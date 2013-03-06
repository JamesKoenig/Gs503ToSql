#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

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
        /* error returned by function call, print it */
        fprintf(stdout, "getaddrinfo: %s\n", gai_strerror(addrErr));
        return NULL;
    }
    else
    {
        /* call successful */
        return localInfo;
    }
}

//Function to create and bind a listener socket based on 
//a passed address value 
//returns a socket file descriptor on success, 0 on failure.
int bindToAddress(struct addrinfo * addr)
{
    int sfd = 0;
    int optVal = 1;
    if(addr = NULL) return 0; //code equivalent of foreshadowing
    /* The if call from hell.  Feel free to make fun of me for this, or change it yourself */
    else if(
            // open the socket file descriptor
            (sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) != -1 &&
            // set the socket to keep alive 
            setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(int))       != -1 &&
            setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(int))       != -1 &&
            bind(sfd, addr->ai_addr, addr->ai_addrlen)                            != -1
           )
    {
        //if that massive if-statement went through, that means we're home free and the socket
        //is bound properly. return the socket descriptor
        return sfd;
    }
    else 
    {
        //there's like 5 ways this function could've gone wrong, let's at least print one
        perror("BindToAddress");
        //close the socket file descriptor
        close(sfd);
        //call this function on the next member of the list THAT'S RIGHT, RECURSION!
        return bindToAddress(addr->next);
    }
}

// returns a socket file descriptor connected to the given port
int listenOnPort(int port)
{
   int sock_fd = 0;

   struct addrinfo *addrInfo, *iter_p;

   addrInfo = getLocalAddressInfo(port);
   if(!addrInfo) return 0;
   
   sock_fd = bindToAddress(addrInfo);

   return sock_fd;
}
