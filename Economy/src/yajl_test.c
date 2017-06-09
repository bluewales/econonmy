#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

typedef struct {
	char key[128];
	char value[128];
} pathHolder;

static void myYajlFree(void * ctx, void * ptr)
{
    free(ptr);
}

static void * myYajlMalloc(void * ctx, unsigned int sz)
{
    return malloc(sz);
}

static void * myYajlRealloc(void * ctx, void * ptr, unsigned int sz)
{
	return realloc(ptr, sz);
}

/* begin parsing callback routines */
#define BUF_SIZE 1024

static int test_yajl_null(void *ctx)
{
    printf("null\n");
    return 1;
}

static int test_yajl_boolean(void * ctx, int boolVal)
{
    printf("bool: %s\n", boolVal ? "true" : "false");
    return 1;
}

static int test_yajl_integer(void *ctx, long integerVal)
{
    printf("integer: %ld\n", integerVal);
    return 1;
}

static int test_yajl_double(void *ctx, double doubleVal)
{
    printf("double: %g\n", doubleVal);
    return 1;
}

static int test_yajl_string(void *ctx, const unsigned char * stringVal,
                            unsigned int stringLen)
{
	pathHolder * holder = (pathHolder *)ctx;
	holder->value[stringLen] = 0;
	memcpy(holder->value, stringVal,stringLen);
    if(holder->key[0] != 0)
	{
		printf("%s : %s\n", holder->key, holder->value);
	}
	holder->key[0] = 0;
	holder->value[0] = 0;
    return 1;
}

static int test_yajl_map_key(void *ctx, const unsigned char * stringVal,
                             unsigned int stringLen)
{
	pathHolder * holder = (pathHolder *)ctx;
	holder->key[stringLen] = 0;
	memcpy(holder->key, stringVal,stringLen);
	
	if(strcmp(holder->key,"path") != 0)
		holder->key[0] = 0;
    return 1;
}

static int test_yajl_start_map(void *ctx)
{
    //printf("map open '{'\n");
    return 1;
}


static int test_yajl_end_map(void *ctx)
{
    //printf("map close '}'\n");
    return 1;
}

static int test_yajl_start_array(void *ctx)
{
    //printf("array open '['\n");
    return 1;
}

static int test_yajl_end_array(void *ctx)
{
    //printf("array close ']'\n");
    return 1;
}

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

static void usage(const char * progname)
{
    fprintf(stderr,
            "usage:  %s [options] <filename>\n"
            "   -c  allow comments\n"
            "   -b  set the read buffer size\n",
            progname);
    exit(1);
}


int main(int argc, char ** argv)
{
    yajl_handle hand;
    unsigned char fileData[BUF_SIZE];
    yajl_status stat;
    size_t rd;
    yajl_parser_config cfg = { 1, 0 };
	pathHolder holder;
    int done;
	holder.key[0] = 0;
	holder.value[0] = 0;

    yajl_alloc_funcs allocFuncs = {
        myYajlMalloc,
        myYajlRealloc,
        myYajlFree,
        (void *) NULL
    };

    if (fileData == NULL) {
        fprintf(stderr,
                "failed to allocate read buffer of %u bytes, exiting.",
                BUF_SIZE);
        exit(2);
    }

    /* ok.  open file.  let's read and parse */
    hand = yajl_alloc(&callbacks, &cfg, &allocFuncs, &holder);

    done = 0;
	while (!done) {
        rd = fread((void *) fileData, 1, BUF_SIZE, stdin);
        
        if (rd == 0) {
            if (!feof(stdin)) {
                fprintf(stderr, "error reading from file\n");
                break;
            }
            done = 1;
        }

        if (done)
            /* parse any remaining buffered data */
            stat = yajl_parse_complete(hand);
        else
            /* read file data, pass to parser */
            stat = yajl_parse(hand, fileData, rd);
        
        if (stat != yajl_status_insufficient_data &&
            stat != yajl_status_ok)
        {
            unsigned char * str = yajl_get_error(hand, 0, fileData, rd);
            fflush(stdout);
            fprintf(stderr, "%s", (char *) str);
            yajl_free_error(hand, str);
            break;
        }
    } 

    yajl_free(hand);
    return 0;
}
