#include <stdio.h>
#include <stdlib.h>

#include "PlayerTrades.h"
#include "MysqlPortal.h"
#include "City.h"
#include "GoodsIndex.h"
#include "Good.h"
#include "fastRandom.h"


PlayerTrades::PlayerTrades(City * city) {
	char query[2014];

	this->city = city;
	this->portal = this->city->portal;
	this->offers = svcreate(0);
	this->bids = svcreate(0);
	
	MysqlPortal * portal = city->portal;

	MYSQL_ROW offer_row, bid_row;
	MYSQL_RES * offers_result, * bids_result; 
	unsigned int offers_num_fields, offers_num_rows, bids_num_fields, bids_num_rows;
	
	// load player offers out of the database
	sprintf(query,(char*)"SELECT o.id,o.user_id,o.good_id,o.stock,o.min_price FROM \
	player_offers as o inner join prices on o.city_id=prices.city_id and o.good_id=prices.good_id \
	where o.stock>0 and o.min_price<=prices.price and o.city_id=%d", this->city->id);
	offers_result = portal->submitQuery(query); 
	offers_num_fields = mysql_num_fields(offers_result);
	offers_num_rows = mysql_num_rows(offers_result);
	
		
	while ((offer_row = mysql_fetch_row(offers_result))) {
		int id = atoi(offer_row[0]);
		int user_id = atoi(offer_row[1]);
		int good_id = atoi(offer_row[2]);
		int stock = atoi(offer_row[3]);
		int min_price = atoi(offer_row[4]);
		Good * good = (Good *)svget(this->city->goodsIndex->goods_by_id, good_id);
		Offer * offer = new Offer(id, good, stock, min_price, user_id);
		
		smallVector * good_offers = (smallVector *)svget(this->offers,good->id);
		if(!good_offers) {
			good_offers = svcreate(0);
			svset(this->offers, good->id, good_offers);
		}
		svset(good_offers, good_offers->count, offer);
	}
	mysql_free_result(offers_result);
	
	// load player bids out of the database
	sprintf(query,(char*)"SELECT b.id,b.user_id,b.good_id,b.to_buy-b.stock as count,b.max_price,b.balance FROM \
	player_bids as b inner join prices on b.city_id=prices.city_id and b.good_id=prices.good_id \
	where b.balance>prices.price and b.max_price>=prices.price and b.city_id=%d", this->city->id);
	
	bids_result = portal->submitQuery(query); 
	bids_num_fields = mysql_num_fields(bids_result);
	bids_num_rows = mysql_num_rows(bids_result);
	
		
	while ((bid_row = mysql_fetch_row(bids_result))) {
		int id = atoi(bid_row[0]);
		int user_id = atoi(bid_row[1]);
		int good_id = atoi(bid_row[2]);
		int count = atoi(bid_row[3]);
		int max_price = atoi(bid_row[4]);
		int balance = atoi(bid_row[5]);
		Good * good = (Good *)svget(this->city->goodsIndex->goods_by_id, good_id);
		Bid * bid = new Bid(id, good, count, max_price, balance, user_id);
		
		printf("id:%d user_id:%d good_id:%d count:%d max_price:%d balance:%d\n", id, user_id, good_id, count, max_price, balance);
		
		smallVector * good_bids = (smallVector *)svget(this->bids,good->id);
		if(!good_bids) {
			good_bids = svcreate(0);
			svset(this->bids, good->id, good_bids);
		}
		svset(good_bids, good_bids->count, bid);
	}
	mysql_free_result(bids_result);
}

PlayerTrades::~PlayerTrades() {
	unsigned int i, j;
	for(i = 0; i< this->offers->size; i++) {
		smallVector * good_offers = (smallVector *)svget(this->offers,i);
		if(!good_offers) continue;
		for(j = 0; j< good_offers->size; j++) {
			Offer * offer = (Offer *)svget(good_offers,j);
			if(!offer) continue;
			delete offer;
		}
		svdestroy(good_offers);
	}
	for(i = 0; i< this->bids->size; i++) {
		smallVector * good_bids = (smallVector *)svget(this->bids,i);
		if(!good_bids) continue;
		for(j = 0; j< good_bids->size; j++) {
			Bid * bid = (Bid *)svget(good_bids,j);
			if(!bid) continue;
			delete bid;
		}
		svdestroy(good_bids);
	}
	svdestroy(this->offers);
	svdestroy(this->bids);
}


PlayerTrades::Bid * PlayerTrades::removeBid(Good * good) {
	smallVector * good_bids = (smallVector *)svget(this->bids,good->id);
	if(!good_bids || good_bids->count == 0) {
		return 0;
	}
	int index = 0;
	Bid * result = 0;
	do {
		index = mwc_rand() % good_bids->count;
		result = (Bid *)svget(good_bids,index);
	} while(!result);
	
	svset(good_bids, index, svget(good_bids, good_bids->count - 1));
	svset(good_bids, good_bids->count-1, 0);
	return result;
}

PlayerTrades::Offer * PlayerTrades::removeOffer(Good * good) {
	smallVector * good_offers = (smallVector *)svget(this->offers,good->id);
	if(!good_offers || good_offers->count == 0) {
		return 0;
	}
	int index = 0;
	Offer * result = 0;
	do {
		index = mwc_rand() % good_offers->count;
		result = (Offer *)svget(good_offers,index);
	} while(!result);
	
	svset(good_offers, index, svget(good_offers, good_offers->count - 1));
	svset(good_offers, good_offers->count-1, 0);
	return result;
}

