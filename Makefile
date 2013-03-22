#massively unfinished
CC = gcc 
LD = gcc
COMPFLAGS = -c -O3 -pthread `mysql_config --cflags`
LINKFLAGS = `mysql_config --libs` -O3 -s

Gs503Interface: connectionFns.o connectLoop.o main.o packetSQL.o \
	serverFns.o servLoop.o socket.o thread.o
	$(LD) connectionFns.o connectLoop.o main.o packetSQL.o \
	serverFns.o servLoop.o socket.o thread.o -o Gs503Interface $(LINKFLAGS)

connectionFns.o: connectionFns.c connectionFns.h connection.h thread.h \
 connectLoop.h
	$(CC) connectionFns.c -o connectionFns.o $(COMPFLAGS)

connectLoop.o: connectLoop.c connectLoop.h connection.h packetSQL.h \
 config.h
	$(CC) connectLoop.c -o connectLoop.o $(COMPFLAGS)

main.o: main.c config.h packetSQL.h socket.h thread.h server.h \
 serverFns.h connection.h connectionFns.h
	$(CC) main.c -o main.o $(COMPFLAGS)

packetSQL.o: packetSQL.c packetSQL.h config.h
	$(CC) packetSQL.c -o packetSQL.o $(COMPFLAGS)

serverFns.o: serverFns.c serverFns.h server.h connection.h thread.h \
 socket.h connectionFns.h servLoop.h config.h
	$(CC) serverFns.c -o serverFns.o $(COMPFLAGS)

servLoop.o: servLoop.c server.h serverFns.h connection.h servLoop.h
	$(CC) servLoop.c -o servLoop.o $(COMPFLAGS)


socket.o: socket.c
	$(CC) socket.c -o socket.o $(COMPFLAGS)

thread.o: thread.c
	$(CC) thread.c -o thread.o $(COMPFLAGS)

clean: 
	$(RM) *.o Gs503Interface
