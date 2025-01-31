#ifndef MatrixFunc
#define MatrixFunc

long double** MatrixInput(unsigned long long *rows, unsigned long long *columns);
void MatrixOutput(long double **mat, unsigned long long rows, unsigned long long columns);
void MatrixFree(long double **mat, unsigned long long rows);
void MatrixSym(long double **mat1, long double **mat2, long double ***mat_mulsym, unsigned long long rows1, unsigned long long rows2, unsigned long long *rows_mulsym, unsigned long long columns1, unsigned long long columns2, unsigned long long *columns_mulsym);
void MatrixMultiply(long double **mat1, long double **mat2, long double ***mat3, unsigned long long rows1, unsigned long long rows2, unsigned long long *rows3, unsigned long long columns1, unsigned long long columns2, unsigned long long *columns3);
void MatrixMultiplyOnConst(long double **mat, long double ***res, unsigned long long rows, unsigned long long *rowsRes, unsigned long long columns, unsigned long long *columnsRes, long double cons);
int MatrixDet(long double **mat, unsigned long long n, unsigned long long m, long double *res);

#endif