#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define  CONST 5

char* InpStr(void){ 
    int n, m = 0; 
    char* str = malloc(CONST); //Выделяет память размером const 
    if(!str) 
        return NULL; //если выделение памяти невозможно, то функция Null
    while (fgets(str+m, CONST, stdin)){ //Считывает строки и помещает в строку s
        n = strlen(str); 
        if(str[n-1]!= '\n'){ //Проверка на окончание строки
            m = m+CONST-1; //И так как не конец выделяется доп память
            str = realloc(str, m+CONST); 
            if(!str) 
                return NULL; 
        } 
        else{ 
            str[n-1] = '\0'; //Если строка заканчивается то ставим "0"
            return str; 
        } 
    } 
    free(str); // Если достигнут конец файла функция освобождает выделенную память
    return NULL; 
} 

char** Memory_allocation(int *n){ 
    int i = 0, m = 0; 
    char** str = calloc(CONST, sizeof(char*)); 
    if(!str) 
        return NULL; //Проверяем, удалось ли выделить память, если нет возвращаем NULL
    while(666) { 
        if(i == m+CONST){ 
            m = m+CONST; 
            str = realloc(str, (m+CONST)*sizeof(char*)); 
            if(!str) 
                return NULL; 
        } 
        if((str[i] = InpStr()) == NULL) 
            break; 
        i++; 
    } 
    *n = i; 
    return str; 
} 

 
void LexigSort(char** array, int n){ 
    char *str1;
    char *str2;
    int i,j;

    for(i = 0; i < n; i++){ 
        for(j = 0; j < n - i - 1; j++){ 
            str1 = array[j];
            str2 = array[j+1]; 
            while( *str1 == *str2 && *str1 != '\0'){ 
                str1++; 
                str2++; 
            }
            if((*str1 - *str2) > 0){ 
                char* arr = array[j+1]; 
                array[j+1] = array[j]; 
                array[j] = arr; 
            } 
        } 
    } 
} 
 
void OutputStr (char* array[], int n){
    int i = 0;
    if (n == 0){
        printf("No strings!!! Try again\n");
        return;
    }
    else{
        printf("Your lexigrophically sorted strings:\n");
        while (i < n){ 
            printf("%s\n" , array[i]); 
            ++i;
        }
        i=0;
        while(i < n){ 
            free(array[i]); 
            ++i;
        } 
        free(array);
    }
    return;
}
 
 
int main(){

    printf("\nThis program lexigrophically sorts the entered strings.\n"); 
    printf("Before starting work, enter an arbitrary number of lines via 'Enter':\n");

    int n = 0;  
    char** string = Memory_allocation(&n);

    LexigSort(string, n); 
    
    printf("\n"); 
    OutputStr(string,n);

    return 0; 
}