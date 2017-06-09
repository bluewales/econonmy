#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "smallVector.h"
#include "goods.h"
#include "Recipe.h"
#include "rvpParser.h"
#include "Good.h"
#include "GoodsIndex.h"

Recipe::Recipe(int id, int good_id, char * action, char * required_materials,
			char * required_tools, char * beneficial_tools,
			char * function_for_production) {

	this->id = id;
	
	this->good_id = good_id;
	
	this->action = (char *)malloc(strlen(action) + 1);
	strcpy(this->action, action);
	
	this->required_materials = (char *)malloc(strlen(required_materials) + 1);
	strcpy(this->required_materials, required_materials);
	
	this->required_tools = (char *)malloc(strlen(required_tools) + 1);
	strcpy(this->required_tools, required_tools);
	
	this->beneficial_tools = (char *)malloc(strlen(beneficial_tools) + 1);
	strcpy(this->beneficial_tools, beneficial_tools);
	
	this->function_for_production = (char *)malloc(strlen(function_for_production) + 1);
	strcpy(this->function_for_production, function_for_production);
	
	this->cost_to_produce = -1;
	
	this->requirements_for_production = 0;
	this->required_materials_vector = 0;
	this->beneficial_tools_vector = 0;
	this->required_tools_vector = 0;
}

Recipe::~Recipe() {
	free(this->action);
	free(this->required_materials);
	free(this->required_tools);
	free(this->beneficial_tools);
	free(this->function_for_production);
	
	if(this->requirements_for_production) {
		svdestroy(this->requirements_for_production);
	}
	if(this->required_materials_vector) {
		svdestroy(this->required_materials_vector);
	}
	if(this->beneficial_tools_vector) {
		svdestroy(this->beneficial_tools_vector);
	}
	if(this->required_tools_vector) {
		svdestroy(this->required_tools_vector);
	}
}


int Recipe::get_cost_to_produce(GoodsIndex * goodsIndex) {
	
	// if this is the first time we are accessing that good, calculate the cost
	if(this->cost_to_produce < 0) {
		unsigned int good_id = 0;
		this->cost_to_produce = 0;
		this->get_required_materials_vector(goodsIndex);
		
		for(good_id = 0; good_id < this->required_materials_vector->size; good_id++) {
			int quantity = (int)svget(this->required_materials_vector, good_id);
			if(!quantity) continue;
			
			int cost = ((Good *)svget(goodsIndex->goods_by_id, good_id))->price * quantity;
			this->cost_to_produce += cost;
		}
	}
	return this->cost_to_produce;
}



smallVector * Recipe::get_required_tools_vector(GoodsIndex * goodsIndex) {
	if(!this->required_tools_vector) {
		this->required_tools_vector = svcreate(0);
		goodsIndex->from_goods_list_to_vector(this->required_tools, this->required_tools_vector);
	}
	return this->required_tools_vector;
}

smallVector * Recipe::get_beneficial_tools_vector(GoodsIndex * goodsIndex) {
	if(!this->beneficial_tools_vector) {
		this->beneficial_tools_vector = svcreate(0);
		goodsIndex->from_goods_list_to_vector(this->beneficial_tools, this->beneficial_tools_vector);
	}
	return this->beneficial_tools_vector;
}


smallVector * Recipe::get_required_materials_vector(GoodsIndex * goodsIndex) {
	if(!this->required_materials_vector) {
		this->required_materials_vector = svcreate(0);
		goodsIndex->from_goods_list_to_vector(this->required_materials, this->required_materials_vector);
	}
	return this->required_materials_vector;
}


smallVector * Recipe::get_requirements_for_production(GoodsIndex * goodsIndex) {
	if(!this->requirements_for_production) {
		this->requirements_for_production = svcreate(0);
		goodsIndex->from_goods_list_to_vector(this->required_materials, this->requirements_for_production);
		goodsIndex->from_goods_list_to_vector(this->required_tools, this->requirements_for_production);
		Good * food = (Good*)hmget(goodsIndex->goods_by_name, (char *)"food");
		if(food) {
			svset(this->requirements_for_production, food->id, (void*)ONE_ROUND_FOOD);
		}
	}
	return this->requirements_for_production;
}


int Recipe::get_production(int skill, int beneficial_tools, GoodsIndex * goodsIndex) {
	int P[4];
	P[1] = skill;
	P[2] = beneficial_tools;
	P[3] = ((Good *)svget(goodsIndex->goods_by_id, this->good_id))->potential;
	int production = (int)rvp_parse(this->function_for_production, P);
	return production;
}