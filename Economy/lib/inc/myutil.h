#include <string.h>
#include <sys/time.h>
int indexOfSubstr(char*, char*, int);
void start_timer(struct timeval * start);
long stop_timer(struct timeval * start);
