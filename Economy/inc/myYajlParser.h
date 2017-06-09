#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>
#include <stdio.h>

/*  Example of a yajl_callbacks;

static yajl_callbacks callbacks = {
    test_yajl_null,
    test_yajl_boolean,
    test_yajl_integer,
    test_yajl_double,
    NULL,
    test_yajl_string,
    test_yajl_start_map,
    test_yajl_map_key,
    test_yajl_end_map,
    test_yajl_start_array,
    test_yajl_end_array
};

*/

int myYajlParse(FILE * file, yajl_callbacks callbacks, void * ctx);
