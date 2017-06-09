#ifndef _profession_h_
#define _profession_h_

#include "hashMap.h"
#include "smallVector.h"
#include "MysqlPortal.h"

class Profession{
	public:

	int id;
	char * name;
	
	Profession(MYSQL_ROW row);
	~Profession();
	
};


#endif
