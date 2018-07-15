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
frac* find_line_eqtn(Point* p1, Point* p2);
frac* kx_plusb(frac* ABC);
int checkP_line(frac* kx, Point p);
frac* height_lineP(frac* ABC, Point p);
frac* height_linePOTHER(frac* kb, Point p);
int** read_vars(int num_of_vars, char* filename){
    FILE *Fvars = fopen(filename, "r");
    if (!Fvars){
        printf("Can't open %s. Program will stopped.\n", filename);
        exit(666);
    }

    int i, j,
        **vars = malloc(num_of_vars);
    char ch,
        box[MAX_NUM_LENGTH];
    for(i=0;i<num_of_vars;i++){
        vars[i] = malloc(6 * sizeof(int));
        j = 0;
        do{
            ch = getc(Fvars);
            if(ch==',' || ch==' ');
            j++;
        }while(ch!='\n');

    }
    return vars;
}

int main(){
    int **varsM = read_vars(4, 'vars.txt');
    int i;
    Point *A = createP(1, 4);
    Point *B = createP(3, -5);
    Point *C = createP(3, 2);
    printf("A: |%s|\n", stringP(A));
    printf("B: |%s|\n", stringP(B));
    printf("C: |%s|\n", stringP(C));
    frac *ABC = find_line_eqtn(A, B),
        *new_ABC = height_lineP(ABC, *C),
        *kb = kx_plusb(ABC),
        *new_kb = height_linePOTHER(kb, *C);
    for(i=0;i<3;i++)
        printf("ABC[%d]: %s\n", i, string_frac(ABC[i]));
    for(i=0;i<3;i++)
        printf("new_ABC[%d]: %s\n", i, string_frac(new_ABC[i]));
    for(i=0;i<2;i++)
        printf("kb[%d]: %s\n", i, string_frac(kb[i]));
    for(i=0;i<2;i++)
        printf("new_kb[%d]: %s\n", i, string_frac(new_kb[i]));
    printf("A: %d\n", checkP_line(kb, *A));
    printf("B: %d\n", checkP_line(kb, *B));
    printf("C: %d\n", checkP_line(new_kb, *C));
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

frac* find_line_eqtn(Point* p1, Point* p2){
    //(x - p1->x) / (p2->x - p1->x) - (y-p1->y) / (p2->y-p1->y) = 0
    //formula on listochek u me est'
    frac A = div_frac(create_frac(1, 1), sub_frac(p2->x, p1->x)),
        B = div_frac(create_frac(-1, 1), sub_frac(p2->y, p1->y)),
        C = sub_frac( div_frac( p1->y, sub_frac(p2->y, p1->y) ),
                     div_frac( (p1->x), sub_frac(p2->x, p1->x) ) ),
        *array = malloc(3 * sizeof(frac));
    array[0] = A;
    array[1] = B;
    array[2] = C;
    return array;
}

frac* kx_plusb(frac* ABC){
    //    Ax + By + C = 0
    //    y = (Ax+C) / -B
    //    y = (A/-B)x + C/-B
    //    k = A/-B, b = C/-B
    frac A = ABC[0],
        B = ABC[1],
        C = ABC[2],
        k = div_frac(A, inverse_frac(B)),
        b = div_frac(C, inverse_frac(B));
    frac *array = malloc(2 * sizeof(frac));
    array[0] = k;
    array[1] = b;
    return array;
}

int checkP_line(frac* kb, Point p){
    frac x = p.x,
        y = p.y,
        k = kb[0],
        b = kb[1];
    return are_equal_frac(sum_frac(mult_frac(k, x), b), y);;
}

frac* height_lineP(frac* ABC, Point p){
//    A(y-y1)-B(x-x1)=0
//    Ay - Ay1 - Bx + Bx1 = 0
//    -Bx + Ay + (Bx1 - Ay1) = 0
    frac A = ABC[0],
        B = ABC[1],
        C = ABC[2],
        *new_ABC = malloc(3 * sizeof(frac));
    new_ABC[0] = inverse_frac(B); // new_A
    new_ABC[1] = A; // new_B
    new_ABC[2] = sub_frac( mult_frac(B, p.x), mult_frac(A, p.y) ); // new_C
    return new_ABC;
}

frac* height_linePOTHER(frac* kb, Point p){
//    y–y1=-a(x-x1)
//    y = (-1/a)x + ((1/a)x1 + y1)

    frac k = kb[0],
        b = kb[1],
        *new_kb = malloc(2 * sizeof(frac));
    new_kb[0] = inverse_frac(k); // new_k
    new_kb[1] = sub_frac( p.y, mult_frac(p.x, inverse_frac(k)) ); // new_b
    return new_kb;
}
