#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EXPRESSION_LENGTH 10
#define N 3
#define SIZE_M 25

//Constants:
int count_x = 0;
int count_y = 0;
int count_z = 0;
char STRING[] = "x*x+2*x*y+y*y+x*x+x*x+y*y";

//Functions:
int poisk(char* mass, char* yeah);
char* kvadrat(char* mass, char* vyr);
char* kvadrati(char* mass);
char* zamena(char* mass, int key_to_change, char symbol);
char* vstavka(char* mass, int key_input_aft, int symbol);
char* zamena_na_massiv(char* mass, int key_to_change, char* symbols);
int num_chrs(char* str, char* chrs);
char** separating(char* str, int* size_sep_str);
void show(char* mass);
void zamena_sep_str(char* mass, char* symbols);

int main () {
    int i;
    char* mass_pt = calloc(SIZE_M, sizeof(char));
    for(i = 0; i < SIZE_M; ++i) {
        mass_pt[i] = STRING[i];
    }
    //mass_pt = kvadrati(mass_pt);
    show(mass_pt);
    int size_skob = 15;
    char skobka[15] = "(sin(x)+cos(y))";
    char* skob_ptr = calloc(size_skob, sizeof(char));
    for(i = 0; i < size_skob; ++i) {
        skob_ptr[i] = skobka[i];
    }
    show(skob_ptr);
    int num_elt;
    char **sprt_str = separating(STRING, &num_elt);
    printf("%s", sprt_str[0]);
    sprt_str[0] = skob_ptr;
    zamena_sep_str(sprt_str[0], skob_ptr);
    printf("%s", sprt_str[0]);
    return 0;
}

int poisk(char* mass, char* yeah) {
    char vyr[3];
    int element = 0;
    for(element = 0; element < SIZE_M; ++element) {
        for(int i = 0; i < 3; ++i) {
            vyr[i] = mass[i+element];
            //printf("%c", vyr[i]);
        }
            //printf("\n");
        if(vyr[0] == yeah[0] && vyr[1] == yeah[1] && vyr[2] == yeah[2]) {
           //printf("%i", element);
           break;
        }
    }
    return element;
}

char* kvadrat(char* mass, char* vyr) {
    int change_zero = poisk(mass, vyr);
    char bukva = mass[change_zero];
    char* new_mass = malloc(SIZE_M*sizeof(char));
    for(int i = 0; i < SIZE_M; ++i) {
        new_mass[i] = mass[i];
    }
    new_mass[change_zero] = bukva;
    new_mass[change_zero+1] = '^';
    new_mass[change_zero+2] = '2';
    return new_mass;
}

char* kvadrati(char* mass) {
    while(poisk(mass, "x*x") != SIZE_M) {
        mass = kvadrat(mass, "x*x");
        ++count_x;
    }
    while(poisk(mass, "y*y") != SIZE_M) {
        mass = kvadrat(mass, "y*y");
        ++count_y;
    }
    while(poisk(mass, "z*z") != SIZE_M) {
        mass = kvadrat(mass, "z*z");
        ++count_z;
    }
    return mass;
}

char* zamena(char* mass, int key_to_change, char symbol) {
    int i;
    char* new_mass = calloc(sizeof(char), strlen(mass));
    for(i = 0; i < strlen(mass); ++i) {
        new_mass[i] = mass[i];
    }
    new_mass[key_to_change] = symbol;
    return new_mass;
}

char* vstavka(char* mass, int key_input_aft, int symbol) {
    int i;
    char* new_mass = calloc(sizeof(char), strlen(mass)+1);
    for(i = 0; i <= key_input_aft; ++i) {
        new_mass[i] = mass[i];
    }
    new_mass[key_input_aft+1] = symbol;
    if(key_input_aft > strlen(mass)-1) {
        ;
    }
    else{
        for(i = key_input_aft+2; i < strlen(mass)+1; ++i) {
            new_mass[i] = mass[i-1];
        }
    }
    return new_mass;
}

char* zamena_na_massiv(char* mass, int key_to_change, char* symbols) {
    int i;
    char* new_mass = calloc(strlen(mass), sizeof(char));
    for(i = 0; i < strlen(mass); ++i) {
        new_mass[i] = mass[i];
    }
    new_mass = zamena(mass, key_to_change, symbols[0]);
    for(i = 1; i < strlen(symbols); ++i) {
        new_mass = vstavka(new_mass, key_to_change + i - 1, symbols[i]);
    }
    return new_mass;
}

int num_chrs(char* str, char* chrs) {
    int i,
        cntr = 0;
    for (i=0;i<strlen(str);i++)
        if (strchr(chrs, str[i]))
            cntr++;
    return cntr;
}

char** separating(char* str, int* size_sep_str) {
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
    //puts("start");
    for(i=0;i<sep_cntr;i++){
        //printf("%d: %d |%s|\n", i, strlen(sprt_str[i]), sprt_str[i]);
    }
    //puts("end");
    *size_sep_str = sep_cntr;
    return sprt_str;
}

void show(char* mass) {
    puts("\n");
    for(int i = 0; i < strlen(mass); ++i) {
            printf("%c", mass[i]);
        }
    puts("\n");
}

void zamena_sep_str(char* mass, char* symbols) {
   mass = symbols;
}
