#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"

void purge_inventories_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table inventories");
	
	// create captains table
	portal->submitQuery((char*)"CREATE TABLE inventories ( \
id int NOT NULL AUTO_INCREMENT, \
captain_id int, \
good_id int, \
count int, \
PRIMARY KEY (id), \
INDEX (captain_id), \
INDEX (good_id))");

}