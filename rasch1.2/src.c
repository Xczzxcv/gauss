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
long long int** read_vars(int* vars_num, char* filename);
float round_fl(float num, int signs);
int sqrt_int(int num, int sub, int cnt);
frac cos_angle_betw_lines(frac* ABC1, frac* ABC2);
frac* sub_ect(frac* coeffs1, frac* coeffs2, int length);
frac* mult_eqt(frac* coeffs, int length, frac num);
frac find_dist_PP(Point p1, Point p2);
Point lines_intersec(frac* ABC1, frac* ABC2){
    printf("\tINPUT\n");
    printf("\tABC1: %s %s %s\t", stringF(ABC1[0]), stringF(ABC1[1]), stringF(ABC1[2]));
    printf("\tABC2: %s %s %s\n", stringF(ABC2[0]), stringF(ABC2[1]), stringF(ABC2[2]));

    int i,
        lenABC = 3;
    frac *ABCres[lenABC],
        ABC1_cff, ABC2_cff,
        x, y;
    Point p_intersec;

    ABC1_cff = ABC1[1];
    ABC2_cff = ABC2[1];
    ABC1 = mult_eqt(ABC1, lenABC, ABC2_cff);
    ABC2 = mult_eqt(ABC2, lenABC, ABC1_cff);
    printf("ABC1: %s %s %s\t", stringF(ABC1[0]), stringF(ABC1[1]), stringF(ABC1[2]));
    printf("ABC2: %s %s %s\n", stringF(ABC2[0]), stringF(ABC2[1]), stringF(ABC2[2]));
    ABC1 = sub_ect(ABC1, ABC2, lenABC);
    printf("ABC1: %s %s %s\t", stringF(ABC1[0]), stringF(ABC1[1]), stringF(ABC1[2]));
    printf("ABC2: %s %s %s\n", stringF(ABC2[0]), stringF(ABC2[1]), stringF(ABC2[2]));

    x = div_frac(ABC1[2], inverse_frac(ABC1[0]));
    y = sub_frac( inverse_frac( div_frac(ABC2[2], ABC2[1]) ), mult_frac(div_frac(ABC2[0], ABC2[1]), x));
    p_intersec.x = x;
    p_intersec.y = y;
    printf("P_intersec: %s\n", stringP(p_intersec));
    return p_intersec;
}
frac dist_lineP(frac* ABC, Point p){
    frac distance,
        *height_ABC = height_lineP(ABC, p);
    Point p_intersec = lines_intersec(ABC, height_ABC);
    printf("Let's find dist: (%s) | (%s)\n", stringP(p), stringP(p_intersec));
    distance = find_dist_PP(p, p_intersec);
    //printf("Distance: %f\n", frac_to_float(distance));
    return distance;
}
frac alternative(frac* ABC, Point p){
    return div_frac( modulo_frac( sum_frac( sum_frac( mult_frac(ABC[0], p.x), mult_frac(ABC[1], p.y) ), ABC[2] ) ),
                   sqrt_frac( sum_frac( mult_frac(ABC[0], ABC[0]), mult_frac(ABC[1], ABC[1]) ) ) );
}

int main(){
    int i, curr_var = -1,
        num_of_vars = 0;
    long long int **varsM = read_vars(&num_of_vars, "vars.txt"); // txt-file must end with an empty string
    //show_intM(varsM, num_of_vars, 6, "\n");
    printf("Kakoi variant jelaete reshit'? (1-%d)\n", num_of_vars);
    scanf("%d", &curr_var);
    printf("\t\tVAR %d\n", curr_var);
    curr_var--; // real shit
    Point A = createP(varsM[curr_var][0], varsM[curr_var][1]),
        B = createP(varsM[curr_var][2], varsM[curr_var][3]),
        C = createP(varsM[curr_var][4], varsM[curr_var][5]);
    printf("A: |%s|\n", stringP(A));
    printf("B: |%s|\n", stringP(B));
    printf("C: |%s|\n", stringP(C));
//        frac *ABC = find_line_eqtn(A, B),
//            *kb = kx_plusb(ABC),
//            *new_ABC = height_lineP(ABC, C);
//        for(i=0;i<3;i++)
//            printf("ABC[%d]: %s\n", i, stringF(ABC[i]));
//        for(i=0;i<3;i++)
//            printf("new_ABC[%d]: %s\n", i, stringF(new_ABC[i]));
//        for(i=0;i<2;i++)
//            printf("kb[%d]: %s\n", i, stringF(kb[i]));
//    for(i=0;i<2;i++)
//        printf("new_kb[%d]: %s\n", i, stringF(new_kb[i]));
//        printf("A: %d %d\n", checkP_line_kb(kb, A), checkP_line_ABC(ABC, A));
//        printf("B: %d %d\n", checkP_line_kb(kb, B), checkP_line_ABC(ABC, B));
//        printf("C: %d %d\n", checkP_line_kb(kx_plusb(new_ABC), C), checkP_line_ABC(new_ABC, C));
//        frac box = cos_angle_betw_lines(ABC, new_ABC);
//        float box_fl = frac_to_float(box);
    //printf("cos betw ABC & new_ABC: %s\n", stringF(box));
//        printf("angle betw ABC & new_ABC: acos(%f) = %f\n", box_fl, acos(box_fl)*180/M_PI);
    printf("\tDistance betw AC and B: \t|%f|\n", frac_to_float( dist_lineP( find_line_eqtn(A, C), B) ) );
    printf("\tDistance betw AC and B: \t|%f|\n", frac_to_float( alternative( find_line_eqtn(A, C), B) ) );
}

frac* sub_ect(frac* coeffs1, frac* coeffs2, int length){
    int i;
    for(i=0;i<length;i++){
        printf("SUBcoeff %d: bef %s\t", i, stringF(coeffs1[i]));
        coeffs1[i] = sub_frac(coeffs1[i], coeffs2[i]);
        printf("aft %s\n", stringF(coeffs1[i]));
    }
    return coeffs1;
}

frac* mult_eqt(frac* coeffs, int length, frac num){
    int i;
    printf("NUM: %s\n", stringF(num));
    for(i=0;i<length;i++){
        printf("MULTcoeff %d: bef %s\t", i, stringF(coeffs[i]));
        coeffs[i] = mult_frac(coeffs[i], num);
        printf("aft %s\n", stringF(coeffs[i]));
    }
    return coeffs;
}

Point createP(int x, int y){
    Point box;
    box.x = create_frac(x, 1);
    box.y = create_frac(y, 1);
    return box;
}

char* stringP(Point p){
    char *box_str = malloc((strlen(" ") + 45*2) * sizeof(char));
    sprintf(box_str, "%s %s", stringF(p.x), stringF(p.y));
    return box_str;
}

frac* find_line_eqtn(Point p1, Point p2){
    //(x - p1.x) / (p2.x - p1.x) - (y-p1.y) / (p2.y-p1.y) = 0
    //formula on listochek u me est'
    frac denom = sub_frac(p2.x, p1.x);
    if (denom.num == 0);
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
    printf("chck_ln_ABC: %s %s %s\n", stringF(A), stringF(B), stringF(C));
    box = sum_frac(box, mult_frac(A, x));
    //printf("box1: %s\n", stringF(box));
    box = sum_frac(box, mult_frac(B, y));
    //printf("box2: %s\n", stringF(box));
    box = sum_frac(box, C);
    //printf("box3: %s\n", stringF(box));
    return are_equal_frac(box, create_frac(0, 1));;
}

frac* height_lineP(frac* ABC, Point p){
//    new_k = -k
//    new_b = yp - xp * new_k
    printf("HEIGHT INPUT: |%s %s %s|, %s, %s\n", stringF(ABC[0]), stringF(ABC[1]), stringF(ABC[2]), stringF(p.x), stringF(p.y));
    frac *new_ABC = malloc(3 * sizeof(frac)),
        A = ABC[0],
        B = ABC[1],
        C = ABC[2];
    new_ABC[0] = inverse_frac(B); // new_A
    new_ABC[1] = A; // new_B
    new_ABC[2] = sub_frac( mult_frac(B, p.x), mult_frac(A, p.y) ); // new_C
    return new_ABC;
}

long long int** read_vars(int* vars_num, char* filename){
    FILE *Fvars = fopen(filename, "r");
    if (!Fvars){
        printf("Can't open %s. Program will stopped.\n", filename);
        exit(666);
    }

    int i, j, box_cntr, num_of_vars;
    char vars_cnt[5],
        ch,
        box_str[MAX_NUM_LENGTH];

    *vars_num = atoi( fgets(vars_cnt, 5, Fvars) );
    num_of_vars = *vars_num;
    long long int **vars = malloc(num_of_vars * sizeof(long long int*));
    for(i=0;i<num_of_vars;i++){
        vars[i] =  malloc(6 * sizeof(long long int));
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

frac find_dist_PP(Point p1, Point p2){
    frac diff_x = sub_frac(p2.x, p1.x),
        diff_y = sub_frac(p2.y, p1.y),
        distance;
    distance = sum_frac( mult_frac(diff_x, diff_x), mult_frac(diff_y, diff_y) );
    printf("inTEST %s\n", stringF(distance));
    distance = sqrt_frac( distance );
    printf("find_dist_PP: p1(%s) p2(%s)\t", stringP(p1), stringP(p2));
    printf("%s %s |= %s\n", stringF(diff_x), stringF(diff_y), stringF(distance));
    return distance;
}

float round_fl(float num, int signs){
    num *= (int)(pow(10, signs)+0.1);
    num = floor(num+0.5);
    num /= (int)(pow(10, signs)+0.1);
    return num;
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

frac cos_angle_betw_lines(frac* ABC1, frac* ABC2){
    frac num, denom, result, box1, box2,
        A1 = ABC1[0],
        B1 = ABC1[1],
        C1 = ABC1[2],
        A2 = ABC2[0],
        B2 = ABC2[1],
        C2 = ABC2[2];
    //printf("%s %s %s %s %s %s\n", stringF(A1), stringF(B1), stringF(C1), stringF(A2), stringF(B2), stringF(C2));
    num = sum_frac( mult_frac(A1, A2), mult_frac(B1, B2) );
    //printf("num: %s\n", stringF(num));
    box1 = sum_frac( mult_frac(A1, A1), mult_frac(B1, B1));
    box2 = sum_frac( mult_frac(A2, A2), mult_frac(B2, B2) );
    //printf("box1 %s box2 %s\n", stringF(box1), stringF(box2));
    //printf("|%s %f| |%s %f|\n", stringF(box1), frac_to_float(box1), stringF(box2), frac_to_float(box2));
    denom = mult_frac( box1, box2 );
    //printf("mult denom: %s\n", stringF(denom));
    denom = sqrt_frac(denom);
    //printf("aft sqrt denom: %s\n", stringF(denom));
    result = div_frac(num, denom);
    printf("%s : %s = %s\n", stringF(num), stringF(denom), stringF(result));
    return result;
}
