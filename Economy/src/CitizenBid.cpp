#include <stdio.h>
#include <stdlib.h>

#include "MysqlPortal.h"
#include "Citizen.h"
#include "Good.h"
#include "CitizenBid.h"

CitizenBid::CitizenBid(Good * good, int count, int max_price, Citizen * citizen) {
	this->good = good;
	this->citizen = citizen;
	this->max_price = max_price;
	this->count = count;
}
CitizenBid::~CitizenBid() {
	
}

int CitizenBid::getBuyable(int price) {

	if(price <= max_price) {

		return this->count;
	} else {
		return 0;
	}
}

int CitizenBid::buy(int num_to_buy, int price) {

	if(num_to_buy > this->count) {
		num_to_buy = this->count;
	}
	
	this->citizen->add_new_good(good->id, num_to_buy);
	this->citizen->remove_money(price * num_to_buy);
	
	this->count -= num_to_buy;
	
	return num_to_buy;
}