#include <stdio.h>          //for perror
#include <sys/socket.h>
#include "connection.h"
#include "connectLoop.h"
#include "packetSQL.h"

void parseBuffer(char * string, int length);

int connectionLoop(Connection * con)
{
    int  bytes;
    char buf[BUFFLEN];

    //recv also blocks, so this will wait
    DEBUGOUT("in connection loop (%d), recieving data\n", con->socket);
    bytes = recv(con->socket, buf, BUFFLEN-1, 0);
    switch(bytes)
    {
        case(-1):
            perror("recv");
        case(0):
            DEBUGOUT("terminating thread\n");
            con->status.is_alive = 0;
            return -1;
        default:
            buf[bytes] = '\0';
            parseBuffer(buf, bytes);
            send(con->socket, buf, bytes, 0);
            return 1;
    }
}

void parseBuffer(char * string, int length)
{
    printf("%d: %s\n", length, string);
    //TODO add the following line at some point
    //mysql_push_packet(string)
}

