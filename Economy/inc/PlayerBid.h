#ifndef _playerbid_h_
#define _playerbid_h_

#include "Bid.h"

class Good;
class Citizen;

class PlayerBid: public Bid {
	public:
	int id;
	int balance;
	int user_id;
	int traded;
	MysqlPortal * portal;
	
	PlayerBid(int id, Good * good, int count, int max_price, int balance, int user_id, MysqlPortal * portal);
	~PlayerBid();
	
	int getBuyable(int price);
	int buy(int count, int price);
};
	
#endif