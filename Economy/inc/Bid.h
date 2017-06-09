#ifndef _bid_h_
#define _bid_h_

class Good;

class Bid {
	public:
	Good * good;
	int count;
	int max_price;
	
	virtual ~Bid(){}
	
	virtual int getBuyable(int price) = 0;
	virtual int buy(int count, int price) = 0;
};
	
#endif