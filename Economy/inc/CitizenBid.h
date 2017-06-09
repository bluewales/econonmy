#ifndef _citizenbid_h_
#define _citizenbid_h_

#include "Bid.h"

class Good;
class Citizen;

class CitizenBid: public Bid {
	public:
	Citizen * citizen;
	
	CitizenBid(Good * good, int count, int max_price, Citizen * citizen);
	~CitizenBid();
	
	int getBuyable(int price);
	int buy(int count, int price);
};
	
#endif