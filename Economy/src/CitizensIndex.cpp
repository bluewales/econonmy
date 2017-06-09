
#include "stdlib.h"

#include "City.h"
#include "CitizensIndex.h"
#include "Citizen.h"
#include "MysqlPortal.h"
#include "myutil.h"


CitizensIndex::~CitizensIndex() {
	int i;
	
	for(i = 0; (unsigned int)i <= this->citizens_by_id->size; i++) {
		Citizen * citizen = (Citizen *)svget(this->citizens_by_id, i);
		if(citizen) {
			delete citizen;
		}
	}
	svdestroy(this->citizens_by_id);
}

CitizensIndex::CitizensIndex(City * city) {
	this->city = city;
	MysqlPortal * portal = city->portal;
	char query[1024];
	
	MYSQL_ROW citizen_row;
	MYSQL_RES *citizens_result; 
	unsigned int citizens_num_fields, citizens_num_rows;
	
	//struct timeval timer;
	//start_timer(&timer);
	sprintf(query,"SELECT * FROM citizens where city_id=%d", city->id);
	citizens_result = portal->submitQuery(query); 
	citizens_num_fields = mysql_num_fields(citizens_result);
	citizens_num_rows = mysql_num_rows(citizens_result);

	this->citizens_by_id = svcreate(citizens_num_rows);
		
	while ((citizen_row = mysql_fetch_row(citizens_result))) {
		Citizen * citizen = new Citizen(citizen_row, portal);
		svset(this->citizens_by_id, citizen->id, citizen);
	}
	mysql_free_result(citizens_result);
	//printf("      load citizens      % 6ld ms\n", stop_timer(&timer));
	
	MYSQL_ROW affinities_row, possessions_row;
	MYSQL_RES *affinities_result, *possessions_result;

	//start_timer(&timer);
	sprintf(query,"SELECT a.citizen_id,a.good_id,a.affinity \
	FROM affinities as a inner join citizens as c on a.citizen_id=c.id where c.city_id=%d", city->id);
	affinities_result = portal->submitQuery(query); 
	while ((affinities_row = mysql_fetch_row(affinities_result))) {
		int citizen_id = atoi(affinities_row[0]);
		Citizen * citizen = (Citizen *)svget(this->citizens_by_id, citizen_id);
		
		int good_id = atoi(affinities_row[1]);
		void * affinity = (void *)atoi(affinities_row[2]);
		
		svset(citizen->affinities, good_id, affinity);
	}
	mysql_free_result(affinities_result);
	//printf("      load affinities    % 6ld ms\n", stop_timer(&timer));
	
	//start_timer(&timer);
	sprintf(query,"SELECT p.citizen_id,p.good_id,p.possessed \
	FROM possessions as p inner join citizens as c on p.citizen_id=c.id where c.city_id=%d", city->id);
	possessions_result = portal->submitQuery(query); 
	while ((possessions_row = mysql_fetch_row(possessions_result))) {
		int citizen_id = atoi(possessions_row[0]);
		Citizen * citizen = (Citizen *)svget(this->citizens_by_id, citizen_id);
		
		int good_id = atoi(possessions_row[1]);
		void * possessed = (void *)atoi(possessions_row[2]);
		
		svset(citizen->possessions, good_id, possessed);
		svset(citizen->possessions_after_trading, good_id, possessed);
	}
	mysql_free_result(possessions_result);
	//printf("      load possessions    % 6ld ms\n", stop_timer(&timer));
	
	//start_timer(&timer);
	MYSQL_ROW skills_row;
	MYSQL_RES * skills_result;
	sprintf(query,"SELECT s.citizen_id,s.profession_id,s.skill \
	FROM skills as s inner join citizens as c on s.citizen_id=c.id where c.city_id=%d", city->id);
	skills_result = portal->submitQuery(query);

	while ((skills_row = mysql_fetch_row(skills_result))) {
		Citizen * citizen = (Citizen *)svget(this->citizens_by_id, atoi(skills_row[0]));
		smallVector * skills = (citizen)->skills;
		svset(skills, atoi(skills_row[1]), (void *)(int)atof(skills_row[2]));
	}
	mysql_free_result(skills_result);
	//printf("      load skills        % 6ld ms\n", stop_timer(&timer));
}

void CitizensIndex::push_delta_possessed(MysqlPortal * portal) {
	portal->submitQuery((char *)"create temporary table market_changes (id INT NOT NULL AUTO_INCREMENT,\
	good_id INT,\
	citizen_id INT,\
	delta_possessed INT,\
	PRIMARY KEY (id))");
	
	portal->submitQuery((char *)"create index delta_p_index on market_changes(citizen_id, good_id, delta_possessed)");
	portal->long_query_begin((char *)"INSERT INTO market_changes(good_id,citizen_id,delta_possessed)VALUES");

	unsigned int citizen_id;
	unsigned int good_id;
	for(citizen_id = 0; citizen_id < this->citizens_by_id->size; citizen_id++) {
		Citizen * citizen = (Citizen *)svget(citizens_by_id,citizen_id);
		if(!citizen) continue;
		char add[1024];
		for(good_id = 0; good_id < citizen->delta_possessed->size; good_id++) {
			int n = (int)svget(citizen->delta_possessed, good_id);
			if(!n) continue;
			
			sprintf(add, "(%d,%d,%d)",good_id,citizen_id,n);
			portal->long_query_update(add);
			
			svset(citizen->delta_possessed, good_id, 0);
		}
	}
	
	portal->long_query_finish();
	portal->submitQuery((char *)"UPDATE possessions AS p \
						INNER JOIN market_changes AS mc \
						ON p.citizen_id=mc.citizen_id AND p.good_id=mc.good_id \
						SET p.possessed=p.possessed+mc.delta_possessed");

	portal->submitQuery((char *)"drop table market_changes");
}

void CitizensIndex::push_delta_money(MysqlPortal * portal) {
	portal->submitQuery((char *)"create temporary table market_changes (id INT NOT NULL AUTO_INCREMENT,\
	citizen_id INT,\
	delta_money INT,\
	PRIMARY KEY (id))");
	
	portal->submitQuery((char *)"create index delta_m_index on market_changes(citizen_id,delta_money)");															
	portal->long_query_begin((char *)"INSERT INTO market_changes(citizen_id,delta_money)VALUES");

	unsigned int citizen_id;
	for(citizen_id = 0; citizen_id < this->citizens_by_id->size; citizen_id++) {
		Citizen * citizen = (Citizen *)svget(citizens_by_id, citizen_id);
		if(!citizen || !citizen->delta_money) continue;
		char add[1024];
		
		sprintf(add, "(%d,%d)", citizen_id, citizen->delta_money);
		portal->long_query_update(add);
		
		citizen->delta_money = 0;
	}
	
	portal->long_query_finish();
	portal->submitQuery((char *)"UPDATE citizens AS c \
						INNER JOIN market_changes AS mc \
						ON c.id=mc.citizen_id \
						SET c.money=c.money+mc.delta_money");

	portal->submitQuery((char *)"drop table market_changes");
}
