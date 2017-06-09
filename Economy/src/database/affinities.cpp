#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "goods.h"
#include "professions.h"
#include "rvpParser.h"
#include "affinities.h"

void purge_affinities_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table affinities");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE affinities ( \
							id int NOT NULL AUTO_INCREMENT, \
							citizen_id int NOT NUll, \
							good_id int NOT NUll, \
							affinity int DEFAULT 0, \
							UNIQUE INDEX(citizen_id,good_id), \
							PRIMARY KEY (id))");
}

void generate_affinities(MysqlPortal * portal) {
	unsigned int i;
	MYSQL_ROW good_row, citizen_row;
	MYSQL_RES *goods_result, *citizens_result; 
	unsigned int goods_num_fields, goods_num_rows, citizens_num_fields, citizens_num_rows;

	citizens_result = portal->submitQuery((char*)"SELECT id FROM citizens"); 
	citizens_num_fields = mysql_num_fields(citizens_result);
	citizens_num_rows = mysql_num_rows(citizens_result);
	
	goods_result = portal->submitQuery((char*)"SELECT id, function_for_affinity FROM goods"); 
	goods_num_fields = mysql_num_fields(goods_result);
	goods_num_rows = mysql_num_rows(goods_result);
	
	char ** formulas = (char **)malloc((1+goods_num_rows) * sizeof(char *));
	memset(formulas, 0, (1+goods_num_rows) * sizeof(char *));
	
	while ((good_row = mysql_fetch_row(goods_result))) {
		unsigned int good_id = atoi(good_row[0]);
		if(good_id > goods_num_rows) {
			printf("ERROR!!\n");
			continue;
		}
		
		formulas[good_id] = (char * ) malloc(strlen(good_row[1]) + 1);
		
		strcpy(formulas[good_id], good_row[1]);
	}
	
	portal->long_query_begin((char *)"INSERT INTO affinities(citizen_id,good_id,affinity)VALUES");
	
	while ((citizen_row = mysql_fetch_row(citizens_result))) {
		printf("%s\r", citizen_row[0]);
		for(i = 1; i <= goods_num_rows; i++) {
			if(!formulas[i])
				continue;
			
			char add[1024];
			int citizen_id = atoi(citizen_row[0]);
			int good_id = i;
			int affinity = (int)rvp_parse(formulas[i], 0);
			
			sprintf(add,"(%d,%d,%d)", citizen_id, good_id, affinity);
			portal->long_query_update(add);
		}
	}
	portal->long_query_finish();
	
	for(i = 1; i <= goods_num_rows; i++) {
		if(formulas[i]) {
			free(formulas[i]);
		}
	}
	free(formulas);
	mysql_free_result(citizens_result);
	mysql_free_result(goods_result);
}
