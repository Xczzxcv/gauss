#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_EXPRESSION_LENGTH 10

//раскрытие скобок
//приведение подобных
char STRING[] = "x^2+2*x*y+y^2+2*rt[2]*x+6*rt[2]*y+16=0"; //1.2

int find_same_term(int ind_start, int num_elt, char** term, char** sprt_str){
    int i;
    for(i=ind_start;i<num_elt;i++)
        ;//for()
}
char** slicing(int from, int to, char** sprt_str);
int num_chrs(char* str, char* chrs);
char** separating(char* str, int* size_sep_str);
void reduc(char** sprt_str, int num_elt){ // privedenie podobnyx
    int i,
        ind_next_same_term = -2,
        end_of_term = -1,
        start_of_term = -1, // term is slagaemoe, pohodu
        buff_cntr = 0;
    char **buff,
        list_of_vars[] = "xy";
    for(i=0;i<num_elt;i++){
        printf("%d |%d %d| |%s|\n", i, start_of_term, end_of_term, sprt_str[i]);
        if((sprt_str[i][0] == '+') || (sprt_str[i][0] == '-') || (sprt_str[i][0] == '=')){
            puts("if");
            buff = slicing(start_of_term, i-1, sprt_str);
            end_of_term = i-1;
            /*
            do{
                find_same_term()
            }while(ind_next_same_term != -1);
            */
            printf("%d %d %p\n", start_of_term, i-1, buff);
            printf("\t\t");
            for(int j=0;j<end_of_term-start_of_term+1;j++)
                printf("%s", buff[j]);
            puts("");
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

int main(){
    int num_elt;
    char **sprt_str = separating(STRING, &num_elt); //separated string
    printf("num_elt: %d\n", num_elt);
    printf("BEF_slice_sprt_str: %p\n", sprt_str);
    char **box = slicing(1, 3, sprt_str);
    puts("start of REDUC");
    reduc(sprt_str, num_elt);
    puts("end of REDUC");
    printf("num_elt2 %d\n", num_elt);
    /*
    puts("start");
    for(int i=0;i<num_elt;i++)
        printf("%d: %d |%s|\n", i, strlen(sprt_str[i]), sprt_str[i]);
    puts("end");
    */
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
            printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(2, sizeof(char)); // 2 is for '\0' maybe...
            //printf("\t\tcheck1: %p\n", buff);
            buff[0] = str[i];
            //printf("kwiok: %d |%s| |%c|%c|%c|\n", strlen(buff), buff, buff[0], buff[1], buff[2]);
            sprt_str[sep_cntr] = buff;
            printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
            //printf("\t\tcheck2: %p\n", buff);
            buff_cntr = 0;
        }
        else{
            buff[buff_cntr] = str[i];
            printf("buff rec: %d %d %c %c\n", i, buff_cntr, str[i], buff[buff_cntr]);
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
    int i,
        slice_size = to - from + 1;
    if(slice_size < 1){
        puts("Wrong 'from' and/or 'to' came to 'slicing'.");
        exit(666);
    }

    char** box = malloc(slice_size * sizeof(char*));
    //printf("slice_check: %d %p\n", slice_size, box);
    for(i=from;i<=to;i++){
        box[i-from] = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
        //box[i-to] = sprt_str[i];
        //printf("\tslice_for: %d %s %p\t\t", i, sprt_str[i], sprt_str[i]);
        strcpy(box[i-from], sprt_str[i]);
        //printf("cont: %s %p\n", box[i-from], box[i-from]);
    }
    /*
    for(i=from;i<=to;i++){
        printf("%d: ", i-from);
        printf("|%s| ", box[i-from]);
        printf("%p\n", box[i-from]);
    }
    */
    return box;
}

