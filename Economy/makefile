# Economy makefile

OBJS = obj/MysqlPortal.o obj/rvpParser.o obj/myYajlParser.o \
obj/goods.o obj/professions.o \
obj/citizens.o obj/affinities.o \
obj/skills.o obj/citizenAI.o \
obj/Good.o obj/GoodsIndex.o obj/fastRandom.o \
obj/smallVector.o obj/hashMap.o obj/Citizen.o \
obj/CitizensIndex.o obj/Market.o \
obj/Profession.o obj/ProfessionsIndex.o obj/cities.o \
obj/City.o obj/possessions.o obj/potentials.o obj/prices.o \
obj/CitiesIndex.o obj/CitizenBid.o obj/CitizenOffer.o \
obj/PlayerBid.o obj/PlayerOffer.o obj/recipes.o obj/Recipe.o obj/RecipesIndex.o \
obj/captains.o obj/inventories.o obj/player_trades.o obj/names.o

LIBS = lib/libyajl.a lib/libmyutil.a

CC = g++ -Wall -g -O3
MYSQLFLAGS = -L/usr/lib/mysql -I/usr/include/mysql 
MYSQLLIBRARY = -lmysqlclient
YAJLFLAGS = -Ilib/inc/yajl
LIBFLAGS = $(MYSQLFLAGS) $(YAJLFLAGS)
INCFLG = -Iinc -Ilib/inc -Iinc/database 

economy : bin/economy bin/initDB

run : bin/economy
	./bin/economy

restart : bin/economy bin/initDB
	./bin/initDB
	rm -f story*.txt
	rm -f /var/www/EveOnTheWater/www/economy/city_*.json
	./bin/economy
	
yajl_test : bin/yajl_test
	./bin/yajl_test < dat/goods.json

test : bin/test
	./bin/test
	
init : bin/initDB
	./bin/initDB
	rm -f story.txt
		
clean : 
	rm -f obj/*.o
	rm -f bin/*
	rm -f lib/obj/*.o
	rm -f lib/*.a
	
	
#####################################################################
# executable files

bin/initDB : obj/initDB.o $(OBJS) $(LIBS)
	$(CC) -o bin/initDB $(INCFLG) obj/initDB.o $(OBJS) $(LIBFLAGS)  $(LIBS) $(MYSQLLIBRARY)
	
bin/economy : obj/economy.o $(OBJS) $(LIBS)
	$(CC) -o bin/economy $(INCFLG) obj/economy.o $(OBJS) $(LIBFLAGS)  $(LIBS) $(MYSQLLIBRARY)

bin/yajl_test : obj/yajl_test.o obj/myYajlParser.o $(LIBS)
	$(CC) -o bin/yajl_test obj/yajl_test.o $(YAJLFLAGS) $(MYSQLLIBRARY)

bin/test : obj/test.o $(OBJS) $(LIBS)
	$(CC) -o bin/test $(INCFLG) obj/test.o $(OBJS) $(LIBFLAGS) $(LIBS) $(MYSQLLIBRARY)
	
#####################################################################
# executable objects files

obj/economy.o : src/economy.cpp $(LIBS)
	$(CC) -c -o obj/economy.o $(INCFLG) src/economy.cpp $(LIBFLAGS)

obj/initDB.o : src/initDB.cpp $(LIBS)
	$(CC) -c -o obj/initDB.o $(INCFLG) src/initDB.cpp $(LIBFLAGS)
	
#####################################################################
# object files

obj/yajl_test.o : src/yajl_test.cpp $(LIBS)
	$(CC) -c -o obj/yajl_test.o $(INCFLG) src/yajl_test.cpp $(YAJLFLAGS)

obj/test.o : src/test.cpp $(LIBS)
	$(CC) -c -o obj/test.o $(INCFLG) src/test.cpp  $(LIBFLAGS)

obj/MysqlPortal.o : src/MysqlPortal.cpp inc/MysqlPortal.h $(LIBS)
	$(CC) -c -o obj/MysqlPortal.o $(INCFLG) src/MysqlPortal.cpp $(LIBFLAGS)

obj/rvpParser.o : src/rvpParser.c inc/rvpParser.h $(LIBS)
	$(CC) -c -o obj/rvpParser.o $(INCFLG) src/rvpParser.c $(LIBFLAGS)

obj/myYajlParser.o : src/myYajlParser.c inc/myYajlParser.h $(LIBS)
	$(CC) -c -o obj/myYajlParser.o $(INCFLG) src/myYajlParser.c $(LIBFLAGS)
	
obj/goods.o : src/database/goods.c inc/database/goods.h $(LIBS)
	$(CC) -c -o obj/goods.o $(INCFLG) src/database/goods.c $(LIBFLAGS)

obj/professions.o : src/database/professions.cpp inc/database/professions.h $(LIBS)
	$(CC) -c -o obj/professions.o $(INCFLG) src/database/professions.cpp $(LIBFLAGS)

obj/citizens.o : src/database/citizens.c inc/database/citizens.h $(LIBS)
	$(CC) -c -o obj/citizens.o $(INCFLG) src/database/citizens.c $(LIBFLAGS)

obj/affinities.o : src/database/affinities.cpp inc/database/affinities.h $(LIBS)
	$(CC) -c -o obj/affinities.o $(INCFLG) src/database/affinities.cpp $(LIBFLAGS)

obj/skills.o : src/database/skills.cpp inc/database/skills.h $(LIBS)
	$(CC) -c -o obj/skills.o $(INCFLG) src/database/skills.cpp $(LIBFLAGS)

obj/citizenAI.o : src/citizenAI.c inc/citizenAI.h $(LIBS)
	$(CC) -c -o obj/citizenAI.o $(INCFLG) src/citizenAI.c $(LIBFLAGS)

obj/Citizen.o : src/Citizen.cpp inc/Citizen.h $(LIBS)
	$(CC) -c -o obj/Citizen.o $(INCFLG) src/Citizen.cpp $(LIBFLAGS)

obj/CitizensIndex.o : src/CitizensIndex.cpp inc/CitizensIndex.h $(LIBS)
	$(CC) -c -o obj/CitizensIndex.o $(INCFLG) src/CitizensIndex.cpp $(LIBFLAGS)

obj/Good.o : src/Good.cpp inc/Good.h $(LIBS)
	$(CC) -c -o obj/Good.o $(INCFLG) src/Good.cpp $(LIBFLAGS)
	
obj/GoodsIndex.o : src/GoodsIndex.cpp inc/GoodsIndex.h $(LIBS)
	$(CC) -c -o obj/GoodsIndex.o $(INCFLG) src/GoodsIndex.cpp $(LIBFLAGS)

obj/fastRandom.o : src/fastRandom.c inc/fastRandom.h $(LIBS)
	$(CC) -c -o obj/fastRandom.o $(INCFLG) src/fastRandom.c $(LIBFLAGS)

obj/smallVector.o : src/smallVector.c inc/smallVector.h $(LIBS)
	$(CC) -c -o obj/smallVector.o $(INCFLG) src/smallVector.c $(LIBFLAGS)

obj/hashMap.o : src/hashMap.c inc/hashMap.h $(LIBS)
	$(CC) -c -o obj/hashMap.o $(INCFLG) src/hashMap.c $(LIBFLAGS)

obj/Market.o :  src/Market.cpp inc/Market.h $(LIBS)
	$(CC) -c -o obj/Market.o $(INCFLG) src/Market.cpp $(LIBFLAGS)

obj/Profession.o :  src/Profession.cpp inc/Profession.h $(LIBS)
	$(CC) -c -o obj/Profession.o $(INCFLG) src/Profession.cpp $(LIBFLAGS)
	
obj/ProfessionsIndex.o :  src/ProfessionsIndex.cpp inc/ProfessionsIndex.h $(LIBS)
	$(CC) -c -o obj/ProfessionsIndex.o $(INCFLG) src/ProfessionsIndex.cpp $(LIBFLAGS)

obj/City.o : src/City.cpp inc/City.h $(LIBS)
	$(CC) -c -o obj/City.o $(INCFLG) src/City.cpp $(LIBFLAGS)
	
obj/cities.o : src/database/cities.c inc/database/cities.h $(LIBS)
	$(CC) -c -o obj/cities.o $(INCFLG) src/database/cities.c $(LIBFLAGS)

obj/possessions.o : src/database/possessions.cpp inc/database/possessions.h $(LIBS)
	$(CC) -c -o obj/possessions.o $(INCFLG) src/database/possessions.cpp $(LIBFLAGS)
	
obj/potentials.o : src/database/potentials.cpp inc/database/potentials.h $(LIBS)
	$(CC) -c -o obj/potentials.o $(INCFLG) src/database/potentials.cpp $(LIBFLAGS)
	
obj/prices.o : src/database/prices.cpp inc/database/prices.h $(LIBS)
	$(CC) -c -o obj/prices.o $(INCFLG) src/database/prices.cpp $(LIBFLAGS)
	
obj/recipes.o : src/database/recipes.cpp inc/database/recipes.h $(LIBS)
	$(CC) -c -o obj/recipes.o $(INCFLG) src/database/recipes.cpp $(LIBFLAGS)
	
obj/CitiesIndex.o : src/CitiesIndex.cpp inc/CitiesIndex.h $(LIBS)
	$(CC) -c -o obj/CitiesIndex.o $(INCFLG) src/CitiesIndex.cpp $(LIBFLAGS)
	
obj/CitizenBid.o : src/CitizenBid.cpp inc/CitizenBid.h inc/Bid.h $(LIBS)
	$(CC) -c -o obj/CitizenBid.o $(INCFLG) src/CitizenBid.cpp $(LIBFLAGS)
	
obj/CitizenOffer.o : src/CitizenOffer.cpp inc/CitizenOffer.h inc/Offer.h $(LIBS)
	$(CC) -c -o obj/CitizenOffer.o $(INCFLG) src/CitizenOffer.cpp $(LIBFLAGS)	

obj/PlayerBid.o : src/PlayerBid.cpp inc/PlayerBid.h inc/Bid.h $(LIBS)
	$(CC) -c -o obj/PlayerBid.o $(INCFLG) src/PlayerBid.cpp $(LIBFLAGS)
	
obj/PlayerOffer.o : src/PlayerOffer.cpp inc/PlayerOffer.h inc/Offer.h $(LIBS)
	$(CC) -c -o obj/PlayerOffer.o $(INCFLG) src/PlayerOffer.cpp $(LIBFLAGS)

obj/Recipe.o : src/Recipe.cpp inc/Recipe.h $(LIBS)
	$(CC) -c -o obj/Recipe.o $(INCFLG) src/Recipe.cpp $(LIBFLAGS)
	
obj/RecipesIndex.o : src/RecipesIndex.cpp inc/RecipesIndex.h $(LIBS)
	$(CC) -c -o obj/RecipesIndex.o $(INCFLG) src/RecipesIndex.cpp $(LIBFLAGS)

obj/captains.o : src/database/captains.cpp inc/database/captains.h $(LIBS)
	$(CC) -c -o obj/captains.o $(INCFLG) src/database/captains.cpp $(LIBFLAGS)
	
obj/inventories.o : src/database/inventories.cpp inc/database/inventories.h $(LIBS)
	$(CC) -c -o obj/inventories.o $(INCFLG) src/database/inventories.cpp $(LIBFLAGS)
	
obj/player_trades.o : src/database/player_trades.cpp inc/database/player_trades.h $(LIBS)
	$(CC) -c -o obj/player_trades.o $(INCFLG) src/database/player_trades.cpp $(LIBFLAGS)
	
obj/names.o : src/database/names.cpp inc/database/names.h $(LIBS)
	$(CC) -c -o obj/names.o $(INCFLG) src/database/names.cpp $(LIBFLAGS)

#####################################################################
# library object files

lib/obj/yajl_alloc.o : lib/src/yajl/yajl_alloc.c lib/inc/yajl/yajl_alloc.h 
	$(CC) -c -o lib/obj/yajl_alloc.o $(INCFLG) lib/src/yajl/yajl_alloc.c $(LIBFLAGS)

lib/obj/yajl_lex.o : lib/src/yajl/yajl_lex.c lib/inc/yajl/yajl_lex.h
	$(CC) -c -o lib/obj/yajl_lex.o $(INCFLG) lib/src/yajl/yajl_lex.c $(LIBFLAGS)
	
lib/obj/yajl_parser.o : lib/src/yajl/yajl_parser.c lib/inc/yajl/yajl_parser.h
	$(CC) -c -o lib/obj/yajl_parser.o $(INCFLG) lib/src/yajl/yajl_parser.c $(LIBFLAGS)
	
lib/obj/yajl_buf.o : lib/src/yajl/yajl_buf.c lib/inc/yajl/yajl_buf.h
	$(CC) -c -o lib/obj/yajl_buf.o $(INCFLG) lib/src/yajl/yajl_buf.c $(LIBFLAGS)

lib/obj/yajl_encode.o : lib/src/yajl/yajl_encode.c lib/inc/yajl/yajl_encode.h
	$(CC) -c -o lib/obj/yajl_encode.o $(INCFLG) lib/src/yajl/yajl_encode.c $(LIBFLAGS)

lib/obj/yajl_tree.o : lib/src/yajl/yajl_tree.c lib/inc/yajl/yajl_tree.h
	$(CC) -c -o lib/obj/yajl_tree.o $(INCFLG) lib/src/yajl/yajl_tree.c $(LIBFLAGS)
	
lib/obj/yajl.o : lib/src/yajl/yajl.c
	$(CC) -c -o lib/obj/yajl.o $(INCFLG) lib/src/yajl/yajl.c $(LIBFLAGS)
	
lib/obj/myutil.o : lib/src/myutil.c lib/inc/myutil.h 
	$(CC) -c -o lib/obj/myutil.o $(INCFLG)  lib/src/myutil.c $(LIBFLAGS)
	
#####################################################################
# libraries

YAJL_OBJS = lib/obj/yajl_tree.o lib/obj/yajl_encode.o lib/obj/yajl_buf.o \
			lib/obj/yajl_parser.o lib/obj/yajl_lex.o lib/obj/yajl_alloc.o lib/obj/yajl.o

lib/libyajl.a : $(YAJL_OBJS)
	ar rcs lib/libyajl.a $(YAJL_OBJS)
	ranlib lib/libyajl.a

lib/libmyutil.a : lib/obj/myutil.o
	ar rcs lib/libmyutil.a lib/obj/myutil.o
	ranlib lib/libmyutil.a

