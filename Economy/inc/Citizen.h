#ifndef _citizen_h_
#define _citizen_h_

#include "MysqlPortal.h"
#include "smallVector.h"
class Good;
class CitizensIndex;
class GoodsIndex;
class ProfessionsIndex;
class City;
class Recipe;

class Citizen {

	

	public:
	int id;
	int city_id;
	char * first_name;
	char * last_name;
	int birthdate;
	char gender;
	int product_id;
	int money;
	int delta_money;
	int alive;
	smallVector * affinities;
	smallVector * skills;
	smallVector * possessions;
	smallVector * possessions_after_trading;
	
	MysqlPortal * portal;
	
	smallVector * delta_possessed;
	
	Citizen(MYSQL_ROW row, MysqlPortal * portal);
	~Citizen();
	
	void add_new_good(int good_id, int count);
	void remove_good(int good_id, int count);
	void add_money(int money);
	void remove_money(int money);
	void scavenge(GoodsIndex * goodsIndex);
	int produce(Recipe * recipe, Good * product, GoodsIndex * goodsIndex);
	
	int has_requirements_for_production(smallVector * required_goods);
	int has_food_for_procution(GoodsIndex * goodsIndex);
	Good * get_most_coveted_good(GoodsIndex * goodsIndex, ProfessionsIndex * pindex, int * margin_of_utility, int first);
	double get_utility_of_good(Good * good, ProfessionsIndex * pindex);
	int get_quantity_producable(Recipe * recipe, GoodsIndex * index, int realistic);
	int get_num_to_sell(Good * good, int max, double utility_of_money, GoodsIndex * goodsIndex, ProfessionsIndex * pindex, int * minimum_price);
	int get_beneficial_tool_score(Recipe * recipe, GoodsIndex * index);
	int get_utilizing_skill_score(Good * good, ProfessionsIndex * pindex);
	int get_cost_of_unowned_tools(Recipe * recipe, GoodsIndex * goodsIndex);
	
	private:
	
	typedef struct {
		double utility;
		int possessed;
	} utility_cache;
	
	smallVector * utilities;
	
};



#endif
