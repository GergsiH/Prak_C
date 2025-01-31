#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int BraceCount = 0;
int CheckEOF = 0;
int LineCount = 0;
int CheckWord = 0;
int File = 0;

void OutputWord(char** l){
    char** p=l;
    while (*p!=NULL) {
        printf("%s\n",*p);
        p++;
    }
}

void ArrayFree(char** l ){
    while(*l!=NULL){
        free(*l);
        l++; 
    }
}

void WordInBrace(char **word, int *i, int *Brace, int symbols) {
    if (symbols == '"') {
        *Brace = 0;
    } else {
        *word = (char *)realloc(*word, (*i + 2) * sizeof(char));
        (*word)[*i] = symbols;
        (*i)++;
    }
}

char *InputWord(FILE * Input) {
    char SimilarSymbols[] = {'&', '|', '<', '>', '\0'};
    char OneSymbol[] = {';', '(', ')', '\0'};
    CheckWord = 0;
    int SimSymbol = 0;
    char symbol = 0;
    int i = 0;
    char *word = (char *)malloc(sizeof(char));
    word[i] = 0;

    while ((symbol = getc(Input)) != EOF) {
        if (BraceCount) { 
            WordInBrace(&word, &i,&BraceCount, symbol);
        } else if (SimSymbol) {
            SimSymbol = 0;
            if (symbol == word[i - 1]) {
                word = (char *)realloc(word, (i + 2) * sizeof(char));
                word[i] = symbol;
                i++;
                word[i] = 0;
                return word;
            } else {
                ungetc(symbol, Input);
                word[i] = 0;
                return word;
            }
        } else if (isspace(symbol)) {
            if (CheckWord) { 
                if (symbol == '\n') {
                    LineCount = 1;
                }
                word[i] = 0;
                return word;
            } else { 
                if (symbol == '\n') {
                    LineCount = 1;
                    free(word);
                    word = NULL;
                    return NULL;
                }
                continue; 
            }
        } else if (symbol == '\"') {
            CheckWord = 1;
            BraceCount = 1;
        }else if (strchr(SimilarSymbols, symbol)) {
            if (CheckWord) {
                ungetc(symbol, Input);
                word[i] = 0;
                return word;
            } else {
                SimSymbol = 1;
                word = (char *)realloc(word, (i + 2) * sizeof(char));
                word[i] = symbol;
                i++;
            }
        } else if (strchr(OneSymbol, symbol)) {
            if (CheckWord) {
                ungetc(symbol, Input);
                word[i] = 0;
                return word;
            } else {
                word = (char *)realloc(word, (i + 2) * sizeof(char));
                word[0] = symbol;
                word[1]= '\0';
                return word;
            }
        } else {               
            CheckWord = 1;
            word = (char *)realloc(word, (i + 2) * sizeof(char));
            word[i] = symbol;
            i++;
        }
    }
    CheckEOF = 1;
    if (word != NULL) {
        free(word);
        return NULL;
    } else {
        word[i] = 0;
        return word;
    }
}


int main(int argc,char** argv){
    FILE* Input;
    if(argc>1){
        Input = fopen(argv[1],"r");
        File = 1;
    }
    else{
         Input = stdin;
    }
    char** ArrayOfWords;
    char* word;
    int cnt;
    while(!CheckEOF) {
        ArrayOfWords = malloc(sizeof(char*));
        cnt=0;
        LineCount = 0;
        while (!CheckEOF && !LineCount){
            word = InputWord(Input);
            ArrayOfWords[cnt] = word;
            ArrayOfWords = realloc(ArrayOfWords, sizeof(char*)*(cnt+2));
            ArrayOfWords[cnt+1]=NULL;
            cnt++; 
        }
        if(!CheckEOF && !BraceCount){
            OutputWord(ArrayOfWords);
        }
        ArrayFree(ArrayOfWords);
        free(ArrayOfWords); 
        }
    if(BraceCount){
        printf("Odd amount of quotes\n");
    }
    if (argc>1){
        fclose(Input);
    } 
    return 0;
}