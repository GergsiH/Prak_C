#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CONST 10

typedef struct tree{
    char* str;
    int numb;
    struct tree* left;
    struct tree* right;
} BinTree;

//Функция ввода слова
char* WordInput(){
    int n = CONST;
    int k = 0;
    int c = 0;
    char* str = malloc(CONST);
    if(!str) {
        return NULL;
    }
    c = getchar();
    if(c == EOF){
        free(str);
        return NULL;
    }
    if(ispunct(c) || isspace(c) || isdigit(c)){
        str[0] = c;
        str[1] = '\0';
        return str;
    }
    while (c != '\n'){
        if(!isalpha(c)){
            ungetc(c, stdin);
            str[k] = '\0';
            return str;
        } 
        if(k == n-2){
            str = realloc(str, k+CONST);
            n = n + CONST;
            if(!str){
                 return NULL;
            }
        }
        str[k] = c;
        k++;
        c = getc(stdin);
    }
    str[k] = '\0';
    return str;
}

//Функция вставки слова в дерево
void TreeInsert(BinTree **tree, char* str){
    if(*tree == NULL){
        *tree = malloc(sizeof(BinTree));
        int n = strlen(str)+1;
        (*tree)->str = malloc(n);
        strncpy((*tree)->str, str, n);
        (*tree)->numb = 1;
        (*tree)->left = (*tree)->right = NULL;
        return;
    }
    if(strcmp(str, (*tree)->str)> 0){
        TreeInsert(&((*tree)->right), str);
    }
    else if(strcmp(str, (*tree)->str)< 0){
         TreeInsert(&((*tree)->left), str);
    }
    else {
        (*tree)->numb++;
    }
}

//Перестройка дерева для правильного вывода
void BuildTree(BinTree **TreeOut, BinTree **tree1){
    BinTree *tree2 = *tree1;
	if(*TreeOut == NULL){
        *TreeOut = malloc(sizeof(BinTree));
        (*TreeOut)->str = tree2->str;
		printf("%s\n", tree2->str);	
        (*TreeOut)->numb = tree2->numb;
        (*TreeOut)->left = (*TreeOut)->right = NULL;
        return;
    }
    if((*TreeOut)->numb > tree2->numb){
        BuildTree(&((*TreeOut)->left), tree1);
    }
    else{
        BuildTree(&((*TreeOut)->right), tree1);
    }
}


void CorrectTree(BinTree **TreeOut, BinTree **tree){
	if((*tree) == NULL){ 
        return;
    }
	CorrectTree(TreeOut, &(*tree)->right);
	BuildTree(TreeOut, tree);
	CorrectTree(TreeOut, &(*tree)->left);
}

//Вывод дерева
void TreeOutput(BinTree **tree1, int n){
    BinTree *tree2 = *tree1;
    if(tree2 == NULL){
        return;
    }
    TreeOutput(&(tree2->right), n);
    double ratio = (double)(tree2->numb)/n;
    printf("%15s %3d %10f\n", tree2->str, tree2->numb, ratio);
    TreeOutput(&(tree2->left), n);
}

void TreeFreeKey(BinTree **tree){
    if((*tree)==NULL){
        return;
    }
    TreeFreeKey(&((*tree)->left));
    TreeFreeKey(&((*tree)->right));
    free((*tree)->str);
}

void TreeFree(BinTree **tree){
    if((*tree)==NULL) return;
    TreeFree(&((*tree)->left));
    TreeFree(&((*tree)->right));
    free(*tree);
}

int main() {
    BinTree *tree = NULL; 
    int AllWords = 0;
    BinTree *TreeOut = NULL;
    char *word;
    while ((word = WordInput()) != NULL){
        if(isspace(word[0]) || isdigit(word[0])){
            free(word);
            continue;
        }
        AllWords++;
        if(word[0] == 0){
             break;
        }
        TreeInsert(&tree, word);
        free(word);
    }
    CorrectTree(&TreeOut, &tree);
    TreeOutput(&TreeOut, AllWords);
	TreeFreeKey(&tree);
    TreeFree(&tree);
	TreeFree(&TreeOut);
}
