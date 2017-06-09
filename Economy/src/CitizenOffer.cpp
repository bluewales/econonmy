#include <stdio.h>
#include <stdlib.h>

#include "MysqlPortal.h"
#include "Citizen.h"
#include "Good.h"
#include "Offer.h"
#include "CitizenOffer.h"


CitizenOffer::CitizenOffer(Good * good, int count, int min_price, Citizen * citizen) {
	this->good = good;
	this->citizen = citizen;
	this->min_price = min_price;
	this->count = count;
}
CitizenOffer::~CitizenOffer() {
	
}

int CitizenOffer::getSellable(int price) {
	if(price >= min_price) {
		return this->count;
	} else {
		return 0;
	}
}

int CitizenOffer::sell(int num_to_sell, int price) {

	if(num_to_sell > this->count) {
		num_to_sell = this->count;
	}
	
	this->citizen->remove_good(good->id, num_to_sell);
	this->citizen->add_money(price * num_to_sell);
	
	this->count -= num_to_sell;
	
	return num_to_sell;
}