/*
 * Alexander Minser
 * ECE 209 Spring 2017
 * Professor Sanders
 * 10/27/2017
 * Program peforms various operations dealing with polynomials
 * Functions implemented include reading, printing, evaluating, integrating,
 * multiplying, and generating polynomials
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* functions to be implemented */
int readPoly(int coeff[], int degree);
void printPoly(int coeff[], int degree);
double evalPoly(int coeff[], int degree, double x);
double integratePoly(int coeff[], int degree, double low, double high);
int multPoly(int c1[], int d1, int c2[], int d2, int c3[], int d3);
int genPoly(int coeff[], int degree, int roots[], int nroots);

/* YOUR SUBMISSION SHOULD HAVE THE EXACT SAME CODE BETWEEN THE TWO LINES*/
/*-----------------------------------------------------------------*/
#ifndef NOMAIN

int main()
{
    int f4[5] = {0};  /* f(x) - degree = 4 or less */
    int g4[5] = {0};  /* g(x) - degree = 4 or less */
    int p8[9] = {0};  /* p(x) - degree = 8 or less */
    int roots[4];     /* roots for generating */
    int degree = 0;   /* degree of generated poly */
    int status = 1;
    double x = 0;     /* point for evaluation */
    double low = 0, high = 0;  /* limits for integration */
    char command;     /* user command character */
    int done = 0;     /* set to 1 to quit */
    int i;

    while (!done)
    {
        printf("\nEnter a command (e,i,g,m,q): ");
        do
        {
            command = getchar();
        } while (command == '\n');
        while (getchar() != '\n');

        switch (command)
        {
            /* e: evaluate a polynomial */
            case 'e':
            {
                printf("EVALUATE\n");
                printf("Enter a polynomial f(x):\n");
                status = readPoly(f4 ,4);
                if (!status)
                {
                    printf("Enter x: ");
                    scanf("%lf", &x);
                    printf("f(%g) = %g\n", x, evalPoly(f4, 4, x));
                }
                else
                {
                    printf("ERROR: Could not read polynomial.\n");
                }
                break;
            }
            /* integrate a polynomial */
            case 'i':
            {
                printf("INTEGRATE\n");
                printf("Enter a polynomial f(x):\n");
                status = readPoly(f4, 4);
                if (!status)
                {
                    printf("Enter lower limit: ");
                    scanf("%lg", &low);
                    printf("Enter upper limit: ");
                    scanf("%lg", &high);
                    printf("Integral of f(x) = %g\n", integratePoly(f4, 4, low, high));
                }
                else
                {
                    printf("ERROR: Could not read polynomial.\n");
                }
                break;
            }
            /* generate a polynomial */
            case 'g':
            {
                printf("GENERATE\n");
                printf("Enter the degree (<= 4): ");
                scanf("%d", &degree);
                for (i=0; i<=4; i++) roots[i] = 0;
                if ((degree <= 4) && (degree > 0))
                {
                    for (i=0; i<degree; i++)
                    {
                        printf("Integer Root %d = ", i+1);
                        scanf("%d", &roots[i]);
                    }
                    status = genPoly(f4, degree, roots, degree);
                    if (!status)
                    {
                        printf("f(x) = ");
                        printPoly(f4, degree);
                    }
                    else
                    {
                        printf("ERROR: could not generate polynomial");
                    }
                    printf("\n");
                }
                else
                {
                    printf("ERROR: Degree must be between 1 and 4.\n");
                }
                break;
            }
            /* multiply two polynomials */
            case 'm':
            {
                printf("MULTIPLY\n");
                printf("Enter first polynomial f(x):\n");
                status = readPoly(f4,4);
                if (!status)
                {
                    printf("Enter second polynomial g(x):\n");
                    status = readPoly(g4,4);
                    if (!status)
                    {
                        printf("f(x) * g(x) = ");
                        status = multPoly(f4, 4, g4, 4, p8, 8);
                        if (status) printf("ERROR");
                        else printPoly(p8, 8);
                        printf("\n");
                    }
                    else
                    {
                        printf("ERROR: Could not read polynomial.\n");
                    }
                }
                else
                {
                    printf("ERROR: Could not read polynomial.\n");
                }
                break;
            }
            /* quit */
            case 'q':
            {
                done = 1;
                break;
            }
        }
    }
    return (EXIT_SUCCESS);
}

#endif  /*NOMAIN*/
/*-----------------------------------------------------------------*/
/* YOUR SUBMISSION SHOULD HAVE THE EXACT SAME CODE BETWEEN THE TWO LINES*/



/* Function for reading in polynomial provided by user at command line
*  entire polynomial is read in as string
*  then split up using strtok on spaces
*
*
*  PARAMETERS:
*  int coeff[] ---> array of coefficients corresponding to their "degree" (index)
*  int degree ---> the highest degree of the given polynomial (largest index in array)
*  RETURN:
*  integer value 0 or 1 indicating success or failure accordingly
*/
int readPoly(int coeff[], int degree)
{
    char input[100];
    fgets(input, 100, stdin);
    char* token = strtok(input, " "); /* string split on ' ' */
    char* temp;                       /* holds rest of string after strtoul */
    int deg;                          /* degree of term currently being parsed */
    int cof;                          /* coefficient of term currently being parsed */
    int isNeg = 0;
    int i;
    for(i=0; i<=degree; i++) {
        coeff[i] = 0;
    }
    while(token != NULL) {
        if (isdigit(*(token+1)) && (*token) == '-') { /* if first term and negative */
            cof = strtoul(token, &temp, 10);
            if (temp[1] == '^'){
                    deg = strtoul((temp+2), &temp, 10);
                    if (deg > degree)
                        return 1; /*error handling, input polynomial of too high degree */
            } else if (temp[0] == 'x'){
                deg = 1;
            } else {
                deg = 0;
            }
            coeff[deg] = cof;
        }
        else if (isdigit(*token)){
            cof = strtoul(token, &temp, 10);
            cof = isNeg? -1 * cof : cof;
            isNeg = 0;
            if (temp[1] == '^'){
                    deg = strtoul((temp+2), &temp, 10);
                    if (deg > degree)
                        return 1; /*error handling, input polynomial of too high degree */
            } else if (temp[0] == 'x'){
                deg = 1;
            } else {
                deg = 0;
            }
            coeff[deg] = cof;
        } else if (*token == 'x') {
            cof = isNeg? -1 : 1;
            isNeg = 0;
            if (*(token+1) == '^')
                deg = strtoul((token + 2), &temp, 10);
                if (deg > degree)
                    return 1; /*error handling, input polynomial of too high degree */
            else
                deg = 1;
            coeff[deg] = cof;
        } else if (*token == '-') {
            isNeg = 1;
        }

        token = strtok(NULL, " ");
    }

    return 0; /* indicates function ran successfully*/
}

/* Function for printing a given polynomial
*  First prints highest degree term in polynomial,
*  then iterates through coefficient array printing the remaining terms of
*  the polynomial, making various checks along the way to assure proper formatting
*
*  PARAMETERS:
*  int coeff[] ---> array of coefficients corresponding to their "degree" (index)
*  int degree ---> the highest degree of the given polynomial (largest index in array)
*  RETURN:
*  nothing, function only performs outuput
*/
void printPoly(int coeff[], int degree)
{
    int i;
    int absoluteCoeff;
    int highestNotZero; /*highest term in polynomial not zero */
    char operator = '+';
    for(i=degree; i>=0; i--){
        if (coeff[i] != 0){
            highestNotZero = i;
            break;
        }
    }
    if (coeff[highestNotZero] == 1)
        printf("x^%d", highestNotZero);
    else
        printf("%dx^%d", coeff[highestNotZero], highestNotZero);
    for(i=highestNotZero-1; i>-1; i--) {
        if (coeff[i] < 0)
            operator = '-';
        else
            operator = '+';
        absoluteCoeff = abs(coeff[i]);
        if (absoluteCoeff == 0)
            continue;
        else if (absoluteCoeff == 1)
            if (i == 1)
                printf(" %c x", operator);
            else if (i == 0)
                printf(" %c 1", operator);
            else
                printf(" %c x^%d", operator, i);
        else if (i==1)
            printf(" %c %dx", operator, absoluteCoeff);
        else if (i==0)
            printf(" %c %d", operator, absoluteCoeff);
        else
            printf(" %c %dx^%d", operator, absoluteCoeff, i);
    }
    printf("\n"); //****GET RID OF BEFORE SUBMITTING***
    return;
}


/* Function evalutes a polynomial at a given value for x
*  by raising that value of x to the degree and
*  multiplying that by it's corresponding coefficient
*
*  PARAMETERS:
*  int *coeff ---> array of coefficients corresponding to their "degree" (index)
*  int degree ---> the highest degree of the given polynomial (largest index in array)
*  double x ---> value to sub in for x in polynomial
*  RETURN:
*  sum of polynomial terms evaluated at given x
*/
double evalPoly(int *coeff, int degree, double x)
{
    int i;
    double sum = 0.0;
    for (i=0; i<=degree; i++){
        sum += coeff[i] * pow(x, i);
    }

    return sum;
}

/* Function integrates given polynomial between bounds low and high
*  uses integration formula: sum a[i](v^(i+1)-u^(i+1))/(i+1)
*  where i is index, v is upper bound, u is lower bound, and
*  a is coefficient
*
*  PARAMETERS:
*  int *coeff ---> array of coefficients corresponding to their "degree" (index)
*  int degree ---> the highest degree of the given polynomial (largest index in array)
*  double low ---> low bound of integration
*  double high ---> high bound of integration
*  RETURN:
*  value of integration taken at bounds low and high
*/
double integratePoly(int *coeff, int degree, double low, double high)
{
    int i;
    double sum = 0.0;
    for(i=0; i<=degree; i++) {
        sum += coeff[i] * ((pow(high, i+1)-pow(low, i+1))/(i+1));
    }

    return sum;
}

/* Function multiplies two polynomials together
*  function uses nested loops to multiply each term together
*  and stores in correct place in array given by degree
*
*  PARAMETERS:
*  int c1[] ---> array of coefficients for first polynomial to multiply
*  int d1 ---> degree of first polynomial to multiply
*  int c2[] ---> array of coefficients for second polynomial to multiply
*  int d2 ---> degree of second polynomial to multiply
*  int c3[] ---> array to store product polynomial in
*  int d3 ---> degree of product polynomial
*  RETURN:
*  0 or 1 to indicate success or failure respectively
*/
int multPoly(int c1[], int d1, int c2[], int d2, int c3[], int d3)
{
    int i;
    int j;
    int deg; /*current degree after any given multiplication b/w terms */
    if (d1+d2 > d3)
        return 1; /*error handling, product polynomial of too high degree */
    for(i=0; i<=d3; i++) {
        c3[i] = 0;
    }
    for(i=0; i<=d1; i++) { /* mutliplies */
        for(j=0; j<=d2; j++) {
            deg = i+j;
            if (c3[deg] == 0)
                c3[deg] = c1[i] * c2[j];
            else
                c3[deg] += c1[i] * c2[j];
        }
    }
    return 0;
}

/* Function generates a polynomial based on the roots provided
*  Uses nested loops to update each term based on next root being multiplied
*
*  PARAMETERS:
*  int c[] ---> array to store generated polynomial in
*  int d ---> highest degree of generated polynomial
*  int r[] ---> array of roots to create polynomial from
*  int n ---> number of roots in polynomial (highest index + 1)
*  RETURN:
*  0 or 1 to indicate success or failure respectively
*/
int genPoly(int c[], int d, int r[], int n)
{
    int i;
    int j;
    int last; /*least significant term coefficient*/
    if (d < n)
        return 1; /*error handling, product polynomial of too high degree */
    for (i=0; i<=d; i++) { /*set all values in new array to zero*/
        c[i] = 0;
    }
    c[1] = 1;
    c[0] = (-1*r[0]);
    for (i=1; i<n; i++) { /*foil terms*/
        last = c[0]*(-1*r[i]);
        for(j=i; j>=0; j--) {
            c[j+1] = c[j] + c[j+1]*(-1*r[i]);
        }
        c[0] = last;
    }
    return 0;
}
