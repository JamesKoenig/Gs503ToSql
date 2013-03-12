#include <stdio.h>          //for perror
#include "connectLoop.h"
#include "packetSQL.h"

int connectionLoop(Connection * con)
{
    int  bytes;
    char buf[BUFFLEN];

    //recv also blocks
    if(bytes = recv(con->thread, buf, BUFFLEN-1, 0) == -1)
    {
        perror("recv");
        con->status.is_alive = 0;
        return -1;
    }
    buf[bytes] = '\0';
 
    mysql_push_packet(buf);
}
