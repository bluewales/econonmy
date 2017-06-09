#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "citizens.h"
#include "MysqlPortal.h"
#include "goods.h"
#include "professions.h"
#include "Market.h"

void purge_citizens_table(MysqlPortal * portal) {
	portal->submitQuery((char*)"drop table citizens");
	// create citizens table
	
	char query[1024];
	sprintf(query,(char*)"CREATE TABLE citizens ( \
id int NOT NULL AUTO_INCREMENT, \
city_id int, \
first_name CHAR(50) NOT NULL, \
last_name CHAR(50) NOT NULL, \
birthdate int NOT NULL DEFAULT 20, \
gender CHAR(1) NOT NULL, \
product_id int DEFAULT 0, \
money int DEFAULT %d, \
alive CHAR(5) NOT NULL DEFAULT 'true', \
INDEX(city_id),\
PRIMARY KEY (id))",CITIZEN_MONEY);
	portal->submitQuery(query);
}

void freeNames(char** array) {
	free(array[1]);
	free(array);
}

char** loadNames(char * file) {
	char** result;
	char* buffer;
	
	FILE * fp = fopen (file, "rt");
	if(!fp)
		printf("Error opening file %s\n", file);
	int sz;
	// Find size of file
	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	
	// Read the whole file into a buffer
	buffer = (char *)malloc(sizeof(char) * sz);
	fread (buffer, 1, sz, fp);
	fclose(fp);
	
	int lineNumbers = 0;
	int i, j;
	for(i = 0; i < sz; i++) {
		if(buffer[i] == '\n') lineNumbers++;
	}
	
	result = (char**)malloc(sizeof(char *) * (lineNumbers + 1));
	
	result[0] = (char *)lineNumbers;
	result[1] = buffer;
	j = 2;
	for(i = 0; i < sz; i++) {
		if(buffer[i] == '\r') {
			//printf("%d %d\n", i, j);
			buffer[i] = 0;
			buffer[i+1] = 0;
			if(j <= lineNumbers) 
				result[j++] = &buffer[i+2];
		}
	}
	return result;
}

void generateCitizens(MysqlPortal * portal, int numPeople, int city_id) {
	int i = 0;
	char** boyNames = loadNames((char*)"dat/names/dutch_boy_first_names.txt");
	char** girlNames = loadNames((char*)"dat/names/dutch_girl_first_names.txt");
	char** lastNames = loadNames((char*)"dat/names/dutch_last_names.txt");
	
	int numBoyNames = (int) boyNames[0];
	int numGirlNames = (int) girlNames[0];
	int numLastNames = (int) lastNames[0];
	
	portal->long_query_begin((char *)"INSERT INTO citizens(city_id,first_name,last_name,birthdate,gender)VALUES");
	for (i = 0; i < numPeople; i++) {
	
		int gender = rand() % 2;
		int firstIndex = (gender ? (rand() % numBoyNames) : (rand() % numGirlNames)) + 1;
		int lastIndex = rand() % numLastNames + 1;
		char add[1024];
		sprintf(add, "(%d,'%s','%s',%d,'%s')",
					city_id,
					gender ? boyNames[firstIndex] : girlNames[firstIndex],
					lastNames[lastIndex],
					rand() % 30 + 20, 
					gender ? "M" : "F");
		portal->long_query_update(add);
	}
	portal->long_query_finish();
	
	freeNames(boyNames);
	freeNames(girlNames);
	freeNames(lastNames);
}
