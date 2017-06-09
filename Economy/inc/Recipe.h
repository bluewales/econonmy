#ifndef _recipe_h_
#define _recipe_h_

#include "smallVector.h"
#include "rvpParser.h"

class GoodsIndex;

#define ONE_ROUND_FOOD 5


class Recipe {
	public:
	int id;
	int good_id;
	char * action;
	char * required_materials;
	char * required_tools;
	char * beneficial_tools;
	char * function_for_production;
	
	Recipe(int id, int good_id, char * action, char * required_materials,
			char * required_tools, char * beneficial_tools,
			char * function_for_production);
	~Recipe();
	
	int get_cost_to_produce(GoodsIndex * goodsIndex);
	smallVector * get_required_materials_vector(GoodsIndex * goodsIndex);
	smallVector * get_required_tools_vector(GoodsIndex * goodsIndex);
	smallVector * get_beneficial_tools_vector(GoodsIndex * goodsIndex);
	smallVector * get_requirements_for_production(GoodsIndex * goodsIndex);
	
	int get_production(int skill, int beneficial_tools, GoodsIndex * goodsIndex);
	
	private:
	int cost_to_produce;
	
	smallVector * required_materials_vector;
	smallVector * required_tools_vector;
	smallVector * beneficial_tools_vector;
	smallVector * requirements_for_production;
};

#endif
