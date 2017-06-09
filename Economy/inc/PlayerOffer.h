#ifndef _playeroffer_h_
#define _playeroffer_h_

#include "Offer.h"

class Good;
class Citizen;

class PlayerOffer: public Offer {
	public:
	int id;
	int balance;
	int user_id;
	int traded;
	MysqlPortal * portal;
	
	PlayerOffer(int id, Good * good, int count, int min_price, int balance, int user_id, MysqlPortal * portal);
	~PlayerOffer();
	
	int getSellable(int price);
	int sell(int count, int price);
};
	
#endif