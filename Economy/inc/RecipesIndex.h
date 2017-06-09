#ifndef _recipesindex_h_
#define _recipesindex_h_

#include "hashMap.h"
#include "smallVector.h"

class MysqlPortal;
class City;

class RecipesIndex {
	public:
	smallVector * recipes_by_id;
	
	RecipesIndex(City * city);
	~RecipesIndex();
};


#endif
