//test
//used to test code for program 2, ece-209
//Alexander Minser
//10/20/2017


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<math.h>

int getNum(char c) {
    return ((int) c) - 48;
}


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

void testSTRTOK() {
    char input[100];
    fgets(input, 100, stdin);
    char* token = strtok(input, " ");
    char* temp;
    int deg;
    int cof;

    while(token != NULL) {
        if (isdigit(*token)){
            cof = strtoul(token, &temp, 10);
            //printf("%s\n", token);
            if (temp[1] == '^'){
                    deg = temp[2] - '0';
            } else if (temp[0] == 'x'){
                deg = 1;
            } else {
                deg = 0;
            }
        }
        if (*token != '+' && *token != '-')
            printf("coefficient: %d, degree: %d\n", cof, deg);
        token = strtok(NULL, " ");
    }

}

void printPoly(int coeff[], int degree)
{
    int i;
    int absoluteCoeff;
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
        absoluteCoeff = abs(coeff[i]);
        if (absoluteCoeff == 0)
            continue;
        else if (absoluteCoeff == 1)
            if (i == 1)
                printf(" %c x", operator);
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

double evalPoly(int *coeff, int degree, double x)
{
    int i;
    double sum = 0.0;
    for (i=0; i<=degree; i++){
        printf("coeff: %d\n", coeff[i]);
        printf("num: %f\n", coeff[i] * pow(x,i));
        sum += coeff[i] * pow(x, i);
    }

    return sum;
}

double integratePoly(int *coeff, int degree, double low, double high)
{
    int i;
    double sum = 0.0;
    for(i=0; i<=degree; i++) {
        sum += coeff[i] * ((pow(high, i+1)-pow(low, i+1))/(i+1));
    }
    printf("integral: %f\n", sum);
    return sum;
}

int main() {
    int coeff[5] = {-4, 2, -3, 0, -1};
    int degree = 4;
    printPoly(coeff, degree);
    integratePoly(coeff, degree, 3, 5);

    return 0;
}
