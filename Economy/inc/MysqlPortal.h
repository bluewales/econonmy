#ifndef _MysqlPortal_h_included_
#define _MysqlPortal_h_included_

#include <iostream>
#include <stdio.h>
#include <mysql.h>
#define MYSQL_PACKET_SIZE_LIMIT 1024 * 1024

class MysqlPortal
{
public:
	//constructor
	MysqlPortal();
	//destructor
	~MysqlPortal();
	
	MYSQL_RES * submitQuery(char * query);
	int affected_rows();
	int openIMTable(char * table, char * im_table);
	int closeIMTable( char * im_table);
	
	int long_query_begin(char * base);
	int long_query_update(char * update);
	int long_query_finish();
  
private:
	MYSQL *connection, mysql;
	char * database, * user, * password;
	
	//long query data
	char * long_query_base;
	char * long_query;
	int long_query_length;
	int long_query_buffer_length;
	int long_query_send_count;
	int long_query_base_length;
	
};
#endif
