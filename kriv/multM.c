#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 4

int** create_randM(int rows, int cols, int from, int to);
int** multiply_matrix(int **A, int **B, int rowsA, int colsA, int rowsB, int colsB);
void show_intM(int** ptM, int rows, int cols, char* text);

int main(){
    srand(time(NULL));
    int **A = create_randM(N, N, -10, 10),
        **B = create_randM(N, 1, -10, 10);
    show_intM(A, 1, N, "");
    show_intM(B, N, 1, "");
    int **resM = multiply_matrix(A, B, 1, N, N, 1);
    show_intM(resM, 1, 1, "");
    return 0;
}

int** multiply_matrix (int **A, int **B, int rowsA, int colsA, int rowsB, int colsB) {
    if (colsA != rowsB){
        printf("'colsA' != 'rowsB'. Sry\n");
        exit(EXIT_FAILURE);
    }
    int i, j, cycle,
        sum;
    int **C = create_randM(rowsA, colsB, 0, 0);
    for(i = 0; i < rowsA; ++i){
        for(j = 0; j < colsB; ++j){
            //sum = 0;
            for(cycle = 0, sum = 0; cycle < colsA; cycle++)
                sum += A[i][cycle] * B[cycle][j];
            C[i][j] = sum;
        }
    }
    return C;
}

int** create_randM(int rows, int cols, int from, int to){
    int i, j;
    int **M = malloc(rows * sizeof(int*));
    for (i=0;i<rows;i++){
        M[i] = malloc(cols * sizeof(int));
        for(j=0;j<cols;j++)
            M[i][j] = (rand() % (to-from + 1)) + from;
    }
    return M;
}

void show_intM(int** ptM, int rows, int cols, char* text){
    if (text == "")
        printf("\nThe Matrix of integers:\n");
    else{
        printf("%s", text);
    }
    int i, j;
    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++)
            printf("%d ", ptM[i][j]);
        printf("\n");
    }
    printf("\n");
}
