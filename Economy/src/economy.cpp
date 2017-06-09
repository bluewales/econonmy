#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "MysqlPortal.h"
#include "citizenAI.h"
#include "fastRandom.h"
#include "Market.h"
#include "CitiesIndex.h"
#include "City.h"
#include "myutil.h"


int verify_state(MysqlPortal * portal) {
	
	char * query = (char*)"select count(*) from citizens";
	MYSQL_RES *result = portal->submitQuery(query);
	MYSQL_ROW row = mysql_fetch_row(result);
	
	
	mysql_free_result(result);
	query = (char*)"select money from citizens";
	result = portal->submitQuery(query);
	
	
	int money = 0;
	while((row = mysql_fetch_row(result))) {
		money += atoi(row[0]);
	}
	
	if(money < 0) {
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("Citizen with negative money\n");
		exit(0);
	}
	
	
	/*
	int citizen_count = atoi(row[0]);
	int money_per_citizen = CITIZEN_MONEY;
	
	if(money != citizen_count * money_per_citizen) {
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("There should be %d money, but there is %d money\n", citizen_count * money_per_citizen, money);
		
		exit(0);
	}
	*/
	mysql_free_result(result);
	return 0;
}

int main() {
	printf("We can run the economy, Captain!\n");
	
	struct timeval timer;

	gettimeofday(&timer, NULL);
	sfastRandom(timer.tv_sec+timer.tv_usec);
	
	MysqlPortal * portal = new MysqlPortal();
	
	int i = 0;
	struct timeval start, fine_timer;
	start_timer(&start);
	for(i = 0; i < 10000; i++) {
	
		CitiesIndex * citiesIndex = new CitiesIndex(portal);
		
		unsigned int city_id;
		for(city_id = 0; city_id < citiesIndex->cities_by_id->size; city_id++) {
			City * city = (City *)svget(citiesIndex->cities_by_id,city_id);
			if(!city) continue;
			
			printf("%s Day %d\n", city->name, city->day);
			start_timer(&fine_timer);
			city->advance_day();
			printf("city cycle % 6ld ms\n", stop_timer(&fine_timer));
		}
		
		
		int millis = stop_timer(&start);
		int seconds = millis/1000;
		int minutes = seconds/60;
		int hours = minutes/60;
		
		millis %= 1000;
		seconds %= 60;
		minutes %= 60;
		
		
		printf("%02d:%02d:%02d.%03d\n", hours,minutes,seconds,millis);
		
		//verify_state(portal);
		
		delete citiesIndex;
	}
	
	// broker_trades();
	delete portal;

}
