//test
//used to test code for program 2, ece-209
//Alexander Minser
//10/20/2017


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>


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

int main() {
    int coeff[5] = {-4, 2, -3, 0, -1};
    int degree = 4;
    printPoly(coeff, degree);

    return 0;
}
