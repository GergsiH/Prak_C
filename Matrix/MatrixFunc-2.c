#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long double **MatrixInput(unsigned long long *rows, unsigned long long *columns)
{
    long double **mat;
    long long numb;
    printf("How many rows are there in the matrix?\n");
    if (scanf("%lld", &numb) == 0)
    {
        printf("Are you idiot?\n");
        return NULL;
    }
    if (numb <= 0)
    {
        printf("Try again\n");
        return NULL;
    }
    *rows = numb;
    printf("How many columns are there in the matrix?\n");
    if (scanf("%lld", &numb) == 0)
    {
        printf("Are you idiot?");
        return NULL;
    }
    if (numb <= 0)
    {
        printf("Try again\n");
        return NULL;
    }
    *columns = numb;

    mat = malloc(sizeof(long double *) * *rows);
    printf("Enter the elements of the matrix\n");
    for (unsigned long long i = 0; i < *rows; i++)
    {
        mat[i] = malloc(sizeof(long double) * *columns);
        for (unsigned long long j = 0; j < *columns; j++)
        {
            if (scanf("%Lf", &mat[i][j]) == 0)
            {
                printf("Are you idiot?");
                return NULL;
            }
        }
    }
    return mat;
}

void MatrixOutput(long double **mat, unsigned long long rows, unsigned long long columns)
{
    if (!mat)
        return;
    for (unsigned long long i = 0; i < rows; i++)
    {
        for (unsigned long long j = 0; j < columns; j++)
        {
            printf("%Lf ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void MatrixFree(long double **mat, unsigned long long rows)
{
    for (unsigned long long i = 0; i < rows; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void MatrixSym(long double **mat1, long double **mat2, long double ***mat_mulsym, unsigned long long rows1, unsigned long long rows2, unsigned long long *rows_mulsym, unsigned long long columns1, unsigned long long columns2, unsigned long long *columns_mulsym)
{
    if (rows1 != rows2 || columns1 != columns2)
    {
        *mat_mulsym = NULL;
        *rows_mulsym = 0;
        *columns_mulsym = 0;
    }
    else
    {
        if (*mat_mulsym)
        {
            printf("1 \n");
            MatrixFree(*mat_mulsym, *rows_mulsym);
        }
        *rows_mulsym = rows1;
        *columns_mulsym = columns1;
        *mat_mulsym = malloc(sizeof(long double *) * (*columns_mulsym));
        for (unsigned long long i = 0; i < rows1; i++)
        {
            (*mat_mulsym)[i] = malloc(sizeof(long double) * (*rows_mulsym));
            for (unsigned long long j = 0; j < columns1; j++)
            {
                (*mat_mulsym)[i][j] = mat1[i][j] + mat2[i][j];
            }
        }
    }
}

void MatrixMultiply(long double **mat1, long double **mat2, long double ***mat3, unsigned long long rows1, unsigned long long rows2,
                    unsigned long long *rows3, unsigned long long columns1, unsigned long long columns2, unsigned long long *columns3)
{
    if (rows2 != columns1)
    {
        *mat3 = NULL;
        *rows3 = 0;
        *columns3 = 0;
    }
    else
    {
        *rows3 = rows1;
        *columns3 = columns2;
        *mat3 = malloc(sizeof(long double *) * (*rows3));
        for (unsigned long long i = 0; i < *rows3; i++)
        {
            (*mat3)[i] = malloc(sizeof(long double) * (*columns3));
            for (unsigned long long j = 0; j < *columns3; j++)
            {
                (*mat3)[i][j] = 0;
                for (unsigned long long k = 0; k < columns1; k++)
                {
                    (*mat3)[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
    }
}

void MatrixMultiplyOnConst(long double **mat, long double ***res, unsigned long long rows, unsigned long long *rowsRes, unsigned long long columns, unsigned long long *columnsRes, long double cons)
{
    *rowsRes = rows;
    *columnsRes = columns;
    *res = calloc(sizeof(long double *), rows);
    for (unsigned long long i = 0; i < rows; i++)
    {
        (*res)[i] = calloc(sizeof(long double), columns);
        for (unsigned long long j = 0; j < columns; j++)
        {
            (*res)[i][j] = cons * mat[i][j];
        }
    }
}

int MatrixDet(long double **mat, unsigned long long rows, unsigned long long columns, long double *result)
{
    if (rows != columns)
        return 0;
    if (rows == 0)
        return 0;
    if (rows == 1)
    {
        *result = mat[0][0];
        return 1;
    }
    if (rows == 2)
    {
        *result = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        return 1;
    }
    *result = 0;
    for (unsigned long long i = 0; i < rows; i++)
    {
        long double **minimatrix = malloc(sizeof(long double *) * rows);
        long double minimatrixDet;
        for (unsigned long long k = 0; k < rows - 1; k++)
        {
            minimatrix[k] = malloc(sizeof(long double) * rows - 1);
        }
        for (unsigned long long k = 1; k < rows; k++)
        {
            unsigned long long tmp = 0;
            for (unsigned long long l = 0; l < rows; l++)
            {
                if (l == i)
                    continue;
                else
                {
                    minimatrix[k - 1][tmp++] = mat[k][l];
                }
            }
        }
        MatrixDet(minimatrix, rows - 1, rows - 1, &minimatrixDet);
        *result = *result + pow(-1, i + 2) * mat[0][i] * minimatrixDet;
        MatrixFree(minimatrix, rows - 1);
        ;
    }
    return 1;
}
