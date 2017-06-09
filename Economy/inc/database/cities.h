#ifndef cities_h_
#define cities_h_

void purge_cities_table(MysqlPortal * portal);
void createCity(MysqlPortal * portal, char * name, int plains, int mountain, int biome, int population);


#endif
