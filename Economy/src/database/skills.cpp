#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "professions.h"
#include "rvpParser.h"
#include "skills.h"



void purge_skills_table(MysqlPortal * portal)
{
	portal->submitQuery((char*)"drop table skills");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE skills ( \
							id int NOT NULL AUTO_INCREMENT, \
							citizen_id int, \
							profession_id int, \
							skill double DEFAULT 0, \
							UNIQUE INDEX(citizen_id,profession_id), \
							PRIMARY KEY (id))");
}

void generate_skills(MysqlPortal * portal)
{
	MYSQL_ROW citizen_row, profession_row;
	MYSQL_RES *professions_result, *citizens_result; 
	unsigned int professions_num_fields, professions_num_rows, citizens_num_fields, citizens_num_rows;

	citizens_result = portal->submitQuery((char*)"SELECT id FROM citizens"); 
	citizens_num_fields = mysql_num_fields(citizens_result);
	citizens_num_rows = mysql_num_rows(citizens_result);
	

	
	portal->long_query_begin((char *)"INSERT INTO skills(citizen_id,profession_id,skill)VALUES");
	while ((citizen_row = mysql_fetch_row(citizens_result))) {
		int citizen_id = atoi(citizen_row[0]);
		
		professions_result = portal->submitQuery((char*)"SELECT id FROM professions"); 
		professions_num_fields = mysql_num_fields(professions_result);
		professions_num_rows = mysql_num_rows(professions_result);
		
		while ((profession_row = mysql_fetch_row(professions_result))) {
			char add[1024];
			int profession_id = atoi(profession_row[0]);
			
			sprintf(add,"(%d,%d,%d)", citizen_id, profession_id, rand() % 101);
			
			portal->long_query_update(add);
		}
		mysql_free_result(professions_result);
	}
	portal->long_query_finish();
	
	mysql_free_result(citizens_result);
}
