/* test file
** used to try out elements of language to see if they work in a certain manner
** also used to test out individual functions
*/

#include <stdio.h>

struct s {
    int n;
};


int main() {
    struct s s1 = {5};
    struct s s2 = s1;
    s1.n = 7;
    printf("s1.n: %d\n", s1.n);
    printf("s2.n: %d\n", s2.n);


    return 0;
}
