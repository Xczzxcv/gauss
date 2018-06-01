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
void sum_strM(int str1, int str2, frac coeff, frac** pfrM, int sizestr);
frac** intM_to_fracM(int sizeM, int** ptM);
void show_fracM(frac** pfrM, int rows, int cols);
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
void checking(int** ptM, int sizeM);

int main(){
    int **ptM = readM();
    setlocale(LC_ALL, "Russian");
    if (ptM != NULL){
        show_intM(ptM, N);
        checking(ptM, N);
        printf("HERE WE GO\n");
        frac** pfrM = intM_to_fracM(N, ptM);
        printf("CONVERTED\n");
        extendingM(pfrM, N);
        printf("EXTENDED\n");
        show_fracM(pfrM, N, N*2);
        zeroing(pfrM, N, N*2);
        show_fracM(pfrM, N, N*2);
        make_invM(pfrM, N, N*2); //sizes of extended Matrix
        show_fracM(pfrM, N, N);
    }
    else
        printf("ptM is NULL.\n");
        printf("в конце составить массив всех знаменателей\n и найти общий (дополнительно домножив нужные дроби/строки),\n затем вынести его перед матрицей\n");
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

                        /*
                        if (symb == '\n')
                            printf("\t\t HI \\n!!!\n");
                        printf("fst_c_of_n -1 on |%d %d|\n", i, j);
                        */
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
    printf("\nShowing Matrix of integers...\n\n");
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

void checking(int** ptM, int sizeM){
    int i, j,
        detector_x = 0,
        detector_y = 0;
    for (i=0;i<sizeM;i++){
        for (j=0;j<sizeM;j++){
            if (ptM[i][j] == 0)
                detector_x++;
            if (ptM[j][i] == 0)
                detector_y++;
        }
        if (detector_x == sizeM || detector_y == sizeM){
            printf("The determinant of this matrix is zero, so it's impossible to find invers of it.\n");
            exit(EXIT_SUCCESS);
        }
        detector_x = 0,
        detector_y = 0;
    }
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
    printf("\nShowing Matrix of fractions...\n\n");
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
        printf("Divison by zero.\n");
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
    int i, j, k,
        detector_y = 0;

    /*
    for (i=0;i<rows;i++){
        if (pfrM[i][0].num == 1){
            IT = i;
            printf("IT1 %d\n", i);
            break;
        }
    }
    if (IT != -1 && IT != 0){
        printf("swap start ");
        swap_strM(pfrM, 0, IT);
        printf("swap end\n");
    }
    else if (IT == -1){
        for(i=1;i<rows;i++)
            if((eukl_NOD(pfrM[i][0].num, pfrM[0][0].num) == pfrM[0][0].num) && (pfrM[i][0].num != -pfrM[0][0].num)){
                IT = i;
                printf("IT2 %d\n", IT);
            }
        printf("SHa sum budet OOOO\n");
        if (IT != -1)
            if (pfrM[IT][0].num > pfrM[0][0].num)
                sum_strM(0, IT, div_frac(pfrM[IT][0], pfrM[0][0]), pfrM, N*2);
            else if (pfrM[IT][0].num < pfrM[0][0].num)
                sum_strM(0, IT, div_frac(pfrM[0][0], pfrM[IT][0]), pfrM, N*2);
            else{
                printf("IT'S START OF END\n");
                printf("Smth goes wrong.\n [IT][0]num %d IT %d\n[0][0]num %d\n", pfrM[IT][0].num, IT, pfrM[0][0].num);
                exit(EXIT_FAILURE);
            }
        printf("VSE(((99\n");
    }

    if (pfrM[i][i].num/pfrM[i][i].denom != 1){
        int coeff = pfrM[i][i].num;
        printf("coeff 1/%d\n", coeff);
        frac box;
        box.denom = 1;
        box.num = coeff;
        for (i=0;i<cols;i++){
            printf("withBOX before %d/%d", pfrM[0][i].num, pfrM[0][i].denom);
            pfrM[0][i] = div_frac(pfrM[0][i], box);
            printf("  after %d/%d\n", pfrM[0][i].num, pfrM[0][i].denom);
        }
    }
    */

    frac cff;
    vse_zanovo:
    for (i=0;i<cols/2;i++){
        if (pfrM[i][i].num == 0)
            for (k=0;k<rows;k++)
                if (pfrM[k][k].num != 0 && pfrM[k][i].num!= 0){
                    swap_strM(i, k, pfrM);
                    goto vse_zanovo;
                }
        for (j=0;j<rows;j++){
        printf("RwCl %d %d\n", j, i);
            if (j != i){
                printf("BFRCFF %d/%d %d/%d\t", pfrM[i][i].num, pfrM[i][i].denom, pfrM[j][i].num, pfrM[j][i].denom);
                if (pfrM[j][i].num == 0)
                    continue;
                cff = div_frac(pfrM[i][i], pfrM[j][i]);
                printf("CFF %d/%d\n", cff.num, cff.denom);
                cff.num = -cff.num; //for vychitanie strok
                sum_strM(j, i, cff, pfrM, N*2);
                show_fracM(pfrM, N, N*2);
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
