#include "myYajlParser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>


#define BUF_SIZE 2048

int myYajlParse(FILE * file, yajl_callbacks callbacks, void * ctx)
{
    yajl_handle hand;
    unsigned char fileData[BUF_SIZE];
    yajl_status stat;
    size_t rd;
    int done;
	
	if(file == NULL) {
		printf("myYajlParse file ERROR\n");
		exit(2);
	}


    if (fileData == NULL) {
        fprintf(stderr,
                "failed to allocate read buffer of %u bytes, exiting.",
                BUF_SIZE);
        exit(2);
    }

    /* ok.  open file.  let's read and parse */
    hand = yajl_alloc(&callbacks, NULL, ctx);
	
	yajl_config(hand, yajl_allow_comments, 1); 
	
    done = 0;
	
	while (!done) {
	
		rd = fread((void *) fileData, 1, BUF_SIZE, file);
		
		//fwrite (fileData, 1, rd, stdout);
        if (rd == 0) {
            done = 1;
        }

        if (done) {
            /* parse any remaining buffered data */
            stat = yajl_complete_parse(hand);
        } else {
            /* read file data, pass to parser */
            stat = yajl_parse(hand, fileData, rd);
        }
        if (stat != yajl_status_ok) {
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
