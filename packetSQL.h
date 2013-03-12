#ifndef _PACKET_SQL_H_
#define _PACKET_SQL_H_
/**
 * packetSQL.h
 */
#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

void mysql_push_packet(char *packet);

#endif /* packet sql header */
