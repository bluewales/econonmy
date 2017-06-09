#include <stdio.h>
#include <stdlib.h>

#include "MysqlPortal.h"
#include "Citizen.h"
#include "Good.h"
#include "PlayerOffer.h"

/*

+-----------+-----------+------+-----+-------------------+----------------+
| Field     | Type      | Null | Key | Default           | Extra          |
+-----------+-----------+------+-----+-------------------+----------------+
| id        | int(11)   | NO   | PRI | NULL              | auto_increment |
| user_id   | int(11)   | YES  |     | NULL              |                |
| good_id   | int(11)   | YES  |     | NULL              |                |
| city_id   | int(11)   | YES  |     | NULL              |                |
| to_sell   | int(11)   | YES  |     | NULL              |                |
| stock     | int(11)   | YES  |     | NULL              |                |
| min_price | int(11)   | YES  |     | NULL              |                |
| balance   | int(11)   | YES  |     | 0                 |                |
| created   | timestamp | NO   |     | CURRENT_TIMESTAMP |                |
+-----------+-----------+------+-----+-------------------+----------------+


*/



PlayerOffer::PlayerOffer(int id, Good * good, int count, int min_price, int balance, int user_id, MysqlPortal * portal) {
	this->id = id;
	this->good = good;
	this->count = count;
	this->min_price = min_price;
	this->balance = balance;
	this->user_id = user_id;
	this->portal = portal;
	this->traded = 0;
}
PlayerOffer::~PlayerOffer() {
	char query[1024];
	
	if(traded) {
		sprintf(query,"update player_offers set stock=stock-%d, balance=%d where id=%d",
		this->traded, this->balance, this->id);
		portal->submitQuery(query);
	}
}

int PlayerOffer::getSellable(int price) {
	if(price >= this->min_price) {
		return this->count;
	} else {
		return 0;
	}
}

int PlayerOffer::sell(int num_to_sell, int price) {

	int sellable = this->getSellable(price);

	if(num_to_sell > sellable) {
		num_to_sell = sellable;
	}
	
	this->traded += num_to_sell;
	this->count -= num_to_sell;
	this->balance += num_to_sell * price;
	
	return num_to_sell;
}