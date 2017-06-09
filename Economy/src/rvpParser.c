#include <stdlib.h>
#include <cstdarg>
#include <time.h>
#include <math.h>
#include "fastRandom.h"
#include "rvpParser.h"

// the formula must be given in reverse polish, with no spaces
// variables are give in the form P1, P2, P3 etc
// the string must be terminated by a =
double rvp_parse(char * formula, int P[]) {
	double stack[100];
	bzero(stack,100 * sizeof(double));
	int stackIndex = -1;
	
	int i;
	for(i = 0; formula[i]; i++) {
		if(formula[i] == '(') { // enstack the number
			i++;
			stackIndex++;
			if(formula[i] == 'P') { // number is a parameter
				i++;
				stack[stackIndex] = P[formula[i] - '0'];
				i++;
			} else if(formula[i] == 'R') { // number is a random number, generated positive
				int random_number = abs(CONG()); 
				stack[stackIndex] = (double)(random_number); 
				i++;
			} else { // number is in a string
				int k = 0;
				char temp[20];
				while (formula[i] != ')')
					temp[k++] = formula[i++];
				temp[k] = 0;
				stack[stackIndex] = atof(temp);
			}
		} else if(formula[i] == '=') { // this is the end, return the number
			if(stackIndex) {
				printf("%s\n", formula);
				printf("stackIndex: %d\n", stackIndex);
			}
			return stack[stackIndex];
		} else if(formula[i] == '?') { // terciary operators
			stack[stackIndex-2] = evaluate_three_param(stack[stackIndex-2], stack[stackIndex-1], stack[stackIndex], formula[i]);
			stackIndex -= 2;
		} else if(formula[i] == 's') { // terciary operators
			stack[stackIndex] = evaluate_one_param(stack[stackIndex], formula[i]);
		} else { // binary operators
			stack[stackIndex - 1] = evaluate(stack[stackIndex-1], stack[stackIndex], formula[i]);
			stackIndex--;
		}
	}
	printf("\n*********************************RVParser*exited in error\n");
	return 0;
} 

double evaluate(double op1, double op2, char op) {
	switch(op) {
		case '!' :
			break;
		case '%' :
			if(op2 < 1)
				return 0;
			else
				return ((int)op1) % ((int)op2);
			break;
		case '&' :
			return ((int)op1) && ((int)op2);
			break;
		case '*' :
			return op1 * op2;
			break;
		case '+' :
			return op1 + op2;
			break;
		case '-' :
			return op1 - op2;
			break;
		case '/' :
			return op1 / op2;
			break;
		case ':' :
			return op1 == op2;
			break;
		case '^' :
			break;
		case '|' :
			return (int)op1 || (int)op2;
		case '<' :
			return op1 < op2;
		case '>' :
			return op1 > op2;
			break;
	}
	return 0;
}

double evaluate_three_param(double op1, double op2, double op3, char op) {	
	switch(op) {
		case '?' :
			if((int)op3)
				return op1;
			else 
				return op2;
			break;
		default:
			break;
	}
	return 0;
}

double evaluate_one_param(double op1, char op) {	
	switch(op) {
		case 's' :
			return sqrt(op1);
		default:
			break;
	}
	return 0;
}
