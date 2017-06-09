# SilverServer makefile

OBJS =  
LIBOBJS = 
CC = g++

bin/eveonthewater : $(OBJS) $(LIBOBJS)
	$(CC) -o bin/silverserver -I inc -I lib/inc -lpthread -lm $(OBJS) $(LIBOBJS)

test-economy : 
	cd Economy; make test
	
test :  test-economy

initDB-economy :
	cd Economy; make initDB
	
initDB : initDB-economy 
	
run :	bin/eveonthewater
	./bin/eveonthewater
	
clean :
	cd Economy; make clean
	rm $(OBJS)
	rm $(LIBOBJS)
	rm bin/eveonthewater
	
#####################################################################
# object files

Economy/obj/test.obj : Economy/src/test.cpp
	$(CC) -c -o Economy/obj/test.obj Economy/src/test.cpp

#####################################################################
# library object files

obj/websocketserver.obj : lib/src/websocketserver.c inc/websocketserver.h lib/inc/sha1.h
	$(CC) -c -o obj/websocketserver.obj -I lib/inc -I inc lib/src/websocketserver.c
