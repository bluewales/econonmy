#ifndef _citizensindex_h_
#define _citizensindex_h_

#include "smallVector.h"
class City;
class MysqlPortal;



class CitiesIndex {
	public:
	smallVector * cities_by_id;
	CitiesIndex(MysqlPortal * portal);
	~CitiesIndex();
};

#endif
