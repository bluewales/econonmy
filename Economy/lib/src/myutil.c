#include "myutil.h"
#include "stdio.h"

int indexOfSubstr(char * string, char * substring, int startIndex) {
	int strsize = strlen(string);
	int substrsize = strlen(substring);
	
	if(startIndex >= strsize || startIndex < 0)
		return -1;
	int i;
	for(i = startIndex; i < strsize; i++) {
		if(string[i] == substring[0]) {
			int j;
			for(j = 0; j < substrsize; j++) {
				if(string[i+j] != substring[j])
					break;
				if(j + 1 == substrsize)
					return i;
			}
		}
	}
	return -1;
}

void start_timer(struct timeval * start) {
	gettimeofday(start, NULL);
} // start_timer

long stop_timer(struct timeval * start) {
	struct timeval end;
	long seconds, useconds;
	gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start->tv_sec;
    useconds = end.tv_usec - start->tv_usec;
    return (long)(((seconds) * 1000 + useconds/1000.0) + 0.5);
} // stop_timer
