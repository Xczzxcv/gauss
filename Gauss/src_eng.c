#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../my_head.h"

int main(){
    int **ptM = readM();
    int detM;
    if (ptM != NULL){
        show_intM(ptM, N, "");
        if (checking_det(ptM, N) == 0){
            printf("The determinant of this matrix is zero, so it's impossible to find invers of it.\n");
            exit(EXIT_SUCCESS);
        }
        else
            printf("Determinant of the Matrix is not null, so we'll continue...\n");
        printf("HERE WE GO\n\n");
        frac** pfrM = intM_to_fracM(N, ptM);
        extendingM(pfrM, N);
        zeroing(pfrM, N, N*2);
        make_invM(pfrM, N, N*2); //sizes of extended Matrix
        show_fracM(pfrM, N, N, "And finally, we have the Matrix:\n");
    }
    else
        printf("ptM is NULL.\n");
}

