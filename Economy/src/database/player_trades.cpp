#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"

void purge_player_trades_tables(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table player_bids");
	portal->submitQuery((char*)"drop table player_offers");
	
	// create player_bids table
	portal->submitQuery((char*)"CREATE TABLE player_bids ( \
id int NOT NULL AUTO_INCREMENT, \
user_id int, \
good_id int, \
city_id CHAR(50), \
to_buy int, \
stock int, \
max_price int, \
balance int, \
created timestamp default current_timestamp, \
PRIMARY KEY (id))");

	// create player_offers table
	portal->submitQuery((char*)"CREATE TABLE player_offers ( \
id int NOT NULL AUTO_INCREMENT, \
user_id int, \
good_id int, \
city_id CHAR(50), \
to_sell int, \
stock int, \
min_price int, \
balance int, \
created timestamp default current_timestamp, \
PRIMARY KEY (id))");
}


