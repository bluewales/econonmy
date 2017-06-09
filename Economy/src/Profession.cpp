#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "professions.h"
#include "Profession.h"

Profession::Profession(MYSQL_ROW row) {
	int i = 0;
	
	this->id = atoi(row[i++]);
	this->name = (char *)malloc(strlen(row[i]) + 1);
	strcpy(this->name, row[i++]);
	
}

Profession::~Profession() {
	free(this->name);
}




