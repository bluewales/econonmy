#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "goods.h"
#include "professions.h"
#include "citizens.h"
#include "affinities.h"
#include "skills.h"
#include "fastRandom.h"
#include "Market.h"
#include "cities.h"
#include "possessions.h"
#include "myutil.h"
#include "potentials.h"
#include "prices.h"
#include "recipes.h"
#include "captains.h"
#include "inventories.h"
#include "player_trades.h"
#include "names.h"

void purge_tables(MysqlPortal * portal) {
	purge_professions_table(portal);
	purge_citizens_table(portal);
	purge_goods_table(portal);
	purge_affinities_table(portal);
	purge_skills_table(portal);
	purge_cities_table(portal);
	purge_possessions_table(portal);
	purge_potentials_table(portal);
	purge_prices_table(portal);
	purge_recipes_table(portal);
	purge_captains_table(portal);
	purge_inventories_table(portal);
	purge_player_trades_tables(portal);
	purge_names_tables(portal);
} // purge_tables

int main() {
	int success = 1;
	MysqlPortal * portal = new MysqlPortal();
	struct timeval timer;
    long mtime;
	int n = 1000;
	int variation = 100;
	
	
	
	gettimeofday(&timer, NULL);
	sfastRandom(timer.tv_sec+timer.tv_usec);
	
	
	purge_tables(portal);
	
	start_timer(&timer);
	printf("Load professions . . . \n");
	load_professions(portal);
	if(!verify_professions(portal))
		success = 0;
    mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Load goods . . . \n");
	load_goods(portal);
	if(!verify_goods(portal))
		success = 0;
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Create Cities . . . \n");
	
	
	
	//createCity(MysqlPortal * portal, char * name, int plains, int mountain, int biome, int population)
	
	createCity(portal, (char *)"St Charles", get_KISS()%101, get_KISS()%101, 0, n + get_KISS() % variation - variation/2);
	//createCity(portal, (char *)"Bloomington",get_KISS()%101, get_KISS()%101, 0, n + get_KISS() % variation - variation/2);
	//createCity(portal, (char *)"Fish Haven", 97, 21, 0, n + get_KISS() % variation - variation/2);
	//createCity(portal, (char *)"Georgetown", 16, 82, 0, n + get_KISS() % variation - variation/2);
	/*
	createCity(portal, (char *)"Bennington", get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Sharon",     get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Geneva",     get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Liberty",    get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Nounan",     get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Ovid",       get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Pegram",     get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Raymond",    get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Wardboro",   get_KISS()%101, get_KISS()%101, 0, n);
	createCity(portal, (char *)"Montpelier", get_KISS()%101, get_KISS()%101, 0, n);
	*/
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Generate affinities . . . \n");
	generate_affinities(portal);
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Generate possessions . . . \n");
	generatePossessions(portal);
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Generate skills . . . \n");
	generate_skills(portal);
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Generate potentials . . . \n");
	generate_potentials(portal);
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	start_timer(&timer);
	printf("Generate prices . . . \n");
	generate_prices(portal);
	mtime = stop_timer(&timer);
	printf("  Done %ld milliseconds\n", mtime);
	
	if(!success) {
		printf("\nInitialization failed.  Dropping tables.\n");
		purge_tables(portal);
	}
	
	delete portal;
} // main
