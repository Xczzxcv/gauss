#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../my_head.h"

typedef struct{
    frac x;
    frac y;
}Point;

Point createP(int x, int y);
char* stringP(Point p);
frac* find_line_eqtn(Point p1, Point p2);
frac* kx_plusb(frac* ABC);
int checkP_line_kb(frac* kb, Point p);
int checkP_line_ABC(frac* ABC, Point p);
frac* height_lineP(frac* kb, Point p);
int** read_vars(int num_of_vars, char* filename);
float round_fl(float num, int signs);
frac float_to_frac(float num);
int sqrt_int(int num, int sub, int cnt);
frac sqrt_frac(frac fr);
frac cos_angle_betw_lines(frac* ABC1, frac* ABC2){
    frac num, denom, result, box,
        A1 = ABC1[0],
        B1 = ABC1[1],
        C1 = ABC1[2],
        A2 = ABC2[0],
        B2 = ABC2[1],
        C2 = ABC2[2],
        one = create_frac(1, 1);
    num = sum_frac( mult_frac(A1, A2), mult_frac(B1, B2) );
    box = sum_frac( mult_frac(A1, A1), mult_frac(B1, B1));
    printf("sqrt(%s) = ", string_frac(box));
    denom = sqrt_frac(box);
    printf("%s\n", string_frac(denom));
    denom = mult_frac( denom, sqrt_frac( sum_frac( mult_frac(A2, A2), mult_frac(B2, B2) ) ) );
    result = div_frac(num, denom);
    printf("%s : %s = %s\n", string_frac(num), string_frac(denom), string_frac(result));
    return result;
}

int main(){
//    int M = 500;
//    int res, from;
//    for(int i=1;i<=M;i++){
//        from = i;
//        res = sqrt_int(from, 1, 1);
//        printf("res: sqrt(%d) = %d\n", from, res);
//    }
//    getch();
//    frac  box,
//        start = create_frac(10, 1),
//        num = create_frac(9, 1);
    int cnt = 10,
        num_of_vars = 4,
        **varsM = read_vars(num_of_vars, "vars.txt"); // txt-file must end with an empty string
//    box = form_geron(num, start, cnt);
//    printf("result: %s\n", string_frac(box));
//    getch();
    show_intM(varsM, num_of_vars, 6, "\n");
    int i;
    Point A = createP(1, 4),
        B = createP(3, -5),
        C = createP(3, 2);
    printf("A: |%s|\n", stringP(A));
    printf("B: |%s|\n", stringP(B));
    printf("C: |%s|\n", stringP(C));
    frac *ABC = find_line_eqtn(A, B),
        *kb = kx_plusb(ABC),
        *new_ABC = height_lineP(ABC, A);
    for(i=0;i<3;i++)
        printf("ABC[%d]: %s\n", i, string_frac(ABC[i]));
    for(i=0;i<3;i++)
        printf("new_ABC[%d]: %s\n", i, string_frac(new_ABC[i]));
    for(i=0;i<2;i++)
        printf("kb[%d]: %s\n", i, string_frac(kb[i]));
//    for(i=0;i<2;i++)
//        printf("new_kb[%d]: %s\n", i, string_frac(new_kb[i]));
    printf("A: %d %d\n", checkP_line_kb(kb, A), checkP_line_ABC(ABC, A));
    printf("B: %d %d\n", checkP_line_kb(kb, B), checkP_line_ABC(ABC, B));
    printf("C: %d %d\n", checkP_line_kb(new_ABC, C), checkP_line_ABC(new_ABC, C));
    frac box = cos_angle_betw_lines(ABC, new_ABC);
    float box_fl = (float)box.num / box.denom;
    printf("cos betw ABC & new_ABC: %s\n", string_frac(box));
    printf("angle betw ABC & new_ABC: acos(%f) = %f\n", box_fl, acos(box_fl));
}

Point createP(int x, int y){
    Point box;
    box.x = create_frac(x, 1);
    box.y = create_frac(y, 1);
    return box;
}

char* stringP(Point p){
    char *box_str = malloc((strlen("x= y=") + 25*2) * sizeof(char));
    sprintf(box_str, "x=%s y=%s", string_frac(p.x), string_frac(p.y));
    return box_str;
}

frac* find_line_eqtn(Point p1, Point p2){
    //(x - p1.x) / (p2.x - p1.x) - (y-p1.y) / (p2.y-p1.y) = 0
    //formula on listochek u me est'
    frac denom = sub_frac(p2.x, p1.x);
    if (denom.num == 0);
    printf("test: %s %s\n", string_frac(p2.x), string_frac(p1.x));
    frac A = div_frac(create_frac(1, 1), denom),
        B = div_frac(create_frac(-1, 1), sub_frac(p2.y, p1.y)),
        C = sub_frac( div_frac( p1.y, sub_frac(p2.y, p1.y) ),
                     div_frac( (p1.x), sub_frac(p2.x, p1.x) ) ),
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

int checkP_line_kb(frac* kb, Point p){
    frac x = p.x,
        y = p.y,
        k = kb[0],
        b = kb[1];
    return are_equal_frac(sum_frac(mult_frac(k, x), b), y);;
}

int checkP_line_ABC(frac* ABC, Point p){
    frac x = p.x,
        y = p.y,
        A = ABC[0],
        B = ABC[1],
        C = ABC[2],
        box = create_frac(0, 1);
    printf("chck_ln_ABC: %s %s %s\n", string_frac(A), string_frac(B), string_frac(C));
    box = sum_frac(box, mult_frac(A, x));
    //printf("box1: %s\n", string_frac(box));
    box = sum_frac(box, mult_frac(B, y));
    //printf("box2: %s\n", string_frac(box));
    box = sum_frac(box, C);
    //printf("box3: %s\n", string_frac(box));
    return are_equal_frac(box, create_frac(0, 1));;
}
/*
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
*/

frac* height_lineP(frac* ABC, Point p){
//    new_k = -k
//    new_b = yp - xp * new_k
    frac *new_ABC = malloc(3 * sizeof(frac)),
        *kb = kx_plusb(ABC),
        k = kb[0],
        b = kb[1],
        *new_kb = malloc(2 * sizeof(frac));
    new_kb[0] = inverse_frac(k); // new_k
    new_kb[1] = sub_frac( p.y, mult_frac(p.x, inverse_frac(k)) ); // new_b
    new_ABC[0] = create_frac(-k.num, 1); // new_A
    new_ABC[1] = create_frac(-k.denom, 1); // new_B
    new_ABC[2] = create_frac(-b.num, 1); // new_C
    printf("||||%s %s\n", string_frac(new_kb[0]), string_frac(new_kb[1]));
    return new_ABC;
}

int** read_vars(int num_of_vars, char* filename){
    FILE *Fvars = fopen(filename, "r");
    if (!Fvars){
        printf("Can't open %s. Program will stopped.\n", filename);
        exit(666);
    }

    int i, j, box_cntr,
        **vars = malloc(num_of_vars * sizeof(int*));
    char ch,
        box_str[MAX_NUM_LENGTH];
    for(i=0;i<num_of_vars;i++){
        vars[i] =  malloc(6 * sizeof(int));
        j = 0;
        box_cntr = 0;
        do{
            ch = getc(Fvars);
            if(ch==',' || ch==' ' || ch=='\n'){
                vars[i][j] = atoi(box_str);
                memset(box_str, 't', sizeof(char)*MAX_NUM_LENGTH);
                j++;
                box_cntr = 0;
            }
            else{
                box_str[box_cntr] = ch;
                box_cntr++;
            }
        }while(ch!='\n');
    }
    return vars;
}

float round_fl(float num, int signs){
    num *= (int)(pow(10, signs)+0.1);
    num = floor(num+0.5);
    num /= (int)(pow(10, signs)+0.1);
    return num;
}

frac float_to_frac(float num){
    int int_part = (int)num,
        cnt = 0;
    float float_part = num - (int)num,
        prev;
    frac box_fr;
    do{
        prev = float_part;
        float_part *= 10;
        cnt++;
    }while(float_part != (int)float_part);
    box_fr = create_frac((int)float_part, (int)(pow(10, cnt)+0.1));
    box_fr = sum_frac( box_fr, create_frac(int_part, 1) );
    return box_fr;
}

int sqrt_int(int num, int sub, int cnt){
    num = num - sub;
    if (num <= 0)
        return cnt;
    else{
        sub += 2;
        cnt++;
        return sqrt_int(num, sub, cnt);
    }
}

frac sqrt_frac(frac fr){
    float box = (float)fr.num / fr.denom;
    box = sqrt(box);
    fr = float_to_frac(box);
    return fr;
}
/*
frac form_geron(frac num, frac prev, int cnt){
    printf("ger: %s %s %d\n", string_frac(num), string_frac(prev), cnt);
    if (cnt > 0){
        frac next;
        next = div_frac(num, prev);
        printf("next1: %s\n", string_frac(next));
        next = sum_frac(prev, next);
        printf("next2: %s\n", string_frac(next));
        next = mult_frac(create_frac(1, 2), next);
        printf("next3: %s\n", string_frac(next));
        return form_geron(num, next, --cnt);
    }
    else
        return prev;
}
*/
