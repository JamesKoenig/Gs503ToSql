/*
 * packetSQL -- A library to turn packets on
 *  cell phones into useable data, and then inserts
 *  that data on a MySQL SQL database
 *
 * Right now, we're just doing raw packet data, because as
 * ol' dirty bastard once said, "oooh baby I like it raawwww"
 */

#include "packetSQL.h"
#include <my_global.h>
#include <mysql.h>

/*
 * Packet Parser
 * input: a 30 byte packet -- that's not a hard limit,
 * but it's what I tested it with.
 */
void mysql_push_packet(char* packet){
    
    int qlen;
    
    /* establish connection */
    MYSQL *conn;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, SQLSERV, SQLUSER, SQLPASS, SQLDATB, SQLPORT, NULL, 0)){
        printf("Error: could not connect to the database\n");
        mysql_close(conn);
        return;
    }
    
    /* Create query */
    char query[BUFFLEN];
    char sanitized[BUFFLEN];
    
    /* Sanitize input */
    mysql_real_escape_string(conn, sanitized, packet, strlen(packet));

    /*Build query, get query length*/
    qlen = sprintf(query, "INSERT INTO packets(contents) VALUES('%s');", sanitized);    
    printf("Query: %s\n", query);
    
    /* Execute query */
    if(mysql_real_query(conn, query, qlen))
        printf("Error: query could not be completed.\n");
    else
        printf("Data insert successful!\nm");
    
    /* close connection */
    mysql_close(conn);
}

/* FOR TESTING ONLY */
/*int main(){
    printf("\n\n");
    char packet[30] = "123456789012345678901234567890";
    mysql_push_packet(packet);
    printf("Done!");
    printf("\n\n");
    return 0;
}*/
