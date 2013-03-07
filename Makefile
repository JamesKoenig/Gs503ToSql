#massively unfinished
CC = gcc
COMPFLAGS = -c -O2

Gs503Interface: 

connectionFns.o: connectionFns.c

packetSQL.o: packetSQL.c packetSQL.h

serverFns.o: serverFns.c server.h serverFns.h thread.h socket.h

socket.o: socket.c

thread.o: thread.c
	
clean: 
	$(RM) *.o Gs503Interface
