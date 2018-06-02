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
void show_intM(int** ptM, int sizeM, char* text);
int** cutM(int row, int col, int** ptM);
int find(int* mass, int size, int quest);
void sum_strM(int str1, int str2, frac coeff, frac** pfrM, int sizestr);
frac** intM_to_fracM(int sizeM, int** ptM);
void show_fracM(frac** pfrM, int rows, int cols, char* text);
void extendingM(frac** pfrM, int sizeM);
void zeroing(frac** pfrM, int rows, int cols); //size - num of rows
int eukl_NOD(int a, int b);
void swap_strM(int str1, int str2, frac** pfrM);
void reduc_frac(frac** frac_ptr_mass, int mass_size); //сокращение дробей
void make_invM(frac** pfrM, int rows, int cols);
frac sum_frac(frac fr1, frac fr2);
frac div_frac(frac fr1, frac fr2);
frac mult_frac(frac fr1, frac fr2);
frac create_frac(int num, int denom);
void checking_det_det(int** ptM, int sizeM);

int main(){
    setlocale(LC_ALL, "Russian");
    int **ptM = readM();
    int detM;
    if (ptM != NULL){
        show_intM(ptM, N, "");
        if (checking_det(ptM, N) == 0){
            printf("Определитель введённой Матрицы равен нулю, следовательно невозможно найти обратную матрицу.\n");
            exit(EXIT_SUCCESS);
        }
        else
            printf("Детерминант матрицы не равен нулю, продолжаем...\n");
        printf("HERE WE GO\n\n");
        frac** pfrM = intM_to_fracM(N, ptM);
        extendingM(pfrM, N);
        //show_fracM(pfrM, N, N*2, "Перед началом:");
        zeroing(pfrM, N, N*2);
        make_invM(pfrM, N, N*2); //sizes of extended Matrix
        show_fracM(pfrM, N, N, "В конце получается:\n");
    }
    else
        printf("ptM is NULL.\n");
    //printf("в конце составить массив всех знаменателей\n и найти общий (дополнительно домножив нужные дроби/строки),\n затем вынести его перед матрицей\n");
}

int** readM(){
    char answer; //'f' - file or 'c' - console
    printf("Откуда взять Матрицу? (Файл(f/c)Консоль)\n");
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
        printf("Импорт из файла:\n");
        char filename[] = "M.txt",
            symb,
            box[MAX_NUM_LENGTH]; //it's char massive for long (more than one symbol) numbers
        int fst_c_of_n = 0; //value of 'j' (pos in string) at first char of current num
        printf("\tОткрытие файла...\n");
        FILE *FwithM = fopen(filename, "r");
        if (FwithM != NULL){
            printf("\tЧтение из файла...\n");
            for(i=0;i<N;i++){
                //printf("\tnew calloc %d(i) %d(ptM[i])\n", i, ptM[i]);
                ptM[i] = calloc(N, sizeof(int));
                //printf("%d(ptM[i])\n", ptM[i]);
                //printf("\tnew string\n");
                num_cntr = 0;
                for(j=0;num_cntr<N;j++){ //N+1 because of \n in the end of every string
                    symb = fgetc(FwithM);
                    //printf("new symbol!\n");
                    if ((symb >= 48 && symb <= 57) || symb ==45){
                        if (fst_c_of_n == -1){//45 - '-', 48 - '0', 57 - '9', 10 - '\n'
                            fst_c_of_n = j;
                            //printf("fst_c_of_n now is %d(j)\n", j);
                        }
                        //printf("bx inst: %c %d(j-fst_c_of_n) %d(j) %d(fst_c_of_n)\n", symb, j-fst_c_of_n, j, fst_c_of_n);
                        box[j-fst_c_of_n] = symb;
                    }
                    else{
                        ptM[i][num_cntr] = atoi(box);
                        //printf("atoi ptM: %d %d %d\n", ptM[i][num_cntr], i, num_cntr);
                        num_cntr++;
                        //printf("num_cntr++\n");

                        //printf("box: ");
                        for (k=0;k<MAX_NUM_LENGTH;k++){
                            //printf("%c", box[k]);
                            box[k] = 't';
                        }
                        //printf("\n");

                        fst_c_of_n = -1;
                    }
            }
            /*
            printf("this (%d) string of M:", i);
            for(k=0;k<N;k++)
                printf(" %d", ptM[i][k]);
            printf("\n");
            */
        }
    }
        else{
            printf("Файл не был открыт. Остановка 'readM'.\n");
            return NULL;
        }
    }
    else{
        printf("Что-то пошло не так, потому что Вы ввели не 'f' и не 'c'. 'readM' будет остановлена.\n");
        printf("answer %c\n", answer);
        return NULL;
    }
    return ptM;
}

void show_intM(int** ptM, int sizeM, char* text){
    if (text == "")
        printf("\nМатрица целых чисел:\n");
    else{
        printf("%s", text);
    }
    int i, j;
    for(i=0;i<sizeM;i++){
        for(j=0;j<sizeM;j++)
            printf("%d ", ptM[i][j]);
        printf("\n");
    }
    printf("\n");
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

int checking_det(int** ptM, int sizeM){
    int i, j, cff, box,
        det = 0,
        minor = 0;
    if(sizeM != 2){
        for(j=0;j<sizeM;j++){
            if ((i+j) % 2)
                cff = -1;
            else
                cff = 1;
            minor = checking_det(cutM(0, j, ptM), sizeM-1);
            box = det;
            det += cff * ptM[0][j] * minor;
            //printf("%d\t%d\t%d = %d -> %d\n", ptM[0][j], cff, minor, box, det);
            //show_intM(ptM, sizeM, " ");
        }
    }
    else{
        det = ptM[0][0] * ptM[1][1] - ptM[0][1] * ptM[1][0];
        //printf("%d\tnea\t%d\n", sizeM, det);
        //show_intM(ptM, sizeM, " ");
    }
    return det;
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

void show_fracM(frac** pfrM, int rows, int cols, char* text){
    if (text == "")
        printf("\nМатрица дробей:\n");
    else{
        printf("%s", text);
    }
    int i, j;
    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++)
            printf("%d/%d ", pfrM[i][j].num, pfrM[i][j].denom);
        printf("\n");
    }
    printf("\n");
}

void sum_strM(int str1, int str2, frac coeff, frac** pfrM, int sizestr){
    //printf("sum %d and %d\ncoeff %d/%d sizestr %d\n", str1, str2, coeff.num, coeff.denom, sizestr);
    //show_fracM(pfrM, N, sizestr);
    int i;
    for (i=0;i<sizestr;i++){
        //printf("BFR: %d/%d and %d/%d ", pfrM[str1][i].num, pfrM[str1][i].denom, pfrM[str2][i].num, pfrM[str2][i].denom);
        pfrM[str1][i] = sum_frac(pfrM[str1][i], div_frac(pfrM[str2][i], coeff));
        //printf("AFR: %d/%d and %d/%d\n", pfrM[str1][i].num, pfrM[str1][i].denom, pfrM[str2][i].num, pfrM[str2][i].denom);
    }
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

int eukl_NOD(int a, int b){
    a = abs(a);
    b = abs(b);
    while (a != 0 && b != 0){
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }
    return a + b;
}

void swap_strM(int str1, int str2, frac** pfrM){
    frac *box = pfrM[str1];
    pfrM[str1] = pfrM[str2];
    pfrM[str2] = box;
}

void reduc_frac(frac** frac_ptr_mass, int mass_size){
    int i, NOD;
    for (i=0;i<mass_size;i++){
        NOD = eukl_NOD(frac_ptr_mass[i][0].num, frac_ptr_mass[i][0].denom);
        frac_ptr_mass[i][0].num /= NOD;
        frac_ptr_mass[i][0].denom /= NOD;
    }
}

frac sum_frac(frac fr1, frac fr2){
    if (fr1.denom == fr2.denom)
        fr1.num += fr2.num;
    else{
        fr1.num = fr1.num * fr2.denom + fr2.num * fr1.denom;
        fr1.denom *= fr2.denom;
    }
    frac* mass[] = {&fr1};
    reduc_frac(mass, 1);
    return fr1;
}

frac div_frac(frac fr1, frac fr2){
    if (fr2.num == 0){
        printf("Деление на ноль.\n");
        exit(EXIT_FAILURE);
    }
    fr1.num *= fr2.denom;
    fr1.denom *= fr2.num;
    frac* mass[] = {&fr1};
    reduc_frac(mass, 1);
    if(fr1.denom < 0){
        fr1.num = -fr1.num;
        fr1.denom = -fr1.denom;
    }
    return fr1;
}

frac mult_frac(frac fr1, frac fr2){
    fr1.num *= fr2.num;
    fr1.denom *= fr2.denom;
    frac* mass[] = {&fr1};
    reduc_frac(mass, 1);
    return fr1;
}

frac create_frac(int num, int denom){
    frac box;
    box.num = num;
    box.denom = denom;
    return box;
}

void zeroing(frac** pfrM, int rows, int cols){
    int i, j, k;
    frac cff;

    vse_zanovo:
    for (i=0;i<cols/2;i++){
        if (pfrM[i][i].num == 0)
            for (k=0;k<rows;k++)
                if /*(pfrM[k][k].num != 0 &&*/ (pfrM[k][i].num!= 0){
                    //sum_strM(i, k, create_frac(2, 1), pfrM, cols);
                    swap_strM(i, k, pfrM);
                    printf("Меняем местами %d и %d строки.\n", k+1, i+1);
                    show_fracM(pfrM, rows, cols, "После лечебного обмена:\n\n");
                    //getch();
                    goto vse_zanovo;
                }
        for (j=0;j<rows;j++){
        //printf("RwCl %d %d\n", j, i);
            if (j != i){
                //printf("BFRCFF %d/%d %d/%d\n", pfrM[i][i].num, pfrM[i][i].denom, pfrM[j][i].num, pfrM[j][i].denom);
                if (pfrM[j][i].num == 0)
                    continue;
                cff = div_frac(pfrM[i][i], pfrM[j][i]);
                printf("Вычитаем %d из %d строки с коэффициентом %d/%d\n", j+1, i+1, cff.num, cff.denom);
                cff.num = -cff.num; //for vychitanie strok
                sum_strM(j, i, cff, pfrM, cols);
                show_fracM(pfrM, rows, cols, "Результат:\n");
            }
        }
    }

    for(i=0;i<rows;i++){
        cff = div_frac(create_frac(1, 1), pfrM[i][i]);
        for (j=0;j<cols;j++)
            pfrM[i][j] = mult_frac(pfrM[i][j], cff);
    }
}

void make_invM(frac** pfrM, int rows, int cols){
    int i, j,
        true_size = cols/2;
    frac *box;
    for (i=0;i<rows;i++){
        box = malloc(true_size * sizeof(frac));
        for (j=true_size;j<cols;j++)
            box[j-true_size] = pfrM[i][j];
        pfrM[i] = box;
    }
}
