#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_EXPRESSION_LENGTH 10

//раскрытие скобок
//приведение подобных
char STRING[] = "x^2+2*x*y+y^2+2*rt[2]*x+6*rt[2]*y+16=0"; //1.2

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
        **sprt_str = malloc(num_chrs(str, signs)*2 * sizeof(char*));
    for (i=0;i<len-1;i++){ // len - 1 is just for my algo
        if(str[i] == '=')
            continue;
        if(strchr(signs, str[i])){
            sprt_str[sep_cntr] = buff;
            printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(2, sizeof(char)); // 2 is for '\0' maybe...
            printf("\t\tcheck1: %p\n", buff);
            buff[0] = str[i];
            printf("kwiok: %d |%s| |%c|%c|%c|\n", strlen(buff), buff, buff[0], buff[1], buff[2]);
            sprt_str[sep_cntr] = buff;
            printf("sprt_str rec: %d %d |%s| |%s|\n", i, sep_cntr, buff, sprt_str[sep_cntr]);
            sep_cntr++;
            buff = calloc(MAX_EXPRESSION_LENGTH, sizeof(char));
            printf("\t\tcheck2: %p\n", buff);
            buff_cntr = 0;
        }
        else{
            buff[buff_cntr] = str[i];
            printf("buff rec: %d %d %c %c\n", i, buff_cntr, str[i], buff[buff_cntr]);
            buff_cntr++;
        }
    }
    puts("start");
    for(i=0;i<sep_cntr;i++){
        printf("%d: %d |%s|\n", i, strlen(sprt_str[i]), sprt_str[i]);
    }
    puts("end");
    *size_sep_str = sep_cntr;
    return sprt_str;
}

int main(){
    int num_elt;
    char **sprt_str = separating(STRING, &num_elt);
}

