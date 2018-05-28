#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <malloc.h>
#define N 4
#define MAX_NUM_LENGTH 10 //with minus

int** readM();
void showM(int** ptM, int size);
int** cutM(int row, int col, int** ptM);

int main(){
    ptM = readM();
    if (ptM != NULL){
        showM(ptM, N);
    else
        printf("ptM is NULL.\n");
}

int** readM(){
    char answer; //'f' - file or 'c' - console
    printf("Where is Matrix will be taken from?(FILE(f/c)CONSOLE)\n");
    scanf("%c", &answer);
    int **ptM = calloc(N, sizeof(int*));
    int i, j, k, num_cntr = 0;
    if(answer == 'c'){
        for(i=0;i<N;i++){
            ptM[i] = calloc(N, sizeof(int));
            for(j=0;j<N;j++){
                printf("Enter %d %d:", i, j);
                scanf("%d", &ptM[i][j]);
            }
        }
    }
    else if(answer == 'f'){
        printf("Extracting from file...\n");
        char filename[] = "M.txt",
            symb,
            box[MAX_NUM_LENGTH]; //it's char massive for long (more than one symbol) numbers
        int fst_c_of_n = 0; //value of 'j' (pos in string) at first char of current num
        printf("Opening file...\n");
        FILE *FwithM = fopen(filename, "r");
        if (FwithM != NULL){
            printf("Reading from file...\n");
            for(i=0;i<N;i++){
                printf("\tnew calloc %d(i) %d(ptM[i])\n", i, ptM[i]);
                ptM[i] = calloc(N, sizeof(int));
                printf("%d(ptM[i])\n", ptM[i]);
                printf("\tnew string\n");
                num_cntr = 0;
                for(j=0;num_cntr<N;j++){ //N+1 because of \n in the end of every string
                    symb = fgetc(FwithM);
                    //printf("%c|", symb);
                    //printf("%c", symb);
                    printf("new symbol!\n");
                    if ((symb >= 48 && symb <= 57) || symb ==45){
                        if (fst_c_of_n == -1){//45 - '-', 48 - '0', 57 - '9', 10 - '\n'
                            fst_c_of_n = j;
                            printf("fst_c_of_n now is %d(j)\n", j);
                        }
                        printf("bx inst: %c %d(j-fst_c_of_n) %d(j) %d(fst_c_of_n)\n", symb, j-fst_c_of_n, j, fst_c_of_n);
                        box[j-fst_c_of_n] = symb;
                    }
                    else{
                        ptM[i][num_cntr] = atoi(box);
                        printf("atoi ptM: %d %d %d\n", ptM[i][num_cntr], i, num_cntr);
                        num_cntr++;
                        printf("num_cntr++\n");
                        printf("box: ");
                        for (k=0;k<MAX_NUM_LENGTH;k++){
                            printf("%c", box[k]);
                            box[k] = 't';
                        }
                        printf("\n");

                        //printf("|%d|%d|%d - it's data\n", ptM[i][j], i, j);
                        if (symb == '\n')
                            printf("\t\t HI \\n!!!\n");
                        printf("fst_c_of_n -1 on |%d %d|\n", i, j);
                        fst_c_of_n = -1;
                    }
            }
            printf("this (%d) string of M:", i);
            for(k=0;k<N;k++)
                printf(" %d", ptM[i][k]);
            printf("\n");
        }
    }
        else{
            printf("File wasn't opened. We'll stop 'readM'.\n");
            return NULL;
        }
    }
    else{
        printf("Something goes wrong and you enter not 'f' or 'c'. We'll stop 'readM'.\n");
        printf("answer %c\n", answer);
        return NULL;
    }
    return ptM;
}

void showM(int** ptM, int size){
    printf("Showing Matrix...\n\n\n");
    int i, j;
    for(i=0;i<size;i++){
        for(j=0;j<size;j++)
            printf("%d ", ptM[i][j]);
        printf("\n");
    }
}

int** cutM(int row, int col, int** ptM){
    int i, j,
        rec_cntr = 0,
        ctN = N - 1, //cuted N
        **ptcM = calloc(ctN, sizeof(int*)); // pointer to cuted Matrix
    for (i=0;i<N;i++){
        ptcM[i] = calloc(ctN, sizeof(int));
        for(j=0;j<N;j++){
            if (i != row && j != col){
                ptcM[rec_cntr / ctN][rec_cntr % ctN] = ptM[i][j];
                rec_cntr++;
                //printf("%d -- %d %d\n", rec_cntr, i, j);
            }
        }
    }
    return ptcM;
}
//————//——————//—————————//——————————//———//
void func(int **ptM)
{
    int i, j,
        **A = ptM, MASS[N][N];
    int key = 0;
    int key2 = 2;
    for (i = 0; i < N - 1; ++i) {
      for (j = 0; j < N - 1; ++j) {
        if (j < key) {
          if (i < key2) {
            MASS[i][j] = A[i][j];
            printf("%3i ", MASS[i][j]);
          } else {
            MASS[i][j] = A[i + 1][j];
            printf("%3i ", MASS[i][j]);
          }
        } else {
          if (i < key2) {
            MASS[i][j] = A[i][j + 1];
            printf("%3i ", MASS[i][j]);
          } else {
            MASS[i][j] = A[i + 1][j + 1];
            printf("%3i ", MASS[i][j]);
          }
        }
      }
      puts("\n");
    }

    return 0;
}
