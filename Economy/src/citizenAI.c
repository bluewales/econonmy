#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>

#include "MysqlPortal.h"
#include "citizenAI.h"
#include "rvpParser.h"
#include "smallVector.h"
#include "hashMap.h"
#include "Good.h"
#include "Profession.h"
#include "Citizen.h"
#include "Market.h"
#include "City.h"
#include "CitizensIndex.h"
#include "GoodsIndex.h"
#include "myutil.h"
#include "Recipe.h"
#include "RecipesIndex.h"

char story_line[1024];
void story(char * line, int citizen_id) {
	if(citizen_id % 500) return;
	char filename[1024];
	sprintf(filename, "story%d.txt", citizen_id);
	FILE * fd = fopen(filename, "a");
	fprintf(fd, "%s\n", line);
	//printf("Story:%s\n",line);
	fclose(fd);
}

int get_profit(Recipe * recipe, Citizen * citizen, GoodsIndex * goodsIndex, int realistic) {
	Good * good = (Good *)svget(goodsIndex->goods_by_id, recipe->good_id);
	
	int production = citizen->get_quantity_producable(recipe, goodsIndex, realistic);

	int cost = recipe->get_cost_to_produce(goodsIndex);
	int revenue = good->price;
	int profit = revenue - cost;
	
	return production * profit - citizen->get_cost_of_unowned_tools(recipe, goodsIndex);
}

int what_to_produce(GoodsIndex * goodsIndex, RecipesIndex * recipesIndex, Citizen * citizen, int * margin, int realistic) {
	unsigned int i; 
	int max_profit = 0;
	int max_index = -1;
	*margin = -1;
	unsigned int count = recipesIndex->recipes_by_id->size;
	
	// compare each recipe and find the one with the highest profit
	for(i = 1; i < count; i++) {
		// find a recipe's profit
		Recipe * recipe = (Recipe *)svget(recipesIndex->recipes_by_id, i);
		if(!recipe) continue;
		
		int profit = get_profit(recipe, citizen, goodsIndex, realistic);

		if(profit >= max_profit) {
			*margin = profit - max_profit;
			max_profit = profit;
			max_index = i;
		} else if(*margin > max_profit-profit) {	
			*margin = max_profit-profit;
		}
	}
	
	if(max_index <= 0) {
		printf("Could not find a profession for citizen %d (index=%d)\n", citizen->id, max_index);
		exit(0);
	}
	return max_index;
}


int amount_to_be_spent(Citizen * citizen, smallVector * required_goods, GoodsIndex * goodsIndex, Good ** most_expensive_good) {
	unsigned int i;
	int amount_to_spend = 0;
	int highest_value = 0;
	*most_expensive_good = 0;
	
	Good * food = (Good *)hmget(goodsIndex->goods_by_name,(char *)"food");
	int food_id = food?food->id:0;

	for(i = 0; i < required_goods->size; i++) {
		int num_required = (int)svget(required_goods, i);
		if(num_required && i != (unsigned int)food_id) {
			int num_possessed = (int)svget(citizen->possessions,i);
			if(num_required > num_possessed) {
				Good * good = (Good *)svget(goodsIndex->goods_by_id, i);
				num_required -= num_possessed;
				int value = good->price;
				amount_to_spend += num_required * value;
				if(value >= highest_value) {
					*most_expensive_good = good;
					highest_value = value;
				}
			}
		}
	}
	assert(*most_expensive_good);
	return amount_to_spend;
}

void citizen_production(MysqlPortal * portal, City * city) {
	assert("Everything is going to be okay.\n");
	struct timeval timer, fine_timer;
	
	Market * market = city->market;
	GoodsIndex * goodsIndex = city->goodsIndex;
	RecipesIndex * recipesIndex = city->recipesIndex;
	CitizensIndex * citizensIndex = city->citizensIndex;
	ProfessionsIndex * professionsIndex = city->professionsIndex;
	
	smallVector * exists = svcreate(100);
	smallVector * produced = svcreate(100);
	smallVector * consumed = svcreate(100);
	
	int idle_citizens = 0;
	
	start_timer(&timer);
	// for each citizen
	
	int total_money = 0;
	
	unsigned int citizen_id;
	for(citizen_id = 0; citizen_id < citizensIndex->citizens_by_id->size; citizen_id++) {
		Citizen * citizen = (Citizen *) svget(citizensIndex->citizens_by_id,citizen_id);
		if(!citizen) continue;
		
		story((char *)"", citizen_id);
		
		total_money += citizen->money;
		
		
		unsigned int j;
		// check for accounting errors, we can't have negative quantity of any good.
		for(j = 0; j < citizen->possessions->size; j++) {
			int count = (int)svget(citizen->possessions,j);
			if(!count) continue;
			if(count < 0) {
				printf("%d has %d of %d\n", citizen->id,count, j);
				exit(0);
			}
			svset(exists,j,(void*)((int)svget(exists,j) + count));
		}
		
		// we shouldn't have negative money either
		if(citizen->money < 0) {
			printf("%d had %d money\n", citizen->id, citizen->money);
			exit(0);
		}
		
		
		start_timer(&fine_timer);
		smallVector * required_goods = 0;
		//printf("Citizen: %s %s\n", citizen->first_name, citizen->last_name);
		// if the citizen has a product to produce
		int product_recipe_id = 0;
		int product_id = 0;
		int production_margin = 0;
		Recipe * product_recipe = 0;
		Good * product = 0;
		
		if(citizen->has_food_for_procution(goodsIndex)) {
			product_recipe_id = what_to_produce(goodsIndex, recipesIndex, citizen, &production_margin, 1);
			
			product_recipe = (Recipe *)svget(recipesIndex->recipes_by_id, product_recipe_id);
			
			product = (Good *)svget(goodsIndex->goods_by_id, product_recipe->good_id);
			product_id = product->id;
			
			// print citizen 1 data to story.txt
			
			sprintf(story_line,"Day %d: %s %s is a level %d %s and so wants to %s.",
				city->day, citizen->first_name, citizen->last_name,
				(int)svget(citizen->skills, product->made_by_profession_id),
				product->made_by_profession, product_recipe->action);
			story(story_line, citizen_id);
			
			int production_requirements_met = 0;
			required_goods = product_recipe->get_requirements_for_production(goodsIndex);
			
			production_requirements_met = citizen->has_requirements_for_production(required_goods);
			
			
			// 	We have the technology! Produce that good!
			int num_produced = 0;
			if(production_requirements_met) {
				num_produced = citizen->produce(product_recipe, product, goodsIndex);
			} else {
				idle_citizens++;
			}
			
			sprintf(story_line,"%d %s were produced at %d each.", num_produced, product->name, product->price);
			story(story_line, citizen_id);
			
			for(j = 0; j < required_goods->size; j++) {
				int count = (int)svget(required_goods,j);
				if(!count) continue;
				svset(consumed,j,(void*)((int)svget(consumed,j) + count));
			}
			svset(produced,product_id,(void*)((int)svget(produced,product_id) + num_produced));
		} else { // else the citizen scavenges for food
			// Cannot produce, scavenge for food
			
			citizen->scavenge(goodsIndex);
			product_id = 0;
		}
		
		product_recipe_id = what_to_produce(goodsIndex, recipesIndex, citizen, &production_margin, 0);
		product_recipe = (Recipe *)svget(recipesIndex->recipes_by_id, product_recipe_id);
		
		product = (Good *)svget(goodsIndex->goods_by_id, product_recipe->good_id);
		product_id = product->id;
		
		
		sprintf(story_line,"It looks like the money is in %s, with a margin of %d", product->name, production_margin);
		story(story_line, citizen_id);
		
		
		//printf("  %d %s decided to produce %s with margin of %d\n",citizen->id, citizen->first_name, product->name, production_margin);
		
		
		//printf("      produce    % 6ld ms\n", stop_timer(&fine_timer));
		start_timer(&fine_timer);
/*
		Good * most_expensive_required_good;
		// if there are dependency loops in recipes, then the program may get caught here in an infinite loop
		// rather than detect infinite loops, I'd much rather eliminate circular dependencies
		int amount_to_spend = amount_to_be_spent(citizen,required_goods,goodsIndex,&most_expensive_required_good);
		while(citizen->money < amount_to_spend && most_expensive_required_good) {
			production_margin = 0;
			product = most_expensive_required_good;
			product_id = most_expensive_required_good->id;
			svdestroy(required_goods);
			required_goods = get_requirements_for_production_list(product, goodsIndex);
			amount_to_spend = amount_to_be_spent(citizen, required_goods, goodsIndex, &most_expensive_required_good);
			//printf("  but, they couldn't afford to, so they decided to produce %s instead\n", product->name);
		}
*/		
		// the citizen now has a profession, and needs to prepare for the next round.
		// the citizen needs to try to buy the things they need to produce for the next round.
		// first, determine what they need,
		// then they try to buy it, according to what they can afford.
		
		
		sprintf(story_line,"They have %d money.", citizen->money);
		story(story_line, citizen_id);
		
		int num_producible = citizen->get_quantity_producable(product_recipe, goodsIndex, 0);

		required_goods = product_recipe->get_requirements_for_production(goodsIndex);
		
		sprintf(story_line,"as a level %d %s, %s can make %d %s if they %s", (int)svget(citizen->skills, product->made_by_profession_id),product->made_by_profession,citizen->first_name, num_producible, product->name, product_recipe->action);
		story(story_line, citizen_id);
		
		{
			unsigned int j;
			smallVector * required_materials = product_recipe->get_required_materials_vector(goodsIndex);
			smallVector * required_tools = product_recipe->get_required_tools_vector(goodsIndex);
			
			for(j = required_goods->size; j > 0 && citizen->money > 0; j--) {
				int num_required = (int)svget(required_goods, j);
				if (!num_required) continue;
				
				Good * good = (Good *)svget(goodsIndex->goods_by_id,j);
				
				num_required = num_producible * (int)svget(required_materials, j) + (int)svget(required_tools, j);
				int num_possessed = (int)svget(citizen->possessions, j);
				
				sprintf(story_line,"%d %s are required, they have %d",
					num_required, good->name, num_possessed);
				story(story_line, citizen_id);
				
				if(num_possessed < num_required) {
					
					num_required = num_required - num_possessed;
					
					int offer_value = good->price;
					int num_to_buy = num_required;
					
					if (num_to_buy > citizen->money/offer_value) {
						num_to_buy = citizen->money/offer_value;
						if(!num_to_buy) {
							num_to_buy = 1;
							offer_value = citizen->money;
						}
					}
					
					sprintf(story_line,"They need %d %s which costs %d each, so they bid on %d for %d each.",
						num_required, good->name, good->price, num_to_buy, offer_value);
					story(story_line, citizen_id);
					
					market->submit_bid_from_citizen(good, num_to_buy, offer_value, citizen);
				}
			}
		}
		
		//printf("      buy needs    % 6ld ms\n", stop_timer(&fine_timer));
		start_timer(&fine_timer);
		
		int margin_of_utility;
		Good * most_coveted_good = citizen->get_most_coveted_good(goodsIndex, professionsIndex, &margin_of_utility,1);
		
		start_timer(&fine_timer);
		
		int target_money = citizen->money * 9/10;
		
		while(citizen->money > target_money && most_coveted_good) {
			
			int value = most_coveted_good->price;
			int num_to_buy = 1;
			
			if(value > citizen->money) {
				break;
			}
			num_to_buy = 1;
			
			{
				double utility = citizen->get_utility_of_good(most_coveted_good, professionsIndex);
				int possessed = ((int)svget(citizen->possessions_after_trading, most_coveted_good->id)) + 1;
				char * post_numeral = (char *)((possessed%10 == 1)?"st":(possessed%10 == 2)?"nd":(possessed%10 == 3)?"rd":"th");
				sprintf(story_line,"they buy a %d%s %s at %g utility and %d money, valuing money at %g utility", 
				possessed, post_numeral, most_coveted_good->name, utility, value, utility / value);
				story(story_line, citizen_id);
			}
			market->submit_bid_from_citizen(most_coveted_good, num_to_buy, value, citizen);
			
			most_coveted_good = citizen->get_most_coveted_good(goodsIndex, professionsIndex, &margin_of_utility, 0);
		}
		
		double utility_from_most_coveted_good = citizen->get_utility_of_good(most_coveted_good, professionsIndex);
		double utility_of_money = utility_from_most_coveted_good/((double)(most_coveted_good->price));
		
		sprintf(story_line,"%g utility from money, most coveted good: %s", utility_of_money, most_coveted_good->name);
		story(story_line, citizen_id);


		{
			unsigned int j;
			smallVector * required_materials = product_recipe->get_required_materials_vector(goodsIndex);
			smallVector * required_tools = product_recipe->get_required_tools_vector(goodsIndex);


			for(j = 0; j < citizen->possessions->size; j++) {
				int good_id = j;
				int num_possessed = (int)svget(citizen->possessions, good_id);
				if(!num_possessed)
					continue;
				
				int num_required = num_producible * (int)svget(required_materials, j) + (int)svget(required_tools, j);
				int num_can_sell = num_possessed - num_required;
				if(num_can_sell > 0) {
					Good * good = (Good *)svget(goodsIndex->goods_by_id,good_id);
					int minimum_price = good->price-1;
					int num_to_sell = citizen->get_num_to_sell(good, num_can_sell, utility_of_money, goodsIndex, professionsIndex, &minimum_price);
					
					sprintf(story_line,"they sell %d/%d %s, and need to keep %d", num_to_sell, num_possessed, good->name, num_required);
					story(story_line, citizen_id);
					
					if(minimum_price < 1) minimum_price = 1;
					if(num_to_sell > 0) {
						market->submit_offer_from_citizen(good, num_to_sell, minimum_price, citizen);
					}
				}
			}
		}
		//printf("      sell extra    % 6ld ms\n", stop_timer(&fine_timer));

		
		//printf("      buy wants    % 6ld ms\n", stop_timer(&fine_timer));
	}
	
	printf("  produce      % 6ld ms\n", stop_timer(&timer));

	start_timer(&timer);
	
	unsigned int i;
	
	char filename[1024];
	
	sprintf(filename, "/var/www/EveOnTheWater/www/economy/city_%d.json", city->id);
	FILE * fd = fopen(filename,"a");
	
	if(fd) {
		if(city->day == 1) {
			fprintf(fd,"{\"city\":\"%s\"}", city->name);
		}
		fprintf(fd,",{");
		for(i = 1; i <= ((goodsIndex->goods_by_id)->count); i++) {
			Good * good = (Good*)svget(goodsIndex->goods_by_id,i);
			int count = (int)svget(exists,i);
			int num_produced =(int)svget(produced,i);
			int num_consumed =(int)svget(consumed,i);
			if(i > 1)
				fprintf(fd,",");
			fprintf(fd,"\"%s\":{\"price\":%d,\"count\":%d,\"produced\":%d,\"consumed\":%d,\"utility\":%g,\"money\":%d}",
				good->name,good->price,count,num_produced,num_consumed,good->utility/good->util_count, total_money);
		}
		fprintf(fd,"}\n");
		fclose(fd);
	}
	printf("  write stats  % 6ld ms\n", stop_timer(&timer));
	
	svdestroy(exists);
	svdestroy(produced);
	svdestroy(consumed);
	
}
