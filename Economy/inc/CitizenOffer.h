#ifndef _citizenoffer_h_
#define _citizenoffer_h_

#include "Offer.h"

class Good;
class Citizen;

class CitizenOffer: public Offer {
	public:
	Citizen * citizen;
	
	CitizenOffer(Good * good, int count, int min_price, Citizen * citizen);
	~CitizenOffer();
	
	int getSellable(int price);
	int sell(int count, int price);
};
	
#endif