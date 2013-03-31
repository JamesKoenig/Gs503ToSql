#ifndef _PACKET_SQL_H_
#define _PACKET_SQL_H_
/**
 * packetSQL.h
 */

typedef unsigned long ulong;
typedef unsigned int uint;

#include <sys/types.h>
//#include <my_global.h> //moved into the file
//#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

void mysql_push_packet(char *packet);

#endif /* packet sql header */
