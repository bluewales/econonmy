// function example
#include <iostream>
#include "rvpParser.h"
using namespace std;

int main ()
{
	
	//  (P2)(10)/(1.2)*(1)+(1.8)*(15)(P1)(P3)(1)+(-.1)**-(5)/+=
	//  ((P2/10)*1.2+1)*1.8+(15-P1*(P3+1)*-.1)/5
/*	
	P1 quality
	P2 affinity
	P3 already_possessed
	P4 producing_skill
	P5 utilizing_skill
	P6 age
*/
	//char * number = (char *)"(P2)(P1)*s(10)/(1)+(0)(P3)(75)>?=";
	char * number = (char *)"(P2)(P2)(10)*(P3)?=";
	int P1, P2, P3, P4, P5, P6;
	P1 = 0;  // quality
	P2 = 10; // affinity
	P3 = 1;  // already_possessed
	P4 = 5;  // producing_skill
	P5 = 5;  // utilizing_skill
	P6 = 25; // age
	
	int P[4];
	int i, j, k;
	
	for(i = 0; i < 101; i+=20) {
		for(j = 0; j < 101; j+=20) {
			for(k = 0; k < 101; k+=20) {
				P[1] = i;
				P[2] = j;
				P[3] = k;
				printf("skill:%d, tools:%d, potential:%d = %g\n",
				P[1],P[2],P[3], rvp_parse(number,P));
			}
		}
	}
	
	
	return 0;
}
