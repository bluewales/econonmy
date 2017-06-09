#include "stdlib.h"
#include "string.h"
#include <MysqlPortal.h>



MysqlPortal::MysqlPortal(void) {
	database = (char*)"economy";
	user = (char*)"econouser";
	password = (char*)"econopass";
	
	mysql_init(&mysql);
	//connection = mysql_real_connect(&mysql,"host","user","password","database",0,0,0);
	connection = mysql_real_connect(&mysql,"localhost",user,password,database,0,0,0);
	if (connection == NULL) {
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
	}
	this->long_query_base = 0;
	this->long_query = 0;
	this->long_query_send_count = 0;
	this->long_query_buffer_length = 0;
	this->long_query_base_length = 0;
}

MysqlPortal::~MysqlPortal() {
	mysql_close(connection);
	mysql_library_end();
	if(this->long_query_base) {
		free(this->long_query_base);
	}
	if(this->long_query) {
		free(this->long_query);
	}
}

MYSQL_RES * MysqlPortal::submitQuery(char * query) {
	if (mysql_query(&mysql, query))  {
		printf("%s\n",query);
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
		//exit(0);
	}
	return mysql_store_result(connection);
}

int MysqlPortal::affected_rows() {
    return mysql_affected_rows(&mysql);
}

int MysqlPortal::openIMTable(char * table, char * im_table) {
	char query[1024];
	int result = 0;
	
	sprintf(query, "CREATE TABLE %s LIKE %s", im_table, table);
	if ((result = mysql_query(&mysql, query)))  {
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
		return result;
	}
	
	sprintf(query, "ALTER TABLE %s ENGINE=MEMORY", im_table);
	if ((result = mysql_query(&mysql, query))) {
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
		return result;
	}
	
	sprintf(query,  "INSERT %s SELECT * FROM %s", im_table, table);
	if ((result = mysql_query(&mysql, query))) {
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
		return result;
	}return result;
}

int MysqlPortal::closeIMTable( char * im_table) {
	int result = 0;
	char query[1024];
	sprintf(query, "DROP TABLES %s", im_table);
	if ((result = mysql_query(&mysql, query)))
	{
		fprintf(stderr, "MYSQL ERROR : %s\n", mysql_error(&mysql));
		return result;
	}return result;
}

int MysqlPortal::long_query_begin(char * base) {
	if(!this->long_query_base) {
		this->long_query_base = (char*)malloc(1024);
		this->long_query_base_length = 1024;
	}
	if(!this->long_query) {
		this->long_query = (char*)malloc(1024);
		this->long_query_buffer_length = 1024;
	}
	
	this->long_query_send_count = 0;
	
	strcpy(this->long_query_base,base);
	strcpy(this->long_query,base);
	this->long_query_length = strlen(this->long_query);
	return this->long_query_send_count;
}

int MysqlPortal::long_query_update(char * update) {
	int additional_length = strlen(update);
	
	if(this->long_query_length + additional_length + 2 >= MYSQL_PACKET_SIZE_LIMIT) {
		this->long_query[this->long_query_length - 1] = 0;
		this->submitQuery(this->long_query);
		strcpy(this->long_query, this->long_query_base);
		this->long_query_length = strlen(this->long_query);
		return this->long_query_send_count += 1;
	} else if(this->long_query_length  + additional_length + 2 >= this->long_query_buffer_length) {
		
		this->long_query_buffer_length *= 2;
		this->long_query = (char*)realloc(this->long_query,this->long_query_buffer_length);
	}
	
	strcat(&this->long_query[this->long_query_length], update);
	this->long_query_length += additional_length;
	strcat(&this->long_query[this->long_query_length], ",");
	this->long_query_length += 1;
	
	return this->long_query_send_count;
}

int MysqlPortal::long_query_finish() {
	if((unsigned int)this->long_query_length > strlen(this->long_query_base)) {
		this->long_query[this->long_query_length - 1] = 0;
		this->submitQuery(this->long_query);
		this->long_query_length = 0;
		return this->long_query_send_count += 1;
	}
	this->long_query[0] = 0;
	return this->long_query_send_count;
}

