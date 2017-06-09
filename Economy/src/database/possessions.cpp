#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "possessions.h"



void purge_possessions_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table possessions");
	// create possessions table
	portal->submitQuery((char*)"CREATE TABLE possessions ( \
							id int NOT NULL AUTO_INCREMENT, \
							citizen_id int NOT NULL, \
							good_id int NOT NULL, \
							possessed int NOT NULL default 0, \
							UNIQUE INDEX(citizen_id,good_id), \
							PRIMARY KEY (id))");
}

void generatePossessions(MysqlPortal * portal) {

	char * query = (char *)"insert into possessions (citizen_id,good_id) select citizens.id,goods.id from goods inner join citizens";
	portal->submitQuery(query);
} 
