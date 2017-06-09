#ifndef _goodsindex_h_
#define _goodsindex_h_

#include "hashMap.h"
#include "smallVector.h"

class MysqlPortal;
class City;

class GoodsIndex {
	public:
	smallVector * goods_by_id;
	hashMap * goods_by_name;
	
	GoodsIndex(City * city);
	~GoodsIndex();
	
	void from_goods_list_to_vector(char * list, smallVector * vector);
};


#endif
