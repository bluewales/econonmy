#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MysqlPortal.h"
#include "myYajlParser.h"
#include "rvpParser.h"
#include "professions.h"

typedef struct {
	char key[128];
	int inProfessions;
	MysqlPortal * portal;
} context;

static int null_callback(void *ctx){return 1;}
static int boolean_callback(void * ctx, int boolVal){return 1;}
static int integer_callback(void *ctx, long long integerVal){return 1;}
static int double_callback(void *ctx, double doubleVal){return 1;}
static int start_map_callback(void *ctx){return 1;}
static int end_map_callback(void *ctx){return 1;}

static int start_array_callback(void *ctx)
{
	context * lctx = (context *)ctx;
	if(strcmp(lctx->key,"professions") == 0)
	{
		lctx->inProfessions = 1;
	}
	return 1;
}
static int end_array_callback(void *ctx)
{
	context * lctx = (context *)ctx;
	lctx->inProfessions = 0;
	return 1;
}

static int string_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen)
{	
	context * lctx = (context *)ctx;
	if(lctx->inProfessions)
	{
		char profession[30];
		memcpy(profession, stringVal, stringLen);
		profession[stringLen] = 0;
		char query[512];
		sprintf(query,"INSERT INTO professions ( name ) VALUES ( '%s' )", profession);
		lctx->portal->submitQuery(query);
	}
	
    return 1;
} // string_callback
static int map_key_callback(void *ctx, const unsigned char * stringVal, unsigned int stringLen)
{
	context * lctx = (context *)ctx;
	lctx->key[stringLen] = 0;
	memcpy(lctx->key, stringVal,stringLen);
    return 1;
} // map_key_callback

void purge_professions_table(MysqlPortal * portal)
{
	// clear professions_meta_data table
	portal->submitQuery((char*)"drop table professions");
	// recreate professions_meta_data table
	portal->submitQuery((char*)"CREATE TABLE professions ( \
							id int NOT NULL AUTO_INCREMENT, \
							name varchar(50) NOT NULL, \
							PRIMARY KEY (id,name))"); 
}

void load_professions(MysqlPortal * portal)
{
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
	context ctx;
	ctx.key[0] = ctx.inProfessions = 0;
	ctx.portal = portal;
	
	fr = fopen ((char *)"dat/professions.json", "rt");
	myYajlParse(fr, callbacks, (void *)&ctx);
	fclose(fr);
}
int verify_professions(MysqlPortal * portal)
{
	return 1;
}
int profession_exists(MysqlPortal * portal, char * string)
{
	//printf("Checking that %s exists\n", string);
	char query[100];
	sprintf(query, "SELECT name FROM professions WHERE name='%s'", string);
	
	MYSQL_RES *result = portal->submitQuery(query);
	int exists = !!mysql_fetch_row(result);
	mysql_free_result(result);
	return exists;
}
