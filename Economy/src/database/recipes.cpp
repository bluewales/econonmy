#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recipes.h"



void purge_recipes_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table recipes");
	// create recipes table
	portal->submitQuery((char*)"CREATE TABLE recipes ( \
						id int NOT NULL AUTO_INCREMENT, \
						good_id int NOT NULL, \
						action varchar(50) NOT NULL,\
						required_materials varchar(50) NOT NULL,\
						required_tools varchar(50) NOT NULL,\
						beneficial_tools varchar(50) NOT NULL,\
						function_for_production varchar(100) NOT NULL,\
						PRIMARY KEY (id))");
}