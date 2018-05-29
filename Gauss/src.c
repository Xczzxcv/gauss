#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define N 4
#define MAX_NUM_LENGTH 10 //with minus

struct frac{ //fraction - дробь, лол
    int numerator; //числитель
    int denominator; //знаменатель
};

int** readM();
void show_intM(int** ptM, int sizeM);
int** cutM(int row, int col, int** ptM);
int find(int* mass, int size, int quest);
void sum_strM(int str1, int str2, int coeff, int** ptM, int sizeM);
frac** intM_to_fracM(int sizeM, int** ptM);
void show_fracM(frac** pfrM, int sizeM);

int main(){
    int **ptM = readM();
    if (ptM != NULL){
        //sum_strM(0, 1, -2, ptM, N);
        show_intM(ptM, N);
        intM_to_fracM()
    }
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

void show_intM(int** ptM, int sizeM){
    printf("Showing Matrix of integers...\n\n\n");
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

int find(int* mass, int size, int quest){
    int i;
    for(i = 0; i<size;i++)
        if (mass[i] == quest)
            return i;
    return -1;
}

void sum_strM(int str1, int str2, int coeff, int** ptM, int sizeM){

frac** intM_to_fracM(int sizeM, int** ptM){
    int i, j;
    frac **ptfrM = calloc(sizeM, sizeof(frac*));
    for (i=0;i<sizeM;i++)
        for (j=0;j<sizeM;j++){
            ptfrM[i][j].numerator = ptM[i][j];
            pfrM[i][j].denominator = 1;
        }
    return ptfrM;

void show_fracM(frac** pfrM, int sizeM){
    printf("Showing Matrix of fractions...\n\n\n");
    int i, j;
    for(i=0;i<sizeM;i++){
        for(j=0;j<sizeM;j++)
            printf("d%/d ", pfrM[i][j].numerator, pfrM[i][j].denominator);
        printf("\n");
    }
}

    int i, j;
    frac **pfrM = calloc(sizeM, sizeof(frac*)); // p_frM - pointer to fraction Matrix
    for (i=0;i<sizeM;i++){
        pfrM[i] = calloc(sizeM, sizeof(frac));
        for(j=0;j<sizeM;j++){
            pfrM[i][j].denominator = ptM[i][j];
            pfrM[i][j].numerator = 1;
        }
    }
}
    int i;
    for (i=0;i<sizeM;i++)
        ptM[str1][i] += ptM[str2][i] * coeff;
}
