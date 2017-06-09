#ifndef citizens_h_
#define citizens_h_

#include "MysqlPortal.h"
#include "Good.h"


#define CITIZEN_MONEY 1000

void generateCitizens(MysqlPortal * portal, int numPeople, int city_id);
void purge_citizens_table(MysqlPortal * portal); 
void freeNames(char** array);
char** loadNames(char * file);

#endif
