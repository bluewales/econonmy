#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "City.h"
#include "RecipesIndex.h"
#include "Recipe.h"
#include "MysqlPortal.h"

RecipesIndex::RecipesIndex(City * city) {
	MysqlPortal * portal = city->portal;

	MYSQL_ROW recipe_row;
	MYSQL_RES *recipes_result; 
	unsigned int recipes_num_fields, recipes_num_rows;
	
	
	// load recipes out of the database
	
	
	
	
	
	
	recipes_result = portal->submitQuery((char*)"SELECT \
id,good_id,action,required_materials,required_tools,beneficial_tools,\
function_for_production \
FROM recipes"); 

	recipes_num_fields = mysql_num_fields(recipes_result);
	recipes_num_rows = mysql_num_rows(recipes_result);
	
	this->recipes_by_id = svcreate(recipes_num_rows);
		
	while ((recipe_row = mysql_fetch_row(recipes_result))) {
	
		int id = atoi(recipe_row[0]);
		int good_id = atoi(recipe_row[1]);
		char * action = recipe_row[2];
		char * required_materials = recipe_row[3];
		char * required_tools = recipe_row[4];
		char * beneficial_tools = recipe_row[5];
		char * function_for_production = recipe_row[6];
			
		Recipe * recipe = new Recipe(id,good_id,action,required_materials,required_tools,beneficial_tools,function_for_production);
		svset(this->recipes_by_id, recipe->id, recipe);
	}
	
	mysql_free_result(recipes_result);
}

RecipesIndex::~RecipesIndex() {
	int i;
	for(i = 0; (unsigned int)i <= this->recipes_by_id->size; i++) {
		Recipe * recipe = (Recipe *)svget(this->recipes_by_id, i);
		if(recipe) {
			delete recipe;
		}
	}
	svdestroy(this->recipes_by_id);
}