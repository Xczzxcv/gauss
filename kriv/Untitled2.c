#include <stdio.h>
#include <stdlib.h>

void zamena_sep_str(char** mass, char** symbols) {
   *mass = *symbols;
}

int main(){
    char mass[] = "lol";
    char symbols[] = "bub";
    int nums[] = {1, 2, 3, 4};
    puts("XXX");
    zamena_sep_str(&mass, &symbols);
    puts("XXX");
    puts(mass);
    puts(nums);
}
