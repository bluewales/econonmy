#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "professions.h"
#include "rvpParser.h"
#include "potentials.h"



void purge_prices_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table prices");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE prices ( \
							id int NOT NULL AUTO_INCREMENT, \
							city_id int, \
							good_id int, \
							price int DEFAULT 10, \
							momentum int DEFAULT 0, \
							UNIQUE INDEX(city_id,good_id), \
							PRIMARY KEY (id))");
}

void generate_prices(MysqlPortal * portal) {

	char * query = (char *)"insert into prices (city_id,good_id) select cities.id,goods.id from goods inner join cities";
	portal->submitQuery(query);
	
	query = (char *)"update prices inner join goods on prices.good_id = goods.id\
	set prices.price=10 where goods.name='copper coin'";
	portal->submitQuery(query);
	
	query = (char *)"update prices inner join goods on prices.good_id = goods.id\
	set prices.price=100 where goods.name='silver coin'";
	portal->submitQuery(query);
	
	query = (char *)"update prices inner join goods on prices.good_id = goods.id\
	set prices.price=1000 where goods.name='gold coin'";
	portal->submitQuery(query);
	
}
