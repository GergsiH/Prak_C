#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MatrixFunc.h"

int main(void)
{
    long double **matrix_1, **matrix_2, **matrix_MulSym=NULL;
    unsigned long long matrix_1_rows, matrix_1_columns;
    unsigned long long matrix_2_rows, matrix_2_columns;
    unsigned long long matrix_MulSym_rows, matrix_MulSym_columns;
    long double num;

    // Блок ввода двух матрицы
    printf("Enter the first matrix number.\n");
    matrix_1 = MatrixInput(&matrix_1_rows, &matrix_1_columns);
    if (matrix_1 == NULL){
        MatrixFree(matrix_1, matrix_1_rows);
        free(matrix_1);
        return 0;
    }
    printf("\n");
    printf("\nEnter the second matrix number.\n");
    matrix_2 = MatrixInput(&matrix_2_rows, &matrix_2_columns);
    if (matrix_2 == NULL){
        MatrixFree(matrix_1, matrix_1_rows);
        MatrixFree(matrix_2, matrix_2_rows);
        free(matrix_2);
        return 0;
    }


    // Блок вывода обеих матриц
    printf("Here is your entered first matrix:\n");
    MatrixOutput(matrix_1, matrix_1_rows, matrix_1_columns);
    printf("Here is your entered second matrix:\n");
    MatrixOutput(matrix_2, matrix_2_rows, matrix_2_columns);

    // Блок суммы двух матриц и вывода результата
    MatrixSym(matrix_1, matrix_2, &matrix_MulSym, matrix_1_rows, matrix_2_rows, &matrix_MulSym_rows, matrix_1_columns, matrix_2_columns, &matrix_MulSym_columns);
    if (matrix_MulSym){
         printf("To get started, work with matrix sums. Here ");
        printf("is the result of summing the first and second matrix:\n");
        MatrixOutput(matrix_MulSym, matrix_MulSym_rows, matrix_MulSym_columns);
        MatrixFree(matrix_MulSym, matrix_MulSym_columns);
    }
    else {
        printf("No sum\n");
        printf("\n");
    }

    // Блок пермножения матриц друг на друга и вывод результата
    MatrixMultiply(matrix_1, matrix_2, &matrix_MulSym, matrix_1_rows, matrix_2_rows, &matrix_MulSym_rows, matrix_1_columns, matrix_2_columns, &matrix_MulSym_columns);
    if (matrix_MulSym){
        printf("Perfectly! Now multiply the two matrices:\n");
        MatrixOutput(matrix_MulSym, matrix_MulSym_rows, matrix_MulSym_columns);
        MatrixFree(matrix_MulSym, matrix_MulSym_rows);
    }
    else{
        printf("No multiply\n");
        printf("\n");
    }

    // Блок умножения матрицы на число
    printf("So. Now multiply the first matrix by some number. Enter the number ");
    printf("by which you want to multiply the matrix:\n");
    scanf("%Lf", &num);
    printf("Here is the matrix multiplied by your number:\n");
    MatrixMultiplyOnConst(matrix_1, &matrix_MulSym, matrix_1_rows, &matrix_MulSym_rows, matrix_1_columns, &matrix_MulSym_columns, num);
    MatrixOutput(matrix_MulSym, matrix_MulSym_rows, matrix_MulSym_columns);
    MatrixFree(matrix_MulSym, matrix_MulSym_rows);

    // Блок вычисления определителя(эх, пришло время приступить к самому сложному)
    if (MatrixDet(matrix_1, matrix_1_rows, matrix_1_columns, &num))
    {
        printf( "Determinant of the first matrix: %Lf \n", num);
        printf("\n");
    }
    else
    {
        printf("No Determinant\n");
        printf("\n");
    }
    printf("Hey, congratulations! We did it! Now it remains only ");
    printf("to clear the memory, but this is no longer your problem, \n");
    printf("user. Well, see you soon!\n");
    MatrixFree(matrix_1, matrix_1_rows);
    MatrixFree(matrix_2, matrix_2_rows);
    
    
    return 0;
}

