#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "City.h"
#include "GoodsIndex.h"
#include "Good.h"
#include "MysqlPortal.h"

GoodsIndex::GoodsIndex(City * city) {
	MysqlPortal * portal = city->portal;

	MYSQL_ROW good_row;
	MYSQL_RES *goods_result; 
	unsigned int goods_num_fields, goods_num_rows;
	
	
	// load goods out of the database
	
	
	
	goods_result = portal->submitQuery((char*)"SELECT \
id,name,made_by_profession,made_by_profession_id,\
function_for_affinity,function_for_utility,durability,\
function_for_potential,used_by_profession,class FROM goods"); 

	goods_num_fields = mysql_num_fields(goods_result);
	goods_num_rows = mysql_num_rows(goods_result);
	
	this->goods_by_id = svcreate(goods_num_rows);
	this->goods_by_name = hmcreate();
		
	while ((good_row = mysql_fetch_row(goods_result))) {
	
		int id = atoi(good_row[0]);
		char * name = good_row[1];
		char * made_by_profession = good_row[2];
		int made_by_profession_id = atoi(good_row[3]);
		char * function_for_affinity = good_row[4];
		char * function_for_utility = good_row[5];
		int durability = atoi(good_row[6]);
		char * function_for_potential = good_row[7];
		char * used_by_profession = good_row[8];
		char * good_class = good_row[9];
	
		Good * good = new Good(id, name, made_by_profession, made_by_profession_id, function_for_affinity, function_for_utility, durability, function_for_potential, used_by_profession, good_class);
		svset(this->goods_by_id, good->id, good);
		hmset(this->goods_by_name, good->name, good);
	}
	
	mysql_free_result(goods_result);

	char query[1024];
	sprintf(query,"SELECT prices.good_id,prices.price,potentials.potential,prices.momentum\
	FROM prices inner join potentials on prices.good_id=potentials.good_id\
	and prices.city_id=potentials.city_id where prices.city_id=%d", city->id);
	goods_result = portal->submitQuery(query);
	
	while ((good_row = mysql_fetch_row(goods_result))) {
		int good_id = atoi(good_row[0]);
		int price = atoi(good_row[1]);
		int potential = atoi(good_row[2]);
		int momentum = atoi(good_row[3]);
		Good * good = (Good *)svget(this->goods_by_id, good_id);
		good->price = price;
		good->potential = potential;
		good->momentum = momentum;
	}
	mysql_free_result(goods_result);
}

GoodsIndex::~GoodsIndex() {
	int i;
	for(i = 0; (unsigned int)i <= this->goods_by_id->size; i++) {
		Good * good = (Good *)svget(this->goods_by_id, i);
		if(good) {
			delete good;
		}
	}
	svdestroy(this->goods_by_id);
	hmdestroy(this->goods_by_name);
}


void GoodsIndex::from_goods_list_to_vector(char * list, smallVector * vector) {
	
	while(list && list[0]) {
		int n = 1;
		char good_name[30];
		char * strptr = strchr(list,',');
		char * amount = strchr(list,':');
		int name_length = strptr ? strptr - list : strlen(list);
		if(amount && (!strptr || amount < strptr)) {
			name_length = amount - list;
			n = atoi(amount+1);
		}
		
		memcpy(good_name, list, (name_length));
		good_name[name_length] = 0;
		if(strptr) strptr++;
		list = strptr;
		
		
		Good * good = (Good *)hmget(this->goods_by_name, good_name);
		if(good) {
			int good_id = good->id;
			int previous_amount;
			previous_amount = (int)svget(vector, good_id);
			svset(vector, good_id, (void *)(previous_amount + n));
		} else {
			
			printf("from_goods_list_to_vector : '%s' wasn't in the index\n", good_name);
		}
	}
}
