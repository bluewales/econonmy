#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Market.h"
#include "smallVector.h"
#include "rvpParser.h"
#include "MysqlPortal.h"
#include "Citizen.h"
#include "Profession.h"
#include "CitizensIndex.h"
#include "GoodsIndex.h"
#include "ProfessionsIndex.h"
#include "Good.h"
#include "City.h"
#include "Recipe.h"

Citizen::Citizen(MYSQL_ROW row, MysqlPortal * portal) {

	this->portal = portal;

	unsigned int i = 0;
	id = atoi(row[i++]);
	this->city_id = atoi(row[i++]);

	this->first_name = (char *)malloc(strlen(row[i]) + 1);
	strcpy(this->first_name, row[i++]);

	this->last_name = (char *)malloc(strlen(row[i]) + 1);
	strcpy(this->last_name, row[i++]);

	this->birthdate = atoi(row[i++]);
	this->gender = row[i++][0];
	this->product_id = atoi(row[i++]);
	this->money = atoi(row[i++]);
	this->alive = strcmp(row[i++],"false");
	
	this->affinities = svcreate(0);
	this->skills = svcreate(0);
	this->possessions = svcreate(0);
	this->possessions_after_trading = svcreate(0);
	
	this->utilities = svcreate(0);
	this->delta_possessed = svcreate(0);
	
	this->delta_money = 0;
	
}

Citizen::~Citizen(){
	free(this->first_name);
	free(this->last_name);
	svdestroy(this->skills);
	svdestroy(this->affinities);
	svdestroy(this->possessions);
	svdestroy(this->possessions_after_trading);
	
	svdestroy(this->delta_possessed);
	
	
	unsigned int i;
	for(i = 0; i < this->utilities->size; i++) {
		void * u = svget(this->utilities,i);
		if(u) free(u);
	}
	svdestroy(this->utilities);
}


void Citizen::add_new_good(int good_id, int count) {

	svset(this->delta_possessed, good_id, (void*)((int)svget(this->delta_possessed, good_id)+count));
	//svset(this->possessions, good_id, (void*)((int)svget(this->possessions, good_id)+count));
}


void Citizen::remove_good(int good_id, int count) {
	
	svset(this->delta_possessed, good_id, (void*)((int)svget(this->delta_possessed, good_id)-count));
	svset(this->possessions, good_id, (void*)((int)svget(this->possessions, good_id)-count));
	
}

void Citizen::add_money(int money) {
	this->delta_money += money;
}
void Citizen::remove_money(int money) {
	this->delta_money -= money;
}


void Citizen::scavenge(GoodsIndex * goodsIndex) {
	int newFood = 1;
	Good * food = (Good*)hmget(goodsIndex->goods_by_name, (char *)"food");
	if(!food) return;
	int food_id = food->id;
	//printf("scavenge\n");
	this->add_new_good(food_id, newFood);
}

// notice that this function assumes that the citizen has required materials for production
// if the citizen does not indeed have enough stuff, an error will occur, one that might be very hard indeed to detect and fix
int Citizen::produce(Recipe * product_recipe, Good * product, GoodsIndex * goodsIndex) {
	unsigned int j;
	smallVector * required_goods = svcreate(0);
	goodsIndex->from_goods_list_to_vector(product_recipe->required_materials, required_goods);
	
	
	int max_production = this->get_quantity_producable(product_recipe, goodsIndex, 0);
	int number_produced = max_production;
	
	int cost_to_make = 0;
	
	//printf("produce %s\n", product->name);
	for(j = 0; j < required_goods->size; j++) {
		int num_required = (int)svget(required_goods, j);
		if (!num_required) continue;
		
		int possessed = (int)svget(this->possessions,j);
		if(possessed/num_required < number_produced) {
			number_produced = possessed/num_required;
		}
	}
	for(j = 0; j < required_goods->size; j++) {
		int num_required = (int)svget(required_goods, j);
		if (!num_required) continue;
		
		this->remove_good(j, num_required * number_produced);
		cost_to_make += ((Good *)svget(goodsIndex->goods_by_id,j))->price * num_required;
	}
	
	svdestroy(required_goods);
	
	
	Good * food = (Good*)hmget(goodsIndex->goods_by_name, (char *)"food");
	if(food) this->remove_good(food->id, ONE_ROUND_FOOD);
	
	this->add_new_good(product->id, number_produced);
	return number_produced;
}

int Citizen::has_requirements_for_production(smallVector * required_goods) {
	unsigned int j;
	int production_requirements_met = 1;
	for(j = 0; j < required_goods->size; j++) {
		int num_required = (int)svget(required_goods, j);
		if(num_required) {
			// find out if citizen has num_required of good # j
			int num_possessed = (int)svget(this->possessions, j);
			if(num_required > num_possessed) {
				production_requirements_met = 0;
				break;
			}
		}
	}
	return production_requirements_met;
}

int Citizen::has_food_for_procution(GoodsIndex * goodsIndex) {
	Good * food = (Good *)hmget(goodsIndex->goods_by_name,(char *)"food");
	if(!food) return 1;
	int food_id = food->id;
	return ((int)svget(this->possessions, food_id) >= ONE_ROUND_FOOD);
}

Good * Citizen::get_most_coveted_good(GoodsIndex * goodsIndex, ProfessionsIndex * pindex, int * margin_of_utility, int first) {
	unsigned int i;
	double utility_per_money = 0;
	Good * food = (Good *)hmget(goodsIndex->goods_by_name,(char *)"food");
	Good * most_coveted_good = food?food:(Good *)svget(goodsIndex->goods_by_id,1);
	*margin_of_utility = 0;
	double prev_utility = 1;
	double prev_value = 1;
	for(i = 0; i < goodsIndex->goods_by_id->size; i++) {
		Good * good = (Good *)svget(goodsIndex->goods_by_id, i);
		
		if(!good)
			continue;
		double utility = (double)this->get_utility_of_good(good, pindex);
		if(first) {
			good->utility += utility;
			good->util_count++;
		}
		double value = (double)good->price;
		if(utility/value > utility_per_money) {
			*margin_of_utility = (int)((utility / prev_utility) * prev_value - value);
			utility_per_money = utility/value;
			most_coveted_good = good;
			
			prev_utility = utility;
			prev_value = value;
		}
	}
	if(utility_per_money == 0) {
		printf("ZERO utility\n");
		
		for(i = 0; i < goodsIndex->goods_by_id->size; i++) {
			Good * good = (Good *)svget(goodsIndex->goods_by_id, i);
			
			if(!good)
				continue;
			double utility = (double)this->get_utility_of_good(good, pindex);
			printf("%g utility for %s\n", utility, good->name);
			if(first) {
				good->utility += utility;
				good->util_count++;
			}
			//printf("%s for %g utility\n", good->name,utility);
			double value = (double)good->price;
			if(utility/value > utility_per_money) {
				*margin_of_utility = (int)((utility / prev_utility) * prev_value - value);
				utility_per_money = utility/value;
				most_coveted_good = good;
				
				prev_utility = utility;
				prev_value = value;
			}
		}
		
		exit(0);
	}
	if(utility_per_money < 0) {
		printf("NEGATIVE utility\n");
		exit(0);
	}
	return most_coveted_good;
}

int utility_ready = 0;
int utility_not = 0;
int utility_wrong = 0;

double Citizen::get_utility_of_good(Good * good, ProfessionsIndex * pindex) {
	
	int already_possessed = (int)svget(this->possessions_after_trading, good->id); // P3 already possessed
	
	utility_cache * u = (utility_cache *)svget(this->utilities,good->id);
	if(u) {
		if(already_possessed == u->possessed) {
			return u->utility;
		}
	} else {
		u = (utility_cache *)malloc(sizeof(utility_cache));
		svset(this->utilities,good->id,u);
	}
	
	int quality = 100; // P1 quality
	int affinity = (int)svget(this->affinities, good->id); // P2 affinity
	//int already_possessed = (int)svget(this->possessions_after_trading, good->id); // P3 already possessed
	int producing_skill = (int)svget(this->skills, good->made_by_profession_id); // P4 producing skill
	int utilizing_skill = this->get_utilizing_skill_score(good, pindex); // P5 utilizing skill
	int age = this->birthdate; // P6 age
	
	double utility = good->get_utility(quality, affinity, already_possessed, producing_skill, utilizing_skill, age);
	u->utility = utility;
	u->possessed = already_possessed;
	
	
	if(utility < 0) {
		printf("good:%d, quality:%d, affinity:%d, already_possessed:%d, producing_skill:%d, utilizing_skill:%d, age:%d\n",
		good->id, quality, affinity, already_possessed, producing_skill, utilizing_skill, age);
	
		printf("%g utility for %s\n", utility, good->name);
		exit(0);
	}
	
	return utility;
}

int Citizen::get_quantity_producable(Recipe * recipe, GoodsIndex * index, int realistic) {
	int skill = 0;
	int beneficial_tools = 0; 
	
	Good * good = (Good *)svget(index->goods_by_id, recipe->good_id);
	
	// find skill in producing profession
	skill = (int)svget(this->skills, good->made_by_profession_id);
	// find score for beneficial tools possessed
	beneficial_tools = this->get_beneficial_tool_score(recipe, index);
	
	int production = recipe->get_production(skill, beneficial_tools, index);
	
	if(realistic && production) {
		unsigned int good_id;
		smallVector * required_materials = recipe->get_required_materials_vector(index);
		smallVector * required_tools = recipe->get_required_tools_vector(index);
		
		for(good_id = 0; good_id < required_tools->size && production; good_id++) {
			int quantity_needed = (int)svget(required_tools, good_id);
			if(!quantity_needed) continue;
			
			int quantity_possessed = (int)svget(this->possessions, good_id);
			if(quantity_possessed < quantity_needed) {
				production = 0;
			}
		}
		
		for(good_id = 0; good_id < required_materials->size && production; good_id++) {
			int quantity_needed = (int)svget(required_materials, good_id);
			if(!quantity_needed) continue;
			
			int quantity_possessed = (int)svget(this->possessions, good_id);
			if(production > quantity_possessed / quantity_needed) {
				production = quantity_possessed / quantity_needed;
			}
		}
	}
	return production;
}

int Citizen::get_num_to_sell(Good * good, int max, double utility_of_money, GoodsIndex * goodsIndex, ProfessionsIndex * pindex, int * minimum_price) {
	int num_to_sell = 0;
	int i = 0;
	int value_of_good = good->price;
	
	int quality = 100; // P1 quality
	int affinity = (int)svget(this->affinities, good->id); // P2 affinity
	int already_possessed = (int)svget(this->possessions, good->id); // P3 already possessed
	int producing_skill = (int)svget(this->skills, good->made_by_profession_id); // P4 producing skill
	int utilizing_skill = this->get_utilizing_skill_score(good, pindex); // P5 utilizing skill
	int age = this->birthdate; // P6 age
	for(i = 0; i <= max; i++) {
		int after_sale = already_possessed - i;
		double utility = good->get_utility(quality,affinity,after_sale,producing_skill,utilizing_skill,age);
		double utility_per_value = (utility)/((double)value_of_good);
		if(utility_per_value < utility_of_money) {
			num_to_sell = i;
			*minimum_price = (int)(utility / utility_of_money) + 1;
		} else {
			break;
		}
	}
	return num_to_sell;
}

int Citizen::get_beneficial_tool_score(Recipe * recipe, GoodsIndex * index) {
	int score = 0;
	int total_tools = 0;
	unsigned int good_id;
	smallVector * tools = recipe->get_beneficial_tools_vector(index);
	
	for(good_id = 0; good_id < tools->size; good_id++) {
		int quantity = (int)svget(tools, good_id);
		if(!quantity) continue;
		
		int possessed = (int)svget(this->possessions, good_id);
		if(possessed) {
			score++;
		}
		total_tools++;
	}
	
	if(total_tools == 0) return 0;
	return (score * 100) / total_tools;
}

int Citizen::get_utilizing_skill_score(Good * good,  ProfessionsIndex * pindex) {
	unsigned int i = 0;
	int score = 0;
	char * professions = good->used_by_profession;
	while(i < strlen(professions)) {
		char * strptr = strchr(&professions[i],',');
		int temp = strptr ? strptr - professions : strlen(professions);
		char profession[64];
		
		memcpy(profession, &professions[i], (temp - i));
		profession[temp-i] = 0;
		i = temp+1;
		
		int profession_id = ((Profession *)hmget(pindex->professions_by_name,profession))->id;
		int skill = (int)svget(this->skills,profession_id);
		if(skill > score)
			score = skill;
	}
	return score;
}

int Citizen::get_cost_of_unowned_tools(Recipe * recipe, GoodsIndex * goodsIndex) {
	int cost = 0;
	unsigned int good_id = 0;
	smallVector * required_tools = recipe->get_required_tools_vector(goodsIndex);
	for(good_id = 0; good_id < required_tools->size; good_id++) {
		int quantity = (int)svget(required_tools, good_id);
		if(!quantity) continue;
		
		int possessed = (int)svget(this->possessions, good_id);
		int need = quantity - possessed;
		if(need > 0) {
			Good * good = (Good *)svget(goodsIndex->goods_by_id, good_id);
			cost += (good->price * need) / good->durability;
		}
	
	}
	return cost;
}












