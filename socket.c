#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


/* returns the address info of a connection to localhost on a given port */
struct addrinfo * getLocalAddressInfo(unsigned short port)
{
    int addrErr;
    char portString[6];
    struct addrinfo hint,
                    *localInfo = NULL;

   /* set up the hint, so the computer knows what we want */
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family   = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags    = AI_PASSIVE;

    /* getaddrinfo requires a character string instead of a port number*/
    sprintf(portString,"%d", port);
    
    /* populate the linked list */
    addrErr = getaddrinfo(NULL, portString, &hint, &localInfo);
    if(addrErr)
    {
        /* error returned by function call, print it */
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addrErr));
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
    if(!addr) return 0; //code equivalent of foreshadowing
    /* The if call from hell.  Feel free to make fun of me for this, or change it yourself */
    else if(
            // open the socket file descriptor
            (sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) != -1 &&
            // set the socket to keep alive 
            setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(int))       != -1 &&
            setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, sizeof(int))       != -1 &&
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
            return bindToAddress(addr->ai_next);
        }
}


// returns a socket file descriptor connected to the given port
int listenOnPort(unsigned short port)
{
   int sock_fd;

   struct addrinfo *addrInfo;

   //get the address info to open a socket at the given port
   addrInfo = getLocalAddressInfo(port);

   //failed to get address information
   if(!addrInfo) return 0;
   
   //get the socket file descriptor from the given address
   sock_fd = bindToAddress(addrInfo);

   freeaddrinfo(addrInfo);
   return sock_fd;
}
