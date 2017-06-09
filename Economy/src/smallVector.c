#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "smallVector.h"

smallVector * svcreate(unsigned int size) {
	if(!size) size = 63;
	size += 1;
	smallVector * v = (smallVector *)malloc(sizeof(smallVector));
	v->array = (void **)malloc((size) * sizeof(void *));
	memset(v->array, 0, (size) * sizeof(void *));
	v->size = size; 
	v->count = 0;
	return v;
}

void svdestroy(smallVector * v) {
	if(v) {
		free(v->array);
		free(v);
	}
}

int svset(smallVector * v, unsigned int index, void * val) {
	if(!v)
		return 0;
	if(v->size <= index) {
		unsigned int size = v->size;
		unsigned int old_size = size;

		size = index + size;
		
		
		//printf("resizing from %d to %d for %d\n", old_size, size, index);
		v->array = (void **)realloc(v->array, (size) * sizeof(void *));
		if(!(v->array)) return 0;
		memset(&(v->array)[old_size], 0, (size - old_size) * sizeof(void *));
		v->size = size;
	}
	if(!v->array[index]) {
		v->count++;
	}
	if (!val) {
		v->count--;
	}
	v->array[index] = val;
	return 1;
}

void * svget(smallVector * v, unsigned int index) {
	if(!v)
		return 0;
	if(v->size <= index) {
		return 0;
	}
	return v->array[index];
}
/*
#define n 5000
int main() {
	int i, j;
	printf("Testing smallVector\n");
	
	int keys[n];
	int numbers[n];
	srand(n);
	for(j = 0; j < n; j++) {
		if(j%100 == 0)
			printf("%d\n",j);
		smallVector * v = svcreate(j);
		int max = rand() % n + j;
		int trys = rand() % 5000 + 5;
		for( i = 0; i < j; i+=((j/trys > 1)?(j/trys):1) ) {
			int sample = 1;
			while(sample) {
				keys[i] = rand() % (max);
				sample = (int)svget(v,keys[i]);
			}
			numbers[i] = rand();
			svset(v,keys[i],(void *)numbers[i]);
		}
		for(i = 0; i < j; i+=((j/trys > 1)?(j/trys):1) ) {
			int number = (int)svget(v,keys[i]);
			if(number != numbers[i])
				printf("%d != %d i=%d\n", number, numbers[i], i);
		}
		svdestroy(v);
	}
}
*/
