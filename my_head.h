#define MAX_NUM_LENGTH 8 //with minus
#define N 3


typedef struct {
    long long int num; //числитель
    long long int denom; //знаменатель
}frac; //fraction - дробь, лол

int** readM();
void show_intM(int** ptM, int rows, int cols, char* text);
int** cutM(int row, int col, int** ptM);
int find(int* mass, int size, int quest);
void sum_strM(int str1, int str2, frac coeff, frac** pfrM, int sizestr);
frac** intM_to_fracM(int sizeM, int** ptM);
void show_fracM(frac** pfrM, int rows, int cols, char* text);
void extendingM(frac** pfrM, int sizeM);
void zeroing(frac** pfrM, int rows, int cols); //size - num of rows
int eukl_NOD(int a, int b);
void swap_strM(int str1, int str2, frac** pfrM);
void reduc_frac(frac* frac_ptr); //сокращение дробей
void make_invM(frac** pfrM, int rows, int cols);
frac sum_frac(frac fr1, frac fr2);
frac sub_frac(frac fr1, frac fr2);
frac div_frac(frac fr1, frac fr2);
frac mult_frac(frac fr1, frac fr2);
frac modulo_frac(frac fr);
frac sqrt_frac(frac fr);
int are_equal_frac(frac fr1, frac fr2);
frac inverse_frac(frac fr);
frac create_frac(long long int num, long long int denom);
int checking_det(int** ptM, int sizeM);
void zero_issue(int problem_str, frac** pfrM, int rows, int cols);
int** try_fracM_to_intM(frac** pfrM, int rows, int cols);
char* stringF(frac fr);
float frac_to_float(frac fr);
frac float_to_frac(float num);

int** readM(){
    char answer; //'f' - file or 'c' - console
    printf("Where is Matrix will be taken from?(FILE(f/c)CONSOLE)\n");
    scanf("%c", &answer);
    int **ptM = malloc(N * sizeof(int*));
    int i, j, k, num_cntr = 0;
    if(answer == 'c'){
        for(i=0;i<N;i++){
            ptM[i] = malloc(N * sizeof(int));
            for(j=0;j<N;j++){
                printf("Enter %d %d:", i, j);
                scanf("%d", &ptM[i][j]);
            }
        }
    }
    else if(answer == 'f'){
        printf("Extracting from file:\n");
        char filename[] = "M.txt",
            symb,
            box[MAX_NUM_LENGTH]; //it's char massive for long (more than one symbol) numbers
        int fst_c_of_n = 0; //value of 'j' (pos in string) at first char of current num
        printf("\tOpening file...\n");
        FILE *FwithM = fopen(filename, "r");
        if (FwithM != NULL){
            printf("\tReading from file...\n");
            for(i=0;i<N;i++){
                ptM[i] = malloc(N * sizeof(int));
                num_cntr = 0;
                for(j=0;num_cntr<N;j++){ //N+1 because of \n in the end of every string
                    symb = fgetc(FwithM);
                    if ((symb >= 48 && symb <= 57) || symb ==45){
                        if (fst_c_of_n == -1)//45 - '-', 48 - '0', 57 - '9', 10 - '\n'
                            fst_c_of_n = j;
                        box[j-fst_c_of_n] = symb;
                    }
                    else{
                        ptM[i][num_cntr] = atoi(box);
                        num_cntr++;

                        for (k=0;k<MAX_NUM_LENGTH;k++)
                            box[k] = 't';

                        fst_c_of_n = -1;
                    }
            }
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

int** cutM(int row, int col, int** ptM){
    int i, j,
        rec_cntr = 0,
        ctN = N - 1, //cuted N
        **ptcM = malloc(ctN * sizeof(int*)); // pointer to cuted Matrix
    for (i=0;i<N;i++){
        ptcM[i] = malloc(ctN * sizeof(int));
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
        }
    }
    else
        det = ptM[0][0] * ptM[1][1] - ptM[0][1] * ptM[1][0];
    return det;
}

frac** intM_to_fracM(int sizeM, int** ptM){
    int i, j;
    frac **pfrM = malloc(sizeM * sizeof(frac*));
    for (i=0;i<sizeM;i++){
        pfrM[i] = malloc(sizeM * sizeof(frac));
        for (j=0;j<sizeM;j++){
            pfrM[i][j].num = ptM[i][j];
            pfrM[i][j].denom = 1;
        }
    }
    return pfrM;
}

void show_fracM(frac** pfrM, int rows, int cols, char* text){
    if (text == "")
        printf("\nThe Matrix of fractions:\n");
    else{
        printf("%s", text);
    }
    int i, j;
    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++)
            printf("%s ", stringF(pfrM[i][j]));
        printf("\n");
    }
    printf("\n");
}

void sum_strM(int str1, int str2, frac coeff, frac** pfrM, int sizestr){
    int i;
    for (i=0;i<sizestr;i++)
        pfrM[str1][i] = sum_frac(pfrM[str1][i], mult_frac(pfrM[str2][i], coeff));
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

void reduc_frac(frac* frac_ptr){
    int NOD = eukl_NOD( frac_ptr->num, frac_ptr->denom );
    frac_ptr->num /= NOD;
    frac_ptr->denom /= NOD;
}

frac sum_frac(frac fr1, frac fr2){
    if (fr1.denom == fr2.denom)
        fr1.num += fr2.num;
    else{
        fr1.num = fr1.num * fr2.denom + fr2.num * fr1.denom;
        fr1.denom *= fr2.denom;
    }
    reduc_frac(&fr1);
    return fr1;
}

frac sub_frac(frac fr1, frac fr2){
    if (fr1.denom == fr2.denom)
        fr1.num -= fr2.num;
    else{
        fr1.num = fr1.num * fr2.denom - fr2.num * fr1.denom;
        fr1.denom *= fr2.denom;
    }
    reduc_frac(&fr1);
    return fr1;
}

frac div_frac(frac fr1, frac fr2){
    if (fr2.num == 0){
        printf("Divison by zero at %s %s.\n", stringF(fr1), stringF(fr2));
        exit(EXIT_FAILURE);
    }
    fr1.num *= fr2.denom;
    fr1.denom *= fr2.num;
    reduc_frac(&fr1);
    if(fr1.denom < 0){
        fr1.num = -fr1.num;
        fr1.denom = -fr1.denom;
    }
    return fr1;
}

frac mult_frac(frac fr1, frac fr2){
    fr1.num *= fr2.num;
    fr1.denom *= fr2.denom;
    reduc_frac(&fr1);
    return fr1;
}

frac inverse_frac(frac fr){
    fr.num = -fr.num;
    return fr;
}

frac create_frac(long long int num, long long int denom){
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
        if (pfrM[i][i].num == 0){
            zero_issue(i, pfrM, rows, cols);
            goto vse_zanovo;
        }
        for (j=0;j<rows;j++){
            if (j != i){
                if (pfrM[j][i].num == 0)
                    continue;
                cff = div_frac(pfrM[j][i], pfrM[i][i]);
                printf("We're subtracting %d string from %d (%d string was multiplied by %s)\n", i+1, j+1, i+1, stringF(cff));
                cff.num = -cff.num; //for vychitanie strok
                sum_strM(j, i, cff, pfrM, cols);
                show_fracM(pfrM, rows, cols, "Result:\n");
            }
        }
    }

    for(i=0;i<rows;i++){
        cff = div_frac(create_frac(1, 1), pfrM[i][i]);
        printf("We multiplyed %d string by %s\n", i+1, stringF(cff));
        for (j=0;j<cols;j++)
            pfrM[i][j] = mult_frac(pfrM[i][j], cff);
        show_fracM(pfrM, rows, cols, "Result:\n");
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

void zero_issue(int problem_str, frac** pfrM, int rows, int cols){
    int k;
    for (k=0;k<rows;k++){
        if (pfrM[k][problem_str].num != 0 && pfrM[problem_str][k].num != 0){
            swap_strM(problem_str, k, pfrM);
            printf("We just have swaped %d и %d strings.\n", k+1, problem_str+1);
            show_fracM(pfrM, rows, cols, "The Matrix after the swap:\n");
        }
    }
}

int** try_fracM_to_intM(frac** pfrM, int rows, int cols){
    int i, j;
    int **box = malloc(rows * sizeof(int*));
    for(i=0;i<rows;i++){
        box[i] = malloc(cols * sizeof(int));
        for(j=0;j<cols;j++){
            if(pfrM[i][j].denom != 1)
                return NULL;
            box[i][j] = pfrM[i][j].num;
        }
    }
    return box;
}

char* stringF(frac fr){
    if(fr.denom == 1){
        char * box_str = malloc(12 * sizeof(char));
        sprintf(box_str, "%lli", fr.num);
    }
    else{
        char* box_str = malloc(22*2 * sizeof(char));
        sprintf(box_str, "%lli/%lli", fr.num, fr.denom);
    }
    return box_str;
}

float frac_to_float(frac fr){
    return (float)fr.num / fr.denom;
}

int are_equal_frac(frac fr1, frac fr2){
    return (fr1.num == fr2.num) && (fr1.denom == fr2.denom);
}

frac sqrt_frac(frac fr){
    float box_num = (float)fr.num,
        box_denom = (float)fr.denom;
    box_num = sqrt(box_num);
    box_denom = sqrt(box_denom);
    fr = div_frac( float_to_frac(box_num), float_to_frac(box_denom) );
    return fr;
}

frac float_to_frac(float num){
    if (num == (int)num)
        return create_frac((int)num, 1);

    int int_part = (int)num,
        cnt = 0;
    float float_part = num - (int)num,
        prev;
    frac box_fr;
    do{
        prev = float_part;
        float_part *= 10;
        cnt++;
    }while(float_part != (int)float_part);
    box_fr = create_frac((int)float_part, (int)(pow(10, cnt)+0.1));
    box_fr = sum_frac( box_fr, create_frac(int_part, 1) );
    return box_fr;
}

frac modulo_frac(frac fr){
    if (fr.num < 0 || fr.denom <0)
        return inverse_frac(fr);
    else
        return fr;
}
