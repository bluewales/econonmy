#ifndef _market_h_
#define _market_h_

#include "smallVector.h"


class Citizen;
class City;
class Market;
class Good;
class MysqlPortal;
class GoodsIndex;
class CitizensIndex;
class Offer;
class Bid;



class Market {
	public:
	
	MysqlPortal * portal;
	
	smallVector * offers;
	smallVector * bids;
	

	Market();
	~Market();
	
	void load_player_trades(City * city);

	int submit_bid_from_citizen(Good * good, int count, int max_price, Citizen * citizen);
	int submit_offer_from_citizen(Good * good, int count, int min_price, Citizen * citizen);
	int facilitate_trades(City * city);
	Bid * removeBid(Good * good);
	Offer * removeOffer(Good * good);
	void addBid(Bid * bid);
	void addOffer(Offer * offer);
};

#endif
