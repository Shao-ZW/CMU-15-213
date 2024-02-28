/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

//s = 5,E = 1, b = 5;
void solve1(int A[][32], int B[][32]);
void solve2(int A[][64], int B[][64]);
void solve3(int A[][61], int B[][67]);

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if(M == 32 && N == 32)
        solve1(A, B);
    else if(M == 64 && N == 64)
        solve2(A, B);
    else if(M == 61 && N == 67)
        solve3(A, B);
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

void solve1(int A[][32], int B[][32])
{
    int a0, a1, a2, a3, a4, a5, a6, a7, tmp;
    int i, j, x, y;

    for(i = 0; i < 32; i += 8)
        for(j = 0; j < 32; j += 8)
        {
            if(i == j)
            {
                for(x = i; x < i + 8; ++x)
                {
                    a0 = A[x][j];
                    a1 = A[x][j + 1];
                    a2 = A[x][j + 2];
                    a3 = A[x][j + 3];
                    a4 = A[x][j + 4];
                    a5 = A[x][j + 5];
                    a6 = A[x][j + 6];
                    a7 = A[x][j + 7];

                    B[x][j] = a0;
                    B[x][j + 1] = a1;
                    B[x][j + 2] = a2;
                    B[x][j + 3] = a3;
                    B[x][j + 4] = a4;
                    B[x][j + 5] = a5;
                    B[x][j + 6] = a6;
                    B[x][j + 7] = a7;
                }

                for(x = i; x < i + 8; ++x)
                    for(y = x + 1; y < j + 8; ++y)
                    {
                        tmp = B[x][y];
                        B[x][y] = B[y][x];
                        B[y][x] = tmp;
                    }
            }
            else
                for(x = i; x < i + 8; ++x)
                    for(y = j; y < j + 8; ++y)
                        B[x][y] = A[y][x];
        }
}

void solve2(int A[][64], int B[][64])
{
    int a0, a1, a2, a3, a4, a5, a6, a7, tmp;
    int i, j, di, dj;

    for(j = 0; j < 64; j += 8)
    {
        //优先处理对角线
        i = j;
        int c = (j == 0) ? 8: 0;
        for(di = 4; di < 8; ++di)
            for(dj = 0; dj < 8; ++dj)
                if(dj < 4)
                    B[j + dj][c + di - 4] = A[i + di][j + dj];
                else    
                    B[j + dj - 4][c + di] = A[i + di][j + dj];

        for(di = 0; di < 4; ++di)
        {
            a0 = A[i + di][j];
            a1 = A[i + di][j + 1];
            a2 = A[i + di][j + 2];
            a3 = A[i + di][j + 3];
            a4 = A[i + di][j + 4];
            a5 = A[i + di][j + 5];
            a6 = A[i + di][j + 6];
            a7 = A[i + di][j + 7];
            B[j + di][i] = a0;
            B[j + di][i + 1] = a1;
            B[j + di][i + 2] = a2;
            B[j + di][i + 3] = a3;
            B[j + di][i + 4] = a4;
            B[j + di][i + 5] = a5;
            B[j + di][i + 6] = a6;
            B[j + di][i + 7] = a7;
        }

        for(dj = 0; dj < 4; ++dj)
            for(di = dj + 1; di < 4; ++di)
            {
                tmp = B[j + dj][i + di];
                B[j + dj][i + di] = B[i + di][j + dj];
                B[i + di][j + dj] = tmp;
            }

        for(dj = 0; dj < 4; ++dj)
            for(di = 4 + dj + 1; di < 8; ++di)
            {
                tmp = B[j + dj][i + di];
                B[j + dj][i + di] = B[i + di - 4][j + dj + 4];
                B[i + di - 4][j + dj + 4] = tmp;
            }
        
        for(dj = 0; dj < 4; ++dj)
        {
            a0 = B[j + dj][i + 4];
            a1 = B[j + dj][i + 5];
            a2 = B[j + dj][i + 6];
            a3 = B[j + dj][i + 7];
            B[j + dj][i + 4] = B[j + dj][c];
            B[j + dj][i + 5] = B[j + dj][c + 1];
            B[j + dj][i + 6] = B[j + dj][c + 2];
            B[j + dj][i + 7] = B[j + dj][c + 3];
            B[j + dj + 4][i] = a0;
            B[j + dj + 4][i + 1] = a1;
            B[j + dj + 4][i + 2] = a2;
            B[j + dj + 4][i + 3] = a3;
        }
        
        for(dj = 4; dj < 8; ++dj)
            for(di = 4; di < 8; ++di)
                B[j + dj][i + di] = B[j + dj - 4][c + di];
        
        //非对角线
        for(i = 0; i < 64; i += 8)
        {
            if(i == j)  continue;

            for(di = 0; di < 4; ++di)
                for(dj = 0; dj < 8; ++dj)
                    if(dj < 4)
                        B[j + dj][i + di] = A[i + di][j + dj];
                    else    
                        B[j + dj - 4][i + di + 4] = A[i + di][j + dj];

            for(dj = 0; dj < 4; ++dj)
            {
                a0 = B[j + dj][i + 4];
                a1 = B[j + dj][i + 5];
                a2 = B[j + dj][i + 6];
                a3 = B[j + dj][i + 7];

                B[j + dj][i + 4] = A[i + 4][j + dj];
                B[j + dj][i + 5] = A[i + 5][j + dj];
                B[j + dj][i + 6] = A[i + 6][j + dj];
                B[j + dj][i + 7] = A[i + 7][j + dj];

                B[j + dj + 4][i] = a0;
                B[j + dj + 4][i + 1] = a1;
                B[j + dj + 4][i + 2] = a2;
                B[j + dj + 4][i + 3] = a3;
            }

            for(di = 4; di < 8; ++di)
                for(dj = 4; dj < 8; ++dj)
                    B[j + dj][i + di] = A[i + di][j + dj];
        }
    }
} 

void solve3(int A[][61], int B[][67])
{
    for(int i = 0; i < 67; i += 16)
        for(int j = 0; j < 61; j += 16)
            for(int di = 0; di < 16 && i + di < 67; ++di)
                for(int dj = 0; dj < 16 && j + dj < 61; ++dj)
                    B[j + dj][i + di] = A[i + di][j + dj];
            
}
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

