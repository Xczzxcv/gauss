#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_EXPRESSION_LENGTH 10

//раскрытие скобок
//приведение подобных
char STRING[] = "x^2-2*x^2+2*x*y+y^2+2*rt[2]*x+6*rt[2]*y+16=0"; //1.2


int* find_same_terms(int ind_start, int num_elt, int len_term, char** term, char** sprt_str);
char** slicing(int from, int to, char** sprt_str);
int num_chrs(char* str, char* chrs);
char** separating(char* str, int* size_sep_str);
char** cutting_term(int term_ind, int term_len, int sprt_str_len, char*** ptr_to_sprt_str){
    //returns coeff before term (e.g. for "..-2*rt[3]*x*y+.." will return "-2*rt[3]") in form of array of strings (in this example {"-", "2", "*", "rt[3]"})
    //this function is designed for 'sprt_str' without '(' and ')'
    char **sprt_str = *ptr_to_sprt_str;
    if(term_ind == 0){
        char **new_sprt_str = malloc((sprt_str_len-1) * sizeof(char*)),
            cff[] = "+";
        new_sprt_str[0] = malloc(sizeof(cff));
        strcpy(new_sprt_str[0], cff);
    }

    int i, j, coeff_len;
    char **coeff,
        stop_signs[] = "+-",
        **new_sprt_str = malloc((sprt_str_len-term_len) * sizeof(char*));

    for(j=term_ind;j>0;j--){
        printf("%d: |%s| %c\n", j, sprt_str[j], sprt_str[j][0]);
        if(strchr(stop_signs, sprt_str[j][0]))
            break;
    }
    coeff_len = term_ind - j;
    if(coeff_len != 1)
        coeff_len--; // it's for '*' between term and coeff
    printf("coeff_len: %d - %d = %d\n", term_ind, i, coeff_len);
    coeff = slicing(j, j+coeff_len, sprt_str);
    for(i=j;i<j+coeff_len;i++){
        printf("cff_in_sprt_str[%d]: |%s| real_cff[%d]: |%s|\n", i, sprt_str[i], i-j, coeff[i-j]);
    }
    for(i=j;i<term_ind+term_len;i++){
        printf("del[%d]: |%s|\n", i, sprt_str[i]);
    }
    for(i=0;i<term_ind-coeff_len-(coeff_len!=1);i++) // (coeff_len != 1) is for '*' between term and coeff (if it is)
        new_sprt_str[i] = sprt_str[i];
    for(i=term_ind+term_len;i<sprt_str_len;i++)
        new_sprt_str[i] = sprt_str[i];
    sprt_str = new_sprt_str;

    free(sprt_str);
    return coeff;
}
void summing_same_terms(int inti_term_ind, int* arr_ind, int term_len, char** sprt_str){

}
void reduc(char** sprt_str, int num_elt){ // privedenie podobnyx
    int i, j,
        ind_next_same_term = -1,
        end_of_term = -1,
        start_of_term = -1, // term is slagaemoe, pohodu
        buff_cntr = 0,
        *arr_ind;
    char **buff,
        list_of_vars[] = "xy";
    for(i=0;i<num_elt;i++){
        printf("%d |%d %d| |%s|\n", i, start_of_term, end_of_term, sprt_str[i]);
        if((sprt_str[i][0] == '+') || (sprt_str[i][0] == '-') || (sprt_str[i][0] == '=')){
            puts("if");
            buff = slicing(start_of_term, i-1, sprt_str);
            printf("%d %d %p\n", start_of_term, i-1, buff);
            printf("\t\t");
            for(int j=0;j<end_of_term-start_of_term+1;j++)
                printf("%s", buff[j]);
            puts("");

            end_of_term = i-1;

            arr_ind = find_same_terms(i+1, num_elt, end_of_term-start_of_term+1, buff, sprt_str);
            //int arr_size = (num_elt - i+1) / (end_of_term-start_of_term+1) + 1;
            //for(int k=0;k<arr_size;k++)
            //    printf("(arr): %d %d   ", k, arr_ind[k]);

            buff_cntr = 0;
        }
        else if(strchr(list_of_vars, sprt_str[i][0])){
            puts("else if");
            if(start_of_term <= end_of_term)
                start_of_term = i;
            /*
            buff = malloc(MAX_EXPRESSION_LENGTH * sizeof(char));
            buff[buff_cntr] = sprt_str[i];
            printf("buff_rec: %d |%s| |%s|\n", buff_cntr, sprt_str[i], buff[buff_cntr]);
            buff_cntr++;
            */
        }
    }
}
char* for_griha(int sprt_str_len, char** sprt_str){
    int i, cmb_size = strlen(sprt_str[0]);
    char *comeback = malloc(strlen(sprt_str[0]) * sizeof(char));
    strcpy(comeback, "");
    for(i=0;i<sprt_str_len;i++)
        strcat(comeback, sprt_str[i]);
    strcat(comeback, "0");
    return comeback;
}


int main(){
    int num_elt;
    char **sprt_str = separating(STRING, &num_elt); //separated string
    printf("num_elt: %d\n", num_elt);
    printf("BEF_slice_sprt_str: %p\n", sprt_str);
    char **box = slicing(1, 3, sprt_str);
    puts("start of REDUC");
    reduc(sprt_str, num_elt);
    puts("end of REDUC");
    printf("test: %s\n", for_griha(num_elt, sprt_str));
    //cutting_term(4, 1, num_elt, sprt_str);
    printf("num_elt2 %d\n", num_elt);

    puts("start");
    for(int i=0;i<num_elt;i++)
        printf("%d: %d |%s|\n", i, strlen(sprt_str[i]), sprt_str[i]);
    puts("end");

    getchar();
}

int num_chrs(char* str, char* chrs){
    int i,
        cntr = 0;
    for (i=0;i<strlen(str);i++)
        if (strchr(chrs, str[i]))
            cntr++;
    return cntr;
}

char** separating(char* str, int* size_sep_str){
    int i,
        sep_cntr = 0,
        buff_cntr = 0,
        buff_size = MAX_EXPRESSION_LENGTH * sizeof(char),
        len = strlen(str);
    char signs[] = "+-*/=",
        *buff = calloc(MAX_EXPRESSION_LENGTH, sizeof(char)),
        **sprt_str = malloc((num_chrs(str, signs)*2 + 1) * sizeof(char*));
    for (i=0;i<len-1;i++){ // len - 1 is just for my algo
        if(strchr(signs, str[i])){
            sprt_str[sep_cntr] = buff;
            //printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(2, sizeof(char)); // 2 is for '\0' maybe...
            //printf("\t\tcheck1: %p\n", buff);
            buff[0] = str[i];
            //printf("kwiok: %d |%s| |%c|%c|%c|\n", strlen(buff), buff, buff[0], buff[1], buff[2]);
            sprt_str[sep_cntr] = buff;
            //printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
            //printf("\t\tcheck2: %p\n", buff);
            buff_cntr = 0;
        }
        else{
            buff[buff_cntr] = str[i];
            //printf("buff rec: %d %d %c %c\n", i, buff_cntr, str[i], buff[buff_cntr]);
            buff_cntr++;
        }
    }
    /*
    puts("start");
    for(i=0;i<sep_cntr;i++){
        printf("%d: %d |%s|\n", i, strlen(sprt_str[i]), sprt_str[i]);
    }
    puts("end");
    */
    *size_sep_str = sep_cntr;
    return sprt_str;
}

char** slicing(int from, int to, char** sprt_str){
    printf("(slicing): %d %d %p\n", from, to, sprt_str);
    int i,
        slice_size = to - from + 1;
    if(slice_size < 1){
        puts("Wrong 'from' and/or 'to' came to 'slicing'.");
        exit(666);
    }
    printf("slice_check1: %d\n", slice_size);
    char *box[slice_size]; //= malloc(slice_size * sizeof(char*));
    printf("slice_check2: %p\n", box);
    for(i=from;i<=to;i++){
        box[i-from] = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
        //box[i-to] = sprt_str[i];
        printf("\tslice_for: %d %s %p\t\t", i, sprt_str[i], sprt_str[i]);
        strcpy(box[i-from], sprt_str[i]);
        printf("cont: %s %p\n", box[i-from], box[i-from]);
    }
    /*
    for(i=from;i<=to;i++){
        printf("%d: ", i-from);
        printf("|%s| ", box[i-from]);
        printf("%p\n", box[i-from]);
    }
    */
    char **buff = malloc(slice_size * sizeof(char*));
    for(int j=0;j<slice_size;j++){
        buff[j] = box[j];
        //strcpy(buff[j], box[j]);
        printf("buff[%d]: |%s| %p\n", j, buff[j], buff[j]);
    }
    return buff;
}

int* find_same_terms(int ind_start, int num_elt, int len_term, char** term, char** sprt_str){
    int i, j,
        arr_size = (num_elt - ind_start) / len_term + 1,
        arr_cntr = 0,
        *answers = calloc(arr_size, sizeof(int));
    puts("(find_same_terms)");
    printf("\t\tind_start: |%d|\n", ind_start);
    printf("\t\tnum_elt: |%d|\n", num_elt);
    printf("\t\tlen_term: |%d|\n", len_term);
    printf("\t\tARRSIZE: |%d|\n", arr_size);
    for(i=ind_start;i<num_elt;i++){
        for(j=0;j<len_term;j++){
            printf("(slc) %d %d ", i, j);
            printf("|%s| ", term[j]);
            printf("|%s|\n", sprt_str[i+j]);
            if(strcmp(term[j], sprt_str[i+j])){
                //puts("goto here");
                goto cont;
            }
        }
        puts("you passed!");
        answers[arr_cntr] = i;
        arr_cntr++;
        cont:   ;
    }
    return answers;
}
