
#include "stdlib.h"

#include "City.h"
#include "CitiesIndex.h"
#include "Citizen.h"
#include "MysqlPortal.h"
#include "myutil.h"


CitiesIndex::~CitiesIndex() {
	int i;
	for(i = 0; (unsigned int)i <= this->cities_by_id->size; i++) {
		City * city = (City *)svget(this->cities_by_id, i);
		if(city) {
			delete city;
		}
	}
	svdestroy(this->cities_by_id);
}

CitiesIndex::CitiesIndex(MysqlPortal * portal) {
	char query[1024];
	
	MYSQL_ROW city_row;
	MYSQL_RES *cities_result; 
	unsigned int cities_num_fields, cities_num_rows;
	
	//struct timeval timer;
	//start_timer(&timer);
	sprintf(query,"SELECT id FROM cities");
	cities_result = portal->submitQuery(query);
	cities_num_fields = mysql_num_fields(cities_result);
	cities_num_rows = mysql_num_rows(cities_result);

	this->cities_by_id = svcreate(cities_num_rows);
		
	while ((city_row = mysql_fetch_row(cities_result))) {
		City * city = new City(portal, atoi(city_row[0]));
		svset(this->cities_by_id, city->id, city);
	}
	mysql_free_result(cities_result);
}