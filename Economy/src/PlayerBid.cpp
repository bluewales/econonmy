#include <stdio.h>
#include <stdlib.h>

#include "MysqlPortal.h"
#include "Citizen.h"
#include "Good.h"
#include "PlayerBid.h"

/*

+-----------+-----------+------+-----+-------------------+----------------+
| Field     | Type      | Null | Key | Default           | Extra          |
+-----------+-----------+------+-----+-------------------+----------------+
| id        | int(11)   | NO   | PRI | NULL              | auto_increment |
| user_id   | int(11)   | YES  |     | NULL              |                |
| good_id   | int(11)   | YES  |     | NULL              |                |
| city_id   | int(11)   | YES  |     | NULL              |                |
| to_buy    | int(11)   | YES  |     | NULL              |                |
| stock     | int(11)   | YES  |     | NULL              |                |
| max_price | int(11)   | YES  |     | NULL              |                |
| balance   | int(11)   | YES  |     | 0                 |                |
| created   | timestamp | NO   |     | CURRENT_TIMESTAMP |                |
+-----------+-----------+------+-----+-------------------+----------------+

*/



PlayerBid::PlayerBid(int id, Good * good, int count, int max_price, int balance, int user_id, MysqlPortal * portal) {
	this->id = id;
	this->good = good;
	this->count = count;
	this->max_price = max_price;
	this->balance = balance;
	this->user_id = user_id;
	this->portal = portal;
	this->traded = 0;
}

PlayerBid::~PlayerBid() {
	char query[1024];
	if(traded) {
		sprintf(query,"update player_bids set stock=stock+%d, balance=%d where id=%d",
		this->traded, this->balance, this->id);
		portal->submitQuery(query);
	}
}

int PlayerBid::getBuyable(int price) {
	if(price <= this->max_price || this->max_price == 0) {
		if(this->balance/price > count) {
			return this->count;
		} else {
			return this->balance/price;
		}
	} else {
		return 0;
	}
}

int PlayerBid::buy(int num_to_buy, int price) {

	int buyable = this->getBuyable(price);

	if(num_to_buy > buyable) {
		num_to_buy = buyable;
	}
	
	this->traded += num_to_buy;
	this->count -= num_to_buy;
	this->balance -= num_to_buy * price;
	
	return num_to_buy;
}