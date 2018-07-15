#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../my_head.h"

typedef struct{
    frac x;
    frac y;
}Point;

Point* createP(int x, int y);
char* stringP(Point* p);
frac* determ_line_eqtn(Point* p1, Point* p2);
frac* kx_plusb(frac A, frac B, frac C);


int main(){
    frac a = create_frac(3, 4),
        b = create_frac(21, 32);
    //printf("%s : %s = %s\n", string_frac(a), string_frac(inverse_frac(b)), string_frac(div_frac(a, inverse_frac(b))));
    int i;
    Point *A = createP(1, 4);
    Point *B = createP(3, -5);
    Point *C = createP(3, 2);
    printf("A: |%s|\n", stringP(A));
    printf("B: |%s|\n", stringP(B));
    printf("C: |%s|\n", stringP(C));
    frac *ABC = determ_line_eqtn(A, B),
        *kb = kx_plusb(ABC[0], ABC[1], ABC[2]);
    for(i=0;i<3;i++)
        printf("ABC[%d]: %s\n", i, string_frac(ABC[i]));
    for(i=0;i<2;i++)
        printf("kb[%d]: %s\n", i, string_frac(kb[i]));
}


Point* createP(int x, int y){
    Point *box = malloc(sizeof(Point));
    box->x = create_frac(x, 1);
    box->y = create_frac(y, 1);
    return box;
}

char* stringP(Point* p){
    char *box_str = malloc((strlen("x= y=") + 25*2) * sizeof(char));
    sprintf(box_str, "x=%s y=%s", string_frac(p->x), string_frac(p->y));
    return box_str;
}

frac* determ_line_eqtn(Point* p1, Point* p2){
    //(x - p1->x) / (p2->x - p1->x) - (y-p1->y) / (p2->y-p1->y) = 0
    printf("%s\n%s\n", stringP(p1), stringP(p2));
    frac A = div_frac(create_frac(1, 1), sub_frac(p2->x, p1->x)),
        B = div_frac(create_frac(-1, 1), sub_frac(p2->y, p1->y)),
        C = sub_frac( div_frac( p1->y, sub_frac(p2->y, p1->y) ),
                     div_frac( (p1->x), sub_frac(p2->x, p1->x) ) ),
        *array = malloc(3 * sizeof(frac));
    array[0] = A;
    array[1] = B;
    array[2] = C;
    printf("d_l_eq: %s %s %s\n", string_frac(A), string_frac(B), string_frac(C));
    printf("d_l_eq: %s %s %s\n", string_frac(array[0]), string_frac(array[1]), string_frac(array[2]));
    return array;
}

frac* kx_plusb(frac A, frac B, frac C){
    //    Ax + By + C = 0
    //    y = (Ax+C) / -B
    //    y = (A/-B)x + C/-B
    //    k = A/-B, b = C/-B
    printf("kx_plusb: %s %s %s\n", string_frac(A), string_frac(B), string_frac(C));
    frac k = div_frac(A, inverse_frac(B)),
        b = div_frac(C, inverse_frac(B));
    frac *array = malloc(2 * sizeof(frac));
    array[0] = k;
    array[1] = b;
    return array;
}
