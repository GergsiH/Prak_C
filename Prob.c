#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long double **read_matrix (unsigned long long *line, unsigned long long *column)
{
    long double **matrix;
    long long n;
    printf("%s\n", "Enter a number of lines:");
    scanf("%lld", &n);
    if (n <= 0)
    {
        printf("%s\n", "Ooops, incorrect data!");
        return NULL;
    }
    *line = n;
    printf("%s\n", "Enter a number of columns:");
    scanf("%lld", &n);
    if (n <= 0)
    {
        printf("%s\n", "Ooops, incorrect data!");
        return NULL;
    }
    *column = n;
    matrix = malloc(*line*sizeof(long double*));
    for (unsigned long long i = 0; i < *line; i++)
    {
        matrix[i] = malloc(*column*sizeof(long double));
        printf("%s", "Enter elements of line ");
        printf("%lld\n", i + 1);
        for (unsigned long long j = 0; j < *column; j++)
        {
            scanf("%Lf", &matrix[i][j]);
        }
    }
    return matrix;
}

void out_matrix(long double **matrix, unsigned long long line, unsigned long long column)
{
    if (matrix == NULL)
        return;
    printf("Your matrix:\n");
    for (unsigned int i = 0; i < line; i++)
    {
        for (unsigned int j = 0; j < column; j++)
        {
            printf("%Lf", matrix[i][j]);
            printf(" ");
        }
        printf("\n");
    }
}

void matrix_free(long double **matrix, unsigned long long line)
{
    for (unsigned long long i = 0; i < line; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

long double **sum_two_matrix (long double **matrix_1, unsigned long long *line_1, unsigned long long *column_1, long double **matrix_2, unsigned long long *line_2, unsigned long long *column_2, long double **result)
{
    if (*line_1 != *line_2 || *column_1 != *column_2)
    {
        printf("ERROR! The matrices have different sizes\n");
        return NULL;
    }
    result = malloc(*line_1*sizeof(long double*));
    for (unsigned int i = 0; i < *line_1; i++)
    {
        result[i] = malloc(*column_1*sizeof(long double));
        for (unsigned int j = 0; j < *column_1; j++)
        {
            result[i][j] =  matrix_1[i][j] + matrix_2[i][j];
        }
    }
    return result;
}

long double **mul_on_num (long double **matrix, long double **result, unsigned long long *line, unsigned long long *column, long double num)
{
    result = malloc(*line*sizeof(long double*));
    for (unsigned int i = 0; i < *line; i++)
    {
        result[i] = malloc(*column*sizeof(long double));
        for (unsigned int j = 0; j < *column; j++)
        {
            result[i][j] =  matrix[i][j] * num;
        }
    }
    return result;
}

long double **mul_two_matrix (long double **matrix_1, unsigned long long *line_1, unsigned long long *column_1, long double **matrix_2, unsigned long long *line_2, unsigned long long *column_2, long double **result)
{
    if (*column_1 != *line_2)
    {
        printf("ERROR! The matrices have incorrect sizes for multiplication");
        return NULL;
    }
    result = malloc(*line_1*sizeof(long double*));
    for (unsigned int i = 0; i < *line_1; i++)
    {
        result[i] = malloc(*column_2*sizeof(long double));
        for (unsigned int j = 0; j < *column_2; j++)
        {
            result[i][j] = 0;
            for (unsigned int n = 0, m = 0; n < *column_1, m < *line_2; n++, m++)
            {
                result[i][j] += (matrix_1[i][n] * matrix_2[m][j]);
            }
        }
    }
    return result;
}

int Matrix_det (long double **matrix, unsigned long long line, unsigned long long column, long double *result)
{
    if (line != column)
    {
        return 0;
    }
    if (line == 0)
    {
        return 0;
    }
    if (line == 1)
    {
        *result = matrix[0][0];
        return 1;
    }
    if (line == 2)
    {
        *result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]; 
        return 1;
    }
    *result = 0;
    for (unsigned long long i = 0; i < line; i++)
    {
        long double **new_matrix = malloc(line * sizeof(long double*));
        long double new_matrix_det;
        for (unsigned long long j = 0; j < line - 1; j++)
        {
            new_matrix[j] = malloc(line * sizeof(long double) - 1);
        }
        for (unsigned long long m = 1; m < line; m++)
        {
            unsigned long long k = 0;
            for (unsigned long long l = 0; l < line; l++)
            {
                if (l == i)
                    continue;
                else 
                {
                    new_matrix[m - 1][k++] = matrix[m][l];
                }
            }
        }
        new_matrix_det = Matrix_det(new_matrix, line - 1, line - 1, &new_matrix_det); 
        *result = *result + pow(-1, i + 2) * matrix[0][i] * new_matrix_det;
        matrix_free(new_matrix, line - 1);
    }
    return 1;
}


int main()
{
    long double **matrix_1;
    //long double **matrix_2;
    //long double **matrixMUL;
    //long double **matrixSUM;
    //long double **matrix2MUL;
    long double matrixDET;
    unsigned long long num_line_1, num_column_1, num_line_2, num_column_2;
    matrix_1 = read_matrix(&num_line_1, &num_column_1);
    out_matrix(matrix_1, num_line_1, num_column_1);
    //matrix_2 = read_matrix(&num_line_2, &num_column_2);
    //out_matrix(matrix_2, num_line_2, num_column_2);
    //matrixMUL = mul_on_num(matrix, matrixMUL, &num_line, &num_column, 4);
    //matrixSUM = sum_two_matrix (matrix_1, &num_line_1, &num_column_1, matrix_2, &num_line_2, &num_column_2, matrixSUM);
    //matrix2MUL = mul_two_matrix (matrix_1, &num_line_1, &num_column_1, matrix_2, &num_line_2, &num_column_2, matrix2MUL);
    //out_matrix(matrix2MUL, num_line_1, num_column_2);
    if (Matrix_det (matrix_1, num_line_1, num_column_1, &matrixDET))
        printf("Determinant is %Lf\n", matrixDET);
    else
        printf("Oops, incorrect data...........");

}