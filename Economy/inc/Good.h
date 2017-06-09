#ifndef _good_h_
#define _good_h_


#define ONE_ROUND_FOOD 5


class Good {
	public:
	int id;
	char * name;
	char * made_by_profession;
	int made_by_profession_id;
	char * function_for_affinity;
	char * function_for_utility;
	int durability;
	char * function_for_potential;
	char * used_by_profession;
	char * good_class;
	int price;
	int momentum;
	double utility;
	double potential;
	int util_count;
	
	Good(int id, char * name, char * made_by_profession, int made_by_profession_id,
			char * function_for_affinity, char * function_for_utility, int durability,
			char * function_for_potential, char * used_by_profession, char * good_class);
	~Good();
	
	double get_utility(int quality, int affinity, int already_possessed,
						int producing_skill, int utilizing_skill, int age);
};

#endif
