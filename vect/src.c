#include <stdlib.h>
#include <stdio.h>
#include "../my_head.h"
#define print_vector(V_name, V) printf("%s: %d %d %d\n", V_name, V->x, V->y, V->z);
#define N 3

typedef struct{
    int x;
    int y;
    int z;
} vector;

vector* create_vector(int x, int y, int z);
int isCoplanar(vector *a, vector *b, vector *c); //return not zero if vectors a, b, c are coplanar
int isBasis(vector *a, vector *b, vector *c); //return not zero if vectors a, b, c form basis
int vect_mixed_mult(vector* a, vector* b, vector* c);
int** get_vectsM(vector* a, vector* b, vector* c);

int main(){
    vector *a = create_vector(-2, -2, -4);
    vector *b = create_vector(5, -1, 5);
    vector *c = create_vector(3, 2, 3);
    printf("Vectors:\n");
    print_vector("    a", a)
    print_vector("    b", b)
    print_vector("    c", c)
    int cplt = isCoplanar(a, b, c);
    if (cplt)
        printf("Vectors a, b, c are complanar.\n");
    else
        printf("Vectors are NOT complanar.\n");
    int basis = isBasis(a, b, c);
    if (basis)
        printf("Vectors a, b, c form basis.\n");
    else
        printf("Vectors NOT form basis.\n");
}

vector* create_vector(int x, int y, int z){
    vector *box = malloc(sizeof(vector));
    box->x = x;
    box->y = y;
    box->z = z;
    return box;
}

int isCoplanar(vector *a, vector *b, vector *c){
    return vect_mixed_mult(a, b, c);
}

int isBasis(vector *a, vector *b, vector *c){
    return vect_mixed_mult(a, b, c);
}

int vect_mixed_mult(vector* a, vector* b, vector* c){
    int detM,
        **vectsM = get_vectsM(a, b, c);
    detM = checking_det(vectsM, 3);
    return detM;
}

int** get_vectsM(vector* a, vector* b, vector* c){
    int i, j;
    int **ptM = malloc(3 * sizeof(int*));
    for(i=0;i<3;i++)
        ptM[i] = malloc(3 * sizeof(int));
    ptM[0][0] = a->x;
    ptM[1][0] = a->y;
    ptM[2][0] = a->z;
    ptM[0][0] = b->x;
    ptM[1][1] = b->y;
    ptM[2][1] = b->z;
    ptM[0][2] = c->x;
    ptM[1][2] = c->y;
    ptM[2][2] = c->z;
    return ptM;
}
