#ifndef city_h_
#define city_h_

class MysqlPortal;
class Market;
class GoodsIndex;
class RecipesIndex;
class CitizensIndex;
class ProfessionsIndex;
class MysqlPortal;

class City {
	public:
	int id;
	char * name;
	int day;
	int plains_score;
	int mountain_score;
	int cultural_biome;
	Market * market;
	GoodsIndex * goodsIndex;
	RecipesIndex * recipesIndex;
	CitizensIndex * citizensIndex;
	ProfessionsIndex * professionsIndex;
	MysqlPortal * portal;
	
	City(MysqlPortal * portal, int city_id);
	~City();
	
	void advance_day();
};


#endif
