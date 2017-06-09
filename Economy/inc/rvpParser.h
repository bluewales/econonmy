#include <stdio.h>
#include <string.h>

double rvp_parse(char * formula, int P[]);
double evaluate(double op1, double op2, char op);
double evaluate_three_param(double op1, double op2, double op3, char op);
double evaluate_one_param(double op1, char op);