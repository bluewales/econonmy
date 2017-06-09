#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "citizens.h"

void purge_names_tables(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table dutch_boy_first_names");
	portal->submitQuery((char*)"drop table dutch_girl_first_names");
	portal->submitQuery((char*)"drop table dutch_last_names");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE dutch_boy_first_names ( \
						id int NOT NULL AUTO_INCREMENT, \
						name varchar(50), \
						PRIMARY KEY (id))");
	portal->submitQuery((char*)"CREATE TABLE dutch_girl_first_names ( \
						id int NOT NULL AUTO_INCREMENT, \
						name varchar(50), \
						PRIMARY KEY (id))");
	portal->submitQuery((char*)"CREATE TABLE dutch_last_names ( \
						id int NOT NULL AUTO_INCREMENT, \
						name varchar(50), \
						PRIMARY KEY (id))");
						
	char** boyNames = loadNames((char*)"dat/names/dutch_boy_first_names.txt");
	char** girlNames = loadNames((char*)"dat/names/dutch_girl_first_names.txt");
	char** lastNames = loadNames((char*)"dat/names/dutch_last_names.txt");
	
	unsigned int numBoyNames = (int) boyNames[0];
	unsigned int numGirlNames = (int) girlNames[0];
	unsigned int numLastNames = (int) lastNames[0];
	
	unsigned int i;
		
	portal->long_query_begin((char *)"INSERT INTO dutch_boy_first_names(name)VALUES");
	for (i = 1; i <= numBoyNames; i++) {
	
		char add[1024];
		sprintf(add, "('%s')", boyNames[i]);
		portal->long_query_update(add);
	}
	portal->long_query_finish();
	
	portal->long_query_begin((char *)"INSERT INTO dutch_girl_first_names(name)VALUES");
	for (i = 1; i <= numGirlNames; i++) {
	
		char add[1024];
		sprintf(add, "('%s')", girlNames[i]);
		portal->long_query_update(add);
	}
	portal->long_query_finish();
	
	portal->long_query_begin((char *)"INSERT INTO dutch_last_names(name)VALUES");
	for (i = 1; i <= numLastNames; i++) {
	
		char add[1024];
		sprintf(add, "('%s')", lastNames[i]);
		portal->long_query_update(add);
	}
	portal->long_query_finish();
	
	freeNames(boyNames);
	freeNames(girlNames);
	freeNames(lastNames);
}


