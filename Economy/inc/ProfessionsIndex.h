#ifndef _professionsindex_h_
#define _professionsindex_h_

class MysqlPortal;

#include "hashMap.h"
#include "smallVector.h"


class ProfessionsIndex {
	public:
	smallVector * professions_by_id;
	hashMap * professions_by_name;
	
	ProfessionsIndex(MysqlPortal * portal);
	~ProfessionsIndex();
	
	
	void from_professions_list_to_vector(char * list, smallVector * vector);

};


#endif
