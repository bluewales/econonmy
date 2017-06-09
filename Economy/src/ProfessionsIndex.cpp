#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "ProfessionsIndex.h"
#include "Profession.h"

ProfessionsIndex::ProfessionsIndex(MysqlPortal * portal) {
	MYSQL_ROW profession_row;
	MYSQL_RES *professions_result; 
	unsigned int professions_num_fields, professions_num_rows;
	
	// load professions out of the database
	professions_result = portal->submitQuery((char*)"SELECT * FROM professions"); 
	professions_num_fields = mysql_num_fields(professions_result);
	professions_num_rows = mysql_num_rows(professions_result);
	
	this->professions_by_id = svcreate(professions_num_rows);
	this->professions_by_name = hmcreate();
		
	while ((profession_row = mysql_fetch_row(professions_result))) {
		Profession * profession = new Profession(profession_row);
		svset(this->professions_by_id, profession->id, profession);
		hmset(this->professions_by_name, profession->name, profession);
	}
	
	mysql_free_result(professions_result);
}

ProfessionsIndex::~ProfessionsIndex() {
	int i;
	for(i = 0; (unsigned int)i <= this->professions_by_id->size; i++) {
		Profession * profession = (Profession *)svget(this->professions_by_id, i);
		if(profession) {
			delete profession;
		}
	}
	svdestroy(this->professions_by_id);
	hmdestroy(this->professions_by_name);
}


void ProfessionsIndex::from_professions_list_to_vector(char * list, smallVector * vector) {
	int j;
	int length = strlen(list);
	for(j = 0; j < length; j++) {
		char profession_name[50];  	
		int start = j;
		for(; j < length && list[j] != ','; j++) {
			profession_name[j-start] = list[j];
			
		}
		profession_name[j-start] = 0;
		Profession * profession = (Profession *)hmget(this->professions_by_name, profession_name);
		//printf("'%s' requires '%s'\n",product->name, material);
		if(profession) {
			int profession_id = profession->id;
			int previous_amount;
			//printf("%s requires %d %s\n",product->name, material_id, material);
			previous_amount = (int)svget(vector, profession_id);
			svset(vector, profession_id, (void *)(previous_amount + 1));
		} else
			printf("from_professions_list_to_vector : '%s' wasn't in the index\n", profession_name);
	}
}
