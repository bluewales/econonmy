#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "goods.h"
#include "professions.h"
#include "rvpParser.h"
#include "Market.h"

typedef struct {
	char key[128];
	char value[128];
	char good_name[64];
	MysqlPortal * portal;
} pathContext;

typedef struct {
	char names[512];
	char values[512];
} recipe;

typedef struct {

	int in_recipe;
	int in_recipes;
	
	int recipe_count;
	recipe recipes[GOODS_MAX_RECIPES];
	
	char names[512];
	char values[512];
	MysqlPortal * portal;
} goodContext;

static int good_start_map_callback(void *ctx){
	goodContext * context = (goodContext *)ctx;
	if(context->in_recipes) {
		context->in_recipe = 1;
		context->recipes[context->recipe_count].names[0] = 0;
		context->recipes[context->recipe_count].values[0] = 0;
	}
	
	return 1;
}
static int good_end_map_callback(void *ctx){
	goodContext * context = (goodContext *)ctx;
	if(context->in_recipes) {
		context->in_recipe = 0;
		context->recipe_count++;
	}
	return 1;
}

static int good_end_array_callback(void *ctx) {
	goodContext * context = (goodContext *)ctx;
	if(context->in_recipes) {
		context->in_recipes = 0;
	}
	return 1;
}

static int good_string_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen) {
	goodContext * context = (goodContext *)ctx;
	char string[128];
	string[stringLen] = 0;
	memcpy(string, stringVal,stringLen);
	
		char * values;
	if(context->in_recipe) {
		values = context->recipes[context->recipe_count].values;
	} else {
		values = context->values;
	}
	if(values[0])
		strcat(values, ",");
	strcat(values, "'");
	strcat(values, string);
	strcat(values, "'");
	
    return 1;
} // good_string_callback

static int good_number_callback(void *ctx, const char * stringVal, unsigned int stringLen) {
	goodContext * context = (goodContext *)ctx;
	char string[128];
	string[stringLen] = 0;
	memcpy(string, stringVal,stringLen);
	char * values;
	if(context->in_recipe) {
		values = context->recipes[context->recipe_count].values;
	} else {
		values = context->values;
	}
	if(values[0])
		strcat(values, ",");
	strcat(values, string);
	
    return 1;
} // good_number_callback

static int good_map_key_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen) {
	goodContext * context = (goodContext *)ctx;
	char string[128];
	string[stringLen] = 0;
	memcpy(string, stringVal, stringLen);
	if(strcmp(string,"recipes") == 0) {
		context->in_recipes = 1;
		context->recipe_count = 0;
	} else {
		char * names;
		if(context->in_recipe) {
			names = context->recipes[context->recipe_count].names;
		} else {
			names = context->names;
		}
		if(names[0])
			strcat(names, ",");
		strcat(names, string);
	}
    return 1;
} // good_map_key_callback

static int null_callback(void *ctx){return 1;}
static int boolean_callback(void * ctx, int boolVal){return 1;}
static int integer_callback(void *ctx, long long integerVal){return 1;}
static int double_callback(void *ctx, double doubleVal){return 1;}
static int start_map_callback(void *ctx){return 1;}
static int end_map_callback(void *ctx){return 1;}
static int start_array_callback(void *ctx){return 1;}
static int end_array_callback(void *ctx){return 1;}

static int string_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen) {
	pathContext * pctx = (pathContext *)ctx;
	pctx->value[stringLen] = 0;
	memcpy(pctx->value, stringVal,stringLen);
    if(pctx->key[0] != 0) {
		FILE *fr;
		static yajl_callbacks callbacks = {
			null_callback,
			boolean_callback,
			integer_callback,
			double_callback,
			good_number_callback,
			good_string_callback,
			good_start_map_callback,
			good_map_key_callback,
			good_end_map_callback,
			start_array_callback,
			good_end_array_callback
		};
		goodContext context;
		
		context.in_recipe = 0;
		context.in_recipes = 0;
		context.recipe_count = 0;
		context.names[0] = context.values[0] = 0;
		context.portal = pctx->portal;
		
		char fullPath[128];
		strcpy(fullPath,"dat/goods/");
		strcat(fullPath, pctx->value);
		fr = fopen (fullPath, "rt");  /* open the file for reading */
		
		if(!fr) {
			printf("ERROR:  file %s not found, cannot open\n", fullPath);
		} else {
			myYajlParse(fr, callbacks, (void *)&context);
			fclose(fr);
		
			int i;
			char query[2048];

			
			sprintf(query, "INSERT INTO goods ( %s ) VALUES ( %s )", context.names, context.values);
			pctx->portal->submitQuery(query);
			
			for(i = 0; i < context.recipe_count; i++) {
				sprintf(query, "INSERT INTO recipes (good_id,%s) select id,%s from goods where name='%s'",
				context.recipes[i].names, context.recipes[i].values, pctx->good_name);
				pctx->portal->submitQuery(query);
			}
		}
	}
	pctx->key[0] = pctx->value[0] = 0;
    return 1;
} // string_callback

static int map_key_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen) {
	pathContext * pctx = (pathContext *)ctx;
	pctx->key[stringLen] = 0;
	memcpy(pctx->key, stringVal, stringLen);
	
	if(strcmp(pctx->key, "path") != 0) {
		strcpy(pctx->good_name, pctx->key);
		pctx->key[0] = 0;
	}
    return 1;
} // map_key_callback


void load_goods(MysqlPortal * portal) {
	FILE *fr;
	
	static yajl_callbacks callbacks = {
		null_callback,
		boolean_callback,
		integer_callback,
		double_callback,
		NULL,
		string_callback,
		start_map_callback,
		map_key_callback,
		end_map_callback,
		start_array_callback,
		end_array_callback
	};
	pathContext pctx;
	pctx.key[0] = pctx.value[0] = pctx.good_name[0] = 0;
	pctx.portal = portal;
	
	char * goods_file_name = (char *)"dat/goods.json";
	
	fr = fopen (goods_file_name, "rt");
	if(!fr) {
		printf("ERROR:  file %s not found, cannot open\n", goods_file_name);
	} else {
		myYajlParse(fr, callbacks, (void *)&pctx);
		fclose(fr);
	}
} // load_goods

void purge_goods_table(MysqlPortal * portal) {
	// clear goods table
	portal->submitQuery((char*)"drop table goods");
	// recreate goods table
	char query[1024];
	sprintf(query,(char*)"CREATE TABLE goods (\
				id int NOT NULL AUTO_INCREMENT,\
				name varchar(50) NOT NULL,\
				made_by_profession varchar(50) NOT NULL,\
				made_by_profession_id int,\
				function_for_affinity varchar(100) NOT NULL DEFAULT '(0)=',\
				function_for_utility varchar(100) NOT NULL,\
				durability int NOT NULL DEFAULT 100,\
				function_for_potential varchar(100) NOT NULL DEFAULT '(50)=',\
				used_by_profession varchar(50) NOT NULL,\
				class varchar(50) NOT NULL,\
				PRIMARY KEY (id,name))");
	portal->submitQuery(query); 
}


int good_exists(MysqlPortal * portal, char * string) {
	char query[100];
	sprintf(query, "SELECT name FROM goods WHERE name='%s'", string);
	
	MYSQL_RES *result = portal->submitQuery(query);
	int exists = !!mysql_fetch_row(result);
	mysql_free_result(result);
	return exists;
}

int verify_list_of_professions(MysqlPortal * portal, char * string) {
	unsigned int i = 0;
	while(i < strlen(string)) {
		char * strptr = strchr(&string[i],',');
		int temp = strptr ? strptr - string : strlen(string);
		char profession[30];
		
		memcpy(profession, &string[i], (temp - i));
		profession[temp-i] = 0;
		if(!profession_exists(portal, profession))
			return 0;
		i = temp+1;
	}
	return 1;
} // verify_list_of_goods

int verify_list_of_goods(MysqlPortal * portal, char * string) {
	while(string && string[0]) {
		int n = 1;
		char good_name[30];
		char * strptr = strchr(string,',');
		char * amount = strchr(string,':');
		int name_length = strptr ? strptr - string : strlen(string);
		if(amount && (!strptr || amount < strptr)) {
			name_length = amount - string;
			n = atoi(amount+1);
		}
		
		memcpy(good_name, string, (name_length));
		good_name[name_length] = 0;
		if(strptr) strptr++;
		string = strptr;
		
		
		if(!good_exists(portal, good_name))
			return 0;
	}
	return 1;
} // verify_list_of_goods

int verify_required_materials(MysqlPortal * portal, char * string) {
	return verify_list_of_goods(portal, string);
}

int verify_required_tools(MysqlPortal * portal, char * string) {
	return verify_list_of_goods(portal, string);
}

int verify_beneficial_tools(MysqlPortal * portal, char * string) {
	return verify_list_of_goods(portal, string);
}

int verify_made_by_profession(MysqlPortal * portal, char * string, int good_id) {
	if(!verify_list_of_professions(portal,string))
		return 0;
	char query[1024];
	sprintf(query, "SELECT id,name FROM professions WHERE name='%s'", string);	
	MYSQL_RES *result = portal->submitQuery(query);
	MYSQL_ROW row = mysql_fetch_row(result);

	if(row) {
		sprintf(query, "UPDATE goods SET made_by_profession_id=%s WHERE id=%d", row[0], good_id);
	}
	mysql_free_result(result);
	//printf("%s\n", query);
	portal->submitQuery(query);
	return !!row;
}

int verify_function_for_affinity(MysqlPortal * portal, char * string) {
	int i;
	int min = 0;
	int max = 100;
	for(i = 0; i < 50; i++) {
		double result = rvp_parse(string, NULL);
		if(result < min || result > max)
			return 0;
	}
	return 1;
} // verify_function_for_affinity

int verify_function_for_utility(MysqlPortal * portal, char * string) {
	int min = 0;
	int A = 2;
	int P1[] = {1,100};  // quality
	int B = 6;
	int P2[] = {0,1,30,55,80,100};  // affinity
	int C = 7;
	int P3[] = {0,1,10,100,1000,10000,100000};  // already possessed
	int D = 5;
	int P4[] = {0,1,4,7,10};  // producing skill
	int E = 5;
	int P5[] = {0,1,5,7,10};  // utilizing skill
	int F = 4;
	int P6[] = {25,50,75,100}; // age
	int G = 5;
	int P7[] = {0,25,50,75,100}; // local potential

	int P[8];
	int a, b, c, d, e, f, g;
	for(a = 0; a < A; a++) {
		P[1] = P1[a];
		for(b = 0; b < B; b++) {
			P[2] = P2[b];
			for(c = 0; c < C; c++) {
				P[3] = P3[c];
				for(d = 0; d < D; d++) {
					P[4] = P4[d];
					for(e = 0; e < E; e++) {
						P[5] = P5[e];
						for(f = 0; f < F; f++) {
							P[6] = P6[f];
							for(g = 0; g < G; g++) {
								P[7] = P7[g];
								double result = rvp_parse(string, P);
								if(result < min)
									return 0;
							}
						}
					}
				}
			}
		}
	}
	return 1;
} // verify_function_for_utility

int verify_durability(MysqlPortal * portal, char * string) {
	if(atoi(string) >= 0)
		return 1;
	else
		return 0;
} // verify_durability

int verify_function_for_production(MysqlPortal * portal, char * string) {
	int min = 0;
	int a = 5;
	int P1[] = {0,1,4,8,10};  // skill for production
	int b = 8;
	int P2[] = {0,1,2,3,4,5,7,10};  // beneficial tools score
	int c = 5;
	int P3[] = {0,25,50,75,100};  // local potential
	for(a = 4; a >= 0; a--) {
		for(b = 7; b >= 0; b--) {
			for(c = 4; c >= 0; c--) {
				int P[4];
				P[1] = P1[a];
				P[2] = P2[b];
				P[3] = P3[c];
				double result = rvp_parse(string, P);
				if(result < min){
					printf("\n%f %s\n", result, string);
					exit(0);
					return 0;
				}
			}
		}
	}
	
	return 1;
}

int verify_function_for_quality(MysqlPortal * portal, char * string) {
	// There are no peramiters for quality yet
	int min = 0;
	int max = 100;
	
	double result = rvp_parse(string, 0);
	if(result < min || result > max) {
		return 0;
	}
	
	return 1;
}

int verify_function_for_potential(MysqlPortal * portal, char * string) {
	int min = 0;
	int max = 100;
	int A = 5;
	int P1[] = {0,25,50,75,100};  // humidity score
	int B = 5;
	int P2[] = {0,25,50,75,100};  // mountain score
	int C = 1;
	int P3[] = {0,1,2,3,4,5,6,7,8,9,10};  // cultural biome code

	int P[7];
	int a, b, c;
	for(a = 0; a < A; a++) {
		P[1] = P1[a];
		for(b = 0; b < B; b++) {
			P[2] = P2[b];
			for(c = 0; c < C; c++) {
				P[3] = P3[c];
				double result = rvp_parse(string, P);
				if(result < min || result > max)
					return 0;
			}
		}
	}
	return 1;
} // verify_function_for_potential

int verify_used_by_profession(MysqlPortal * portal, char * string, int good_id) {
	if(!verify_list_of_professions(portal,string))
		return 0;
	return 1;
}

int verify_goods(MysqlPortal * portal) {
	MYSQL_ROW row;
	MYSQL_RES *result; 
	unsigned int num_fields;

	result = portal->submitQuery((char*)"SELECT id,name,\
made_by_profession,function_for_affinity,\
function_for_utility,durability,function_for_potential,\
used_by_profession FROM goods order by name"); 
	num_fields = mysql_num_fields(result);
	
	while ((row = mysql_fetch_row(result))) {
		printf("Testing %s\r", row[1]);
		fflush(stdout);
		
		
		// row[0] is id, row[1] is name, nothing to verify there
		if(row[2] && row[2][0] && !verify_made_by_profession(portal, row[2], atoi(row[0])))
			printf("\nFailed. Invalid 'made_by_profession': '%s' in entry with name: '%s'\n", row[2], row[1]);
			
		if(row[3] && row[3][0] && !verify_function_for_affinity(portal, row[3]))
			printf("\nFailed. Invalid 'function_for_affinity': '%s' in entry with name: '%s'\n", row[3], row[1]);
			
		if(row[4] && row[4][0] && !verify_function_for_utility(portal, row[4]))
			printf("\nFailed. Invalid 'function_for_utility': '%s' in entry with name: '%s'\n", row[4], row[1]);
			
		if(row[5] && row[5][0] && !verify_durability(portal, row[5]))
			printf("\nFailed. Invalid 'durability': '%s' in entry with name: '%s'\n", row[5], row[1]);
			
		if(row[6] && row[6][0] && !verify_function_for_potential(portal, row[6]))
			printf("\nFailed. Invalid 'verify_function_for_potential': '%s' in entry with name: '%s'\n", row[7], row[1]);
			
		if(row[7] && row[7][0] && !verify_used_by_profession(portal, row[7], atoi(row[0])))
			printf("\nFailed. Invalid 'used_by_profession': '%s' in entry with name: '%s'\n", row[7], row[1]);
		
		printf("\r                                           \r");
    }
	mysql_free_result(result);
	return 1;
}
