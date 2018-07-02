#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "mathomatic/lib/mathomatic.h"
#define X2 0
#define XY 1
#define Y2 2
#define X1 3
#define Y1 4
#define NUMBER 5

typedef struct{
    int num;
    int und_rt; //under root
} cff;

int COLS = 6; // size of coefficients' massive

char* ind_to_name(int index);
cff* create_mass_cfs();
void show_mass_cfs(cff* mass_cfs, char* text);
void print_eqt(cff* mass_cfs);
void read_mass_cfs(cff* mass_cfs);

int main(){
    cff* mass_cfs = create_mass_cfs(); // massive of coefficients
    if (!mass_cfs){
        printf("Pointer to 'mass_cfs' is NULL. Program will be stopped.\n");
        exit(EXIT_SUCCESS);
    }
    read_mass_cfs(mass_cfs);
    print_eqt(mass_cfs);
}

cff* create_mass_cfs(){
    cff* mass_cfs = malloc(COLS * sizeof(cff));
    return mass_cfs;
}

void show_mass_cfs(cff* mass_cfs, char* text){
    int i;
    if (text == " ")
        printf("Array of coefficients:\n");
    else
        printf("%s", text);

    for (i=0;i<COLS;i++)
        printf("%s   ", ind_to_name(i));
    for (i=0;i<COLS;i++)
        printf("%d*rt(%d)  ", mass_cfs[i].num, mass_cfs[i].und_rt);
    printf("\n");
}

char* ind_to_name(int index){
    char *box = malloc(10 * sizeof(char));
    switch (index){
    case X2:
        return "X^2";
    case Y2:
        return "Y^2";
    case XY:
        return "XY";
    case X1:
        return "X";
    case Y1:
        return "Y";
    case NUMBER:
        return "number";
    default:
        printf("(ind_to_name)Wrong index: %d\n", index);
        exit(EXIT_FAILURE);
    }
}

void print_eqt(cff* mass_cfs){ // print equation
    int i;
    char rt1[20], rt2[20], var_name[10];
    for (i=0;i<COLS;i++){
        if (mass_cfs[i].num == 0)
            continue;
        if (i == NUMBER)
            strcpy(var_name, "");
        else
            strcpy(var_name, ind_to_name(i));
        if (mass_cfs[i].und_rt != 1){
            strcpy(rt1, "*rt(");
            strcat(rt1, itoa(mass_cfs[i].und_rt, rt2, 10));
            strcat(rt1, ")");
        }
        else
            strcpy(rt1, "");
        printf("%+d%s%s", mass_cfs[i].num, rt1, var_name);
    }
    printf("=0\n");
}

void read_mass_cfs(cff* mass_cfs){
    int i;
    char *var_name,
        cff_for[20];
    for (i=0;i<COLS;i++){
        var_name = ind_to_name(i);
        if (i == NUMBER)
            strcpy(cff_for, "");
        else
            strcpy(cff_for, "coefficient for ");
        printf("Enter %s%s\n", cff_for, var_name);
        scanf("%d", &mass_cfs[i].num);
        if (mass_cfs[i].num == 0)
            continue;
        scanf("%d", &mass_cfs[i].und_rt);
    }
}

