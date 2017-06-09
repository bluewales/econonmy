#ifndef _offer_h_
#define _offer_h_


#include "Good.h"

class Good;

class Offer {
	public:
	Good * good;
	int count;
	int min_price;
	
	virtual ~Offer(){}
	
	virtual int getSellable(int price) = 0;
	virtual int sell(int count, int price) = 0;
};
	
#endif