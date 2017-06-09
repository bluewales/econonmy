#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "smallVector.h"
#include "goods.h"
#include "Good.h"
#include "rvpParser.h"

Good::Good(int id, char * name, char * made_by_profession, int made_by_profession_id,
			char * function_for_affinity, char * function_for_utility, int durability,
			char * function_for_potential, char * used_by_profession, char * good_class) {

	this->id = id;
	
	this->name = (char *)malloc(strlen(name) + 1);
	strcpy(this->name, name);
	
	this->made_by_profession = (char *)malloc(strlen(made_by_profession) + 1);
	strcpy(this->made_by_profession, made_by_profession);
	
	this->made_by_profession_id = made_by_profession_id;
	
	this->function_for_affinity = (char *)malloc(strlen(function_for_affinity) + 1);
	strcpy(this->function_for_affinity, function_for_affinity);
	
	this->function_for_utility = (char *)malloc(strlen(function_for_utility) + 1);
	strcpy(this->function_for_utility, function_for_utility);
	
	this->durability = durability;
	
	this->function_for_potential = (char *)malloc(strlen(function_for_potential) + 1);
	strcpy(this->function_for_potential, function_for_potential);
	
	this->used_by_profession = (char *)malloc(strlen(used_by_profession) + 1);
	strcpy(this->used_by_profession, used_by_profession);
	
	this->good_class = (char *)malloc(strlen(good_class) + 1);
	strcpy(this->good_class, good_class);
	
	this->utility = 0;
	this->util_count = 0;
}

Good::~Good() {
	free(this->name);
	free(this->made_by_profession);
	free(this->function_for_affinity);
	free(this->function_for_utility);
	free(this->function_for_potential);
	free(this->used_by_profession);
	free(this->good_class);
}



double Good::get_utility(int quality, int affinity, int already_possessed, int producing_skill, int utilizing_skill, int age) {
	//printf("calculating utility for\n", good->name);
	//printf("%s\n",good->function_for_utility);
	int P[8];
	P[1] = quality;
	P[2] = affinity;
	P[3] = already_possessed;
	P[4] = producing_skill;
	P[5] = utilizing_skill;
	P[6] = age;
	P[7] = this->potential;
	double utility = rvp_parse(this->function_for_utility, P);
	return utility;
}

