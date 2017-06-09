#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"

void purge_captains_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table captains");
	
	// create captains table
	portal->submitQuery((char*)"CREATE TABLE captains ( \
id int NOT NULL AUTO_INCREMENT, \
user_id int, \
first_name CHAR(50), \
last_name CHAR(50), \
ship_id int, \
city_id int, \
money int, \
PRIMARY KEY (id))");
}