#include <stdio.h>
#include <stdlib.h>

#include "Market.h"
#include "MysqlPortal.h"
#include "Citizen.h"
#include "Good.h"
#include "GoodsIndex.h"
#include "myutil.h"
#include "City.h"
#include "CitizensIndex.h"
#include "fastRandom.h"
#include "PlayerTrades.h"
#include "Bid.h"
#include "Offer.h"
#include "CitizenBid.h"
#include "CitizenOffer.h"
#include "PlayerBid.h"
#include "PlayerOffer.h"


Market::Market() {
	this->portal = portal;
	this->offers = svcreate(0);
	this->bids = svcreate(0);
}

void Market::load_player_trades(City * city) {

	MysqlPortal * portal = city->portal;
	char query[1024];

	MYSQL_ROW offer_row, bid_row;
	MYSQL_RES * offers_result, * bids_result; 
	unsigned int offers_num_fields, offers_num_rows, bids_num_fields, bids_num_rows;
	
	// load player offers out of the database
	sprintf(query,(char*)"SELECT o.id,o.user_id,o.good_id,o.stock,o.min_price,o.balance FROM \
	player_offers as o inner join prices on o.city_id=prices.city_id and o.good_id=prices.good_id \
	where o.stock>0 and o.min_price<=prices.price and o.city_id=%d", city->id);
	offers_result = portal->submitQuery(query); 
	offers_num_fields = mysql_num_fields(offers_result);
	offers_num_rows = mysql_num_rows(offers_result);
	
	while ((offer_row = mysql_fetch_row(offers_result))) {
		int id = atoi(offer_row[0]);
		int user_id = atoi(offer_row[1]);
		int good_id = atoi(offer_row[2]);
		int stock = atoi(offer_row[3]);
		int min_price = atoi(offer_row[4]);
		int balance = atoi(offer_row[5]);
		Good * good = (Good *)svget(city->goodsIndex->goods_by_id, good_id);
		Offer * offer = new PlayerOffer(id, good, stock, min_price, balance, user_id, portal);
		
		printf("id:%d user_id:%d good_id:%d count:%d min_price:%d balance:%d\n", 
		id, user_id, good_id, stock, min_price, balance);
		
		addOffer(offer);
		
		
	}
	mysql_free_result(offers_result);
	
	
	// load player bids out of the database
	sprintf(query,(char*)"SELECT b.id,b.user_id,b.good_id,b.to_buy-b.stock as count,b.max_price,b.balance FROM \
	player_bids as b inner join prices on b.city_id=prices.city_id and b.good_id=prices.good_id \
	where b.balance>prices.price and b.max_price>=prices.price and b.city_id=%d", city->id);
	
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
		Good * good = (Good *)svget(city->goodsIndex->goods_by_id, good_id);
		PlayerBid * bid = new PlayerBid(id, good, count, max_price, balance, user_id, portal);
		
		addBid(bid);
	}
	mysql_free_result(bids_result);
}

Market::~Market() {
	unsigned int i;
	for(i = 0; i< this->offers->size; i++) {
		smallVector * good_offers = (smallVector *)svget(this->offers,i);
		if(!good_offers) continue;
		svdestroy(good_offers);
//		for(j = 0; j< good_offers->size; j++) {
//			Offer * offer = (Offer *)svget(good_offers,j);
//			if(!offer) continue;
//			delete offer;
//		}
	}
	for(i = 0; i< this->bids->size; i++) {
		smallVector * good_bids = (smallVector *)svget(this->bids,i);
		if(!good_bids) continue;
		svdestroy(good_bids);
//		for(j = 0; j< good_bids->size; j++) {
//			Bid * bid = (Bid *)svget(good_bids,j);
//			if(!bid) continue;
//			delete bid;
//		}
	}
	svdestroy(this->offers);
	svdestroy(this->bids);
}

int Market::submit_bid_from_citizen(Good * good,int count,int max_price,Citizen * citizen) {

	CitizenBid * bid = new CitizenBid(good,count,max_price,citizen);
	addBid(bid);
	
	citizen->money -= max_price * count;
	if(citizen->money < 0) {
		printf("%d has %d money\n", citizen->id, citizen->money);
		exit(0);
	}
	svset(citizen->possessions_after_trading, good->id, (void*)(((int)svget(citizen->possessions_after_trading, good->id))+count));
	
	return 0; 
}


int Market::submit_offer_from_citizen(Good * good,int count,int min_price,Citizen * citizen) {
	
	CitizenOffer * offer =  new CitizenOffer(good,count,min_price,citizen);
	addOffer(offer);
	
	svset(citizen->possessions_after_trading, good->id, (void*)(((int)svget(citizen->possessions_after_trading, good->id))-count));
	return 0;
}

void Market::addBid(Bid * bid) {
	smallVector * good_bids = (smallVector *)svget(this->bids,bid->good->id);
	if(!good_bids) {
		good_bids = svcreate(0);
		svset(this->bids, bid->good->id, good_bids);
	}
	
	svset(good_bids, good_bids->count, bid);
}

void Market::addOffer(Offer * offer) {
	smallVector * good_offers = (smallVector *)svget(this->offers,offer->good->id);
	if(!good_offers) {
		good_offers = svcreate(0);
		svset(this->offers, offer->good->id, good_offers);
	}
	
	svset(good_offers, good_offers->count, offer);
}

Bid * Market::removeBid(Good * good) {
	smallVector * good_bids = (smallVector *)svget(this->bids,good->id);
	if(!good_bids || good_bids->count == 0) {
		return 0;
	}
	int index = 0;
	Bid * result = 0;
	do {
		index = CONG() % good_bids->count;
		result = (Bid *)svget(good_bids,index);
	} while(!result);
	
	svset(good_bids, index, svget(good_bids, good_bids->count - 1));
	svset(good_bids, good_bids->count-1, 0);
	return result;
}

Offer * Market::removeOffer(Good * good) {
	smallVector * good_offers = (smallVector *)svget(this->offers,good->id);
	if(!good_offers || good_offers->count == 0) {
		return 0;
	}
	int index = 0;
	Offer * result = 0;
	do {
		index = CONG() % good_offers->count;
		result = (Offer *)svget(good_offers,index);
	} while(!result);
	
	svset(good_offers, index, svget(good_offers, good_offers->count - 1));
	svset(good_offers, good_offers->count-1, 0);
	return result;
}

// This is on of the most expensive function that I have right now.
// Most trades here wont be serviced, so it would be nice if we didn't have to undo 
// partially done trades	
int Market::facilitate_trades(City * city) {
	unsigned int good_id;
	char query[1024];
	
	MysqlPortal * portal = city->portal;
	GoodsIndex * goodsIndex = city->goodsIndex;
	
	this->load_player_trades(city);

	printf("Start trades\n");
	
	for(good_id = 0; good_id < goodsIndex->goods_by_id->size; good_id++) {
		Good * good = (Good *)svget(goodsIndex->goods_by_id,good_id);
		if(!good) continue;
		
		int num_offers = 0;
		int num_bids = 0;
		int num_trades = 0;
		int is_coin = 0;
		
		if(!strcmp("gold coin",good->name) || 
				!strcmp("silver coin",good->name) || 
				!strcmp("copper coin",good->name)) {
			is_coin = 1;
		}
		
		Offer * offer = removeOffer(good);
		Bid * bid = removeBid(good);
		
		int market_price = good->price;
		int price_momentum = good->momentum;

		
		
		while(bid && offer) {
			int sellable = offer->getSellable(market_price);
			int buyable = bid->getBuyable(market_price);
			
			if(!sellable) {
				delete offer;
				offer = removeOffer(good);
				continue;
			}
			if(!buyable) {
				delete bid;
				bid = removeBid(good);
				continue;
			}
			
			int trade_count = (sellable > buyable)?buyable:sellable;
			int trade_amount = market_price;
			
			if(trade_count > 0) {

			
				num_trades += trade_count;
				
				offer->sell(trade_count, trade_amount);
				bid->buy(trade_count, trade_amount);
			}
		}
		
		num_offers = num_trades;
		num_bids = num_trades;
		int new_price = market_price;

		if(!bid && !offer) {
			price_momentum = 0;
		}

		if(bid) {
			
			while(bid) {
				num_bids += bid->count;
				if(is_coin) {

					int buyable = bid->getBuyable(market_price);
					num_trades += bid->count;

					bid->buy(buyable, market_price);
				}
				delete bid;
				bid = removeBid(good);
			}

			if(num_trades > 0) {
				price_momentum = 1;
			} else if(price_momentum < 1) {
				price_momentum = 1;
			} else {
				price_momentum += 1;
			}

			new_price = market_price + price_momentum;
			if(new_price <= market_price) {
				new_price = market_price + 1;
			}
		}


		if(offer) {
			
			while(offer) {
				num_offers += offer->count;
				if(is_coin) {
					num_trades += offer->count;

					offer->sell(offer->count, market_price);
				}
				delete offer;
				offer = removeOffer(good);
			}

			if(num_trades > 0) {
				price_momentum = -1;
			} else if(price_momentum > -1) {
				price_momentum = -1;
			} else {
				price_momentum -= 1;
			}

			new_price = market_price + price_momentum;
			if(new_price >= market_price) {
				new_price = market_price - 1;
			}
			if(new_price < 1) new_price = 1;
		}
		if(is_coin) {
		} else {
			sprintf(query,"update prices set price=%d, momentum=%d where good_id=%d and city_id=%d",
				new_price, price_momentum, good_id, city->id);
			portal->submitQuery(query);
		}
		
	}
	
	printf("Done trading\n");
	return 0;
}

