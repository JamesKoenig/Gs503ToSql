#include "connectionFns.h"
#include "connection.h"

void stopConnection(Connection * con)
{
    con->status.is_alive = 0;
    pthread_join(con->thread);
    return;
}

void delConnection(Connection * con)
{
    stopConnection(Connection * con);
    free(con);
    return;
}
