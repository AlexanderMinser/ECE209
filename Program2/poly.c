/*
 * Header goes here
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include<ctype.h>

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



/* function header */
//***Don't forget to check if polynomial is valid
int readPoly(int coeff[], int degree)
{
    char input[100];
    fgets(input, 100, stdin);
    char* token = strtok(input, " ");
    char* temp;
    int deg;
    int cof;
    int isNeg = 0;
    int i;
    for(i=0; i<=degree; i++) {
        coeff[i] = 0;
    }
    while(token != NULL) {
        if (isdigit(*token)){
            cof = strtoul(token, &temp, 10);
            cof = isNeg? -1 * cof : cof;
            isNeg = 0;
            if (temp[1] == '^'){
                    deg = temp[2] - '0';
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
                deg = (*(token + 2)) - '0';
            else
                deg = 1;
            coeff[deg] = cof;
        } else if (*token == '-') {
            isNeg = 1;
        }

        token = strtok(NULL, " ");
    }

    return 1;
}
/* function header */
void printPoly(int coeff[], int degree)
{
    int i;
    char operator = '+';
    if (coeff[degree] == 1)
        printf("x^%d", degree);
    else
        printf("%dx^%d", coeff[degree], degree);
    for(i=degree-1; i>-1; i--) {
        if (coeff[i] < 0)
            operator = '-';
        else
            operator = '+';
        coeff[i] = abs(coeff[i]);
        if (coeff[i] == 0)
            continue;
        else if (coeff[i] == 1)
            if (i == 1)
                printf(" %c x", operator);
            else
                printf(" %c x^%d", operator, i);
        else if (i==1)
            printf(" %c %dx", operator, coeff[i]);
        else if (i==0)
            printf(" %c %d", operator, coeff[i]);
        else
            printf(" %c %dx^%d", operator, coeff[i], i);
    }
    printf("\n"); //****GET RID OF BEFORE SUBMITTING***
    return;
}

/* function header */
double evalPoly(int *coeff, int degree, double x)
{
    return 0;
}

/* function header */
double integratePoly(int *coeff, int degree, double low, double high)
{
    return 0;
}

/* function header */
int multPoly(int c1[], int d1, int c2[], int d2, int c3[], int d3)
{
    return 1;
}

/* function header */
int genPoly(int c[], int d, int r[], int n)
{
    return 1;
}
