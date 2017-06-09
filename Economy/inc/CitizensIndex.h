#ifndef _citizensindex_h_
#define _citizensindex_h_

#include "smallVector.h"
class City;
class MysqlPortal;



class CitizensIndex {
	public:
	smallVector * citizens_by_id;
	City * city;
	CitizensIndex(City * city);
	~CitizensIndex();
	
	void push_delta_possessed(MysqlPortal * portal);
	void push_delta_money(MysqlPortal * portal);
};

#endif
