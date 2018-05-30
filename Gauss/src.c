#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define N 4
#define MAX_NUM_LENGTH 10 //with minus

typedef struct {
    int num; //числитель
    int denom; //знаменатель
}frac; //fraction - дробь, лол

int** readM();
void show_intM(int** ptM, int sizeM);
int** cutM(int row, int col, int** ptM);
int find(int* mass, int size, int quest);
void sum_strM(int str1, int str2, int coeff, int** ptM, int sizeM);
frac** intM_to_fracM(int sizeM, int** ptM);
void show_fracM(frac** pfrM, int rows, int cols);
void extendingM(frac** pfrM, int sizeM);
void zeroing(frac** pfrM, int rows, int cols); //size - num of rows
int eukl_NOD(int a, int b);
void swap_strM(frac** pfrM, int str1, int str2);
void frac_reduc(frac** frac_ptr_mass, int mass_size); //сокращение дробей
frac sum_frac(frac fr1, frac fr2);

int main(){
    int **ptM = readM();
    if (ptM != NULL){
        //sum_strM(0, 1, -2, ptM, N);
        show_intM(ptM, N);
        frac** pfrM = intM_to_fracM(N, ptM);
        extendingM(pfrM, N);
        frac A, B;
        A.num = 14323;
        A.denom = 164577;
        B.num = 51234;
        B.denom = 444444;
        A = sum_frac(A, B);
        printf("exprmt %d/%d\n", A.num, A.denom);
        zeroing(pfrM, N, N*2);
        show_fracM(pfrM, N, N*2);
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
    for(i=0;i<sizeM;i++){
        for(j=0;j<sizeM;j++)
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

frac** intM_to_fracM(int sizeM, int** ptM){
    int i, j;
    frac **pfrM = calloc(sizeM, sizeof(frac*));
    for (i=0;i<sizeM;i++){
        pfrM[i] = calloc(sizeM, sizeof(frac));
        for (j=0;j<sizeM;j++){
            pfrM[i][j].num = ptM[i][j];
            pfrM[i][j].denom = 1;
        }
    }
    return pfrM;
}

void show_fracM(frac** pfrM, int rows, int cols){
    printf("Showing Matrix of fractions...\n\n\n");
    int i, j;
    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++)
            printf("%d/%d ", pfrM[i][j].num, pfrM[i][j].denom);
        printf("\n");
    }
}

void sum_strM(int str1, int str2, int coeff, int** ptM, int sizeM){
    int i;
    for (i=0;i<sizeM;i++)
        ptM[str1][i] += ptM[str2][i] * coeff;
}

void extendingM(frac** pfrM, int sizeM){
    int i, j;
    for (i=0;i<sizeM;i++){
        pfrM[i] = realloc(pfrM[i], sizeM*2 * sizeof(frac));
        for (j=0;j<sizeM*2;j++){
            if (j >= sizeM)
                if (i+sizeM == j){
                    pfrM[i][j].num = 1;
                    pfrM[i][j].denom = 1;
                }
                else{
                    pfrM[i][j].num = 0;
                    pfrM[i][j].denom = 1;
                }
        }
    }
}

void zeroing(frac** pfrM, int rows, int cols){
    int i, j, IT = -1;
    for (i=0;i<rows;i++){
        if (pfrM[i][0].num == 1){
            printf("IT %d\n", i);
            IT = i;
            break;
        }
    }
    if (IT != -1 && IT != 0)
        swap_strM(pfrM, 0, IT);
    else if (IT == -1){
        for(i=1;i<rows;i++)
            if(eukl_NOD(pfrM[i][0].num, pfrM[0][0].num) == pfrM[0][0].num)
                IT = i;
        if (IT != -1)
            ;//sum_strM(0, IT, pfrM[IT][0].num);
    }
    int coeff = pfrM[0][0].num;
    for (i=0;i<cols;i++){
        pfrM[0][i].denom *= coeff;

    for (i=1;i<rows;i++){
        for (j=0;j<cols;j++)
            pfrM[i][j].denom = 666;
    }

//for (j=0;j<sizeM;j++)


    }
}

int eukl_NOD(int a, int b){
    while (a != 0 && b != 0){
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }
    return a + b;
}

void swap_strM(frac** pfrM, int str1, int str2){
    frac *box = pfrM[str1];
    pfrM[str1] = pfrM[str2];
    pfrM[str2] = box;
}

void frac_reduc(frac** frac_ptr_mass, int mass_size){
    int i, NOD;
    for (i=0;i<mass_size;i++){
        printf("red input: %d/%d\t", frac_ptr_mass[i][0].num, frac_ptr_mass[i][0].denom);
        NOD = eukl_NOD(frac_ptr_mass[i][0].num, frac_ptr_mass[i][0].denom);
        frac_ptr_mass[i][0].num /= NOD;
        frac_ptr_mass[i][0].denom /= NOD;
        printf("result: %d/%d\n", frac_ptr_mass[i][0].num, frac_ptr_mass[i][0].denom);
    }
}

frac sum_frac(frac fr1, frac fr2){
    frac* mass[] = {&fr1, &fr2};
    frac_reduc(mass, 2);
    printf("test %d %d\n", fr1.denom, fr2.denom);
    if (fr1.denom == fr2.denom)
        fr1.num += fr2.num;
    else{
        fr1.num = fr1.num * fr2.denom + fr2.num * fr1.denom;
        fr1.denom *= fr2.denom;
    }
    return fr1;
}
