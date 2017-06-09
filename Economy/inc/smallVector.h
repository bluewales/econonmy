#ifndef smallVector_h_
#define smallVector_h_

typedef struct {
	unsigned int size;
	unsigned int count;
	void ** array;
} smallVector;

smallVector * svcreate(unsigned int size);
void svdestroy(smallVector * v);
int svset(smallVector * v, unsigned int index, void * val);
void * svget(smallVector * v, unsigned int index);

#endif
