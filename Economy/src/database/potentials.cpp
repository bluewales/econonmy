#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "professions.h"
#include "rvpParser.h"
#include "potentials.h"



void purge_potentials_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table potentials");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE potentials ( \
							id int NOT NULL AUTO_INCREMENT, \
							city_id int, \
							good_id int, \
							potential double DEFAULT 50, \
							UNIQUE INDEX(city_id,good_id), \
							PRIMARY KEY (id))");
}

void generate_potentials(MysqlPortal * portal) {
	MYSQL_ROW city_row, good_row;
	MYSQL_RES *goods_result, *cities_result; 
	unsigned int goods_num_fields, goods_num_rows, cities_num_fields, cities_num_rows;

	cities_result = portal->submitQuery((char*)"SELECT id,plains_score,mountain_score,cultural_biome FROM cities"); 
	cities_num_fields = mysql_num_fields(cities_result);
	cities_num_rows = mysql_num_rows(cities_result);
	

	
	portal->long_query_begin((char *)"INSERT INTO potentials(city_id,good_id,potential)VALUES");
	while ((city_row = mysql_fetch_row(cities_result))) {
		int city_id = atoi(city_row[0]);
		
		goods_result = portal->submitQuery((char*)"SELECT id,function_for_potential FROM goods"); 
		goods_num_fields = mysql_num_fields(goods_result);
		goods_num_rows = mysql_num_rows(goods_result);
		
		while ((good_row = mysql_fetch_row(goods_result))) {
			
			char add[1024];
			int good_id = atoi(good_row[0]);
			char * function_for_potential = good_row[1];
			int plains_score = atoi(city_row[1]);
			int mountain_score = atoi(city_row[2]);
			int cultural_biome = atoi(city_row[3]);
			int P[4];
			P[1] = plains_score;
			P[2] = mountain_score;
			P[3] = cultural_biome;
			
			double potential = (int)rvp_parse(function_for_potential, P);
			
			sprintf(add,"(%d,%d,%f)", city_id, good_id, potential);
			
			portal->long_query_update(add);
		}
		mysql_free_result(goods_result);
	}
	portal->long_query_finish();
	
	mysql_free_result(cities_result);
}
