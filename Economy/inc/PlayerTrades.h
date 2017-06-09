#ifndef _playertrades_h_
#define _playertrades_h_

#include "smallVector.h"


class MysqlPortal;
class City;
class Good;



class PlayerTrades {
	public:
	class Offer {
		public:
		int id;
		Good * good;
		int count;
		int min_price;
		int user_id;
		Offer(int id, Good * good, int count,int min_price, int user_id) {
			this->id = id;
			this->good = good;
			this->count = count;
			this->min_price = min_price;
			this->user_id = user_id;
		}
	};

	class Bid {
		public:
		int id;
		Good * good;
		int count;
		int max_price;
		int balance;
		int user_id;
		Bid(int id, Good * good, int count, int max_price, int balance, int user_id) {
			this->id = id;
			this->good = good;
			this->count = count;
			this->max_price = max_price;
			this->balance = balance;
			this->user_id = user_id;
		}
	};
	
	smallVector * offers;
	smallVector * bids;
	
	City * city;
	MysqlPortal * portal;

	PlayerTrades(City * city);
	~PlayerTrades();

	Bid * removeBid(Good * good);
	Offer * removeOffer(Good * good);
};

#endif
