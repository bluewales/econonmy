
#include "stdlib.h"
#include "string.h"

#include "City.h"
#include "MysqlPortal.h"
#include "Market.h"
#include "GoodsIndex.h"
#include "RecipesIndex.h"
#include "CitizensIndex.h"
#include "ProfessionsIndex.h"
#include "citizenAI.h"
#include "Citizen.h"

City::City(MysqlPortal * portal, int city_id) {
	char query[1024];

	MYSQL_ROW city_row;
	MYSQL_RES *cities_result; 
	unsigned int cities_num_fields, cities_num_rows;
	
	sprintf(query, "SELECT id,name,day,plains_score,mountain_score,cultural_biome FROM cities where id=%d", city_id);
	cities_result = portal->submitQuery(query);
	cities_num_fields = mysql_num_fields(cities_result);
	cities_num_rows = mysql_num_rows(cities_result);
	
	city_row = mysql_fetch_row(cities_result);
	this->id = atoi(city_row[0]);
	
	this->name = (char *)malloc(strlen(city_row[1]) + 1);
	strcpy(this->name, city_row[1]);
	
	this->day = atoi(city_row[2]);
	
	this->plains_score = atoi(city_row[3]);
	this->mountain_score = atoi(city_row[4]);
	this->cultural_biome = atoi(city_row[5]);
	
	mysql_free_result(cities_result);
	
	this->portal = portal;
	
	this->market = new Market();
	this->goodsIndex = new GoodsIndex(this);
	this->recipesIndex = new RecipesIndex(this);
	this->citizensIndex = new CitizensIndex(this);
	this->professionsIndex = new ProfessionsIndex(portal);
}

City::~City() {
	free(this->name);
	delete market;
	delete goodsIndex;
	delete recipesIndex;
	delete citizensIndex;
	delete professionsIndex;
}

void City::advance_day() {

	char query[1024];

	citizen_production(this->portal, this);
	
	this->market->facilitate_trades(this);
	
	this->citizensIndex->push_delta_possessed(this->portal);
	this->citizensIndex->push_delta_money(this->portal);
	
	sprintf(query, "UPDATE cities set day=day+1 where id=%d", this->id);
	portal->submitQuery(query);
	this->day++;
	
	sprintf(query,"update possessions p inner join goods g on g.id=p.good_id inner join citizens c on c.id=p.citizen_id\
	set p.possessed=p.possessed-dead_goods(p.possessed,g.durability) where p.possessed>0 and c.city_id=%d", this->id);

	portal->submitQuery(query);
	//printf("%d rows deleted\n",portal->affected_rows());
}
