#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "citizens.h"
#include "MysqlPortal.h"
#include "goods.h"
#include "professions.h"

void purge_cities_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table cities");
	portal->submitQuery((char*)"drop function dead_goods");
	// create citizens table
	portal->submitQuery((char*)"CREATE TABLE cities ( \
id int NOT NULL AUTO_INCREMENT, \
name CHAR(50), \
mountain_score int default 50, \
plains_score int default 50, \
cultural_biome int default 0, \
day int DEFAULT 1, \
PRIMARY KEY (id))");
	char * query = (char *)"\
create function dead_goods(num int,p int)\n\
	returns int\n\
begin\n\
	if p = 0 then\n\
		return 0;\n\
	end if;\n\
	if num > p then\n\
		return (num/p);\n\
	end if;\n\
	if num = 0 then\n\
		return 0;\n\
	end if;\n\
	if (RAND() * p < num) then\n\
		return 1;\n\
	end if;\n\
	return 0;\n\
end;\n\
";
	portal->submitQuery((char*)query);
}


void createCity(MysqlPortal * portal, char * name, int plains, int mountain, int biome, int population) {
	char query[1024];
	sprintf(query,"INSERT INTO cities(name,plains_score,mountain_score,cultural_biome,day)VALUES('%s',%d,%d,%d,1)",
		name, plains, mountain, biome);
	portal->submitQuery(query);
	
	MYSQL_ROW city_row;
	MYSQL_RES *cities_result; 
	unsigned int  cities_num_fields, cities_num_rows;

	sprintf(query,"SELECT id FROM cities where name='%s'",name);
	
	cities_result = portal->submitQuery(query); 
	cities_num_fields = mysql_num_fields(cities_result);
	cities_num_rows = mysql_num_rows(cities_result);
	
	city_row = mysql_fetch_row(cities_result);
	int city_id = atoi(city_row[0]);
	
	generateCitizens(portal, population, city_id);
	
	mysql_free_result(cities_result);
}
