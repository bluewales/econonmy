#ifndef _goods_h_
#define _goods_h_

#define GOODS_MAX_RECIPES			10

void purge_goods_table(MysqlPortal * portal);
void load_goods(MysqlPortal * portal);
int verify_goods(MysqlPortal * portal);
int good_exists(MysqlPortal * portal, char * string);
int current_value_of_good(MysqlPortal * portal, char * string);

#endif