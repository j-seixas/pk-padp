#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THR 4
#define N 3
double det(double A[N][N], int n);
omp_lock_t lock;

// Function to calculate the determinant of a matrix
double det(double A[N][N], int n) {
    double Minor[N][N];
    int c1,c2;
    double determinant;
    int O = 1;

    determinant = 0;
    if(n == 2) {
        determinant = A[0][0] * A[1][1] - A[0][1] * A[1][0];
        return determinant;
    } else {
        for(int i = 0; i < n; i++) {
            c1 = 0, c2 = 0;
            for(int j = 0; j < n; j++) {
                for(int k = 0; k < n ; k++){
                    if(j != 0 && k != i){
                        Minor[c1][c2] = A[j][k];
                        c2++;
                        if(c2 > n - 2) {
                            c1++;
                            c2 = 0;
                        }
                    }
                }
            }
            determinant += O * (A[0][i] * det(Minor, n - 1));
            O *= -1;
        }
    }

    return determinant;
}

// Function to print a matrix on screen
void print_matrix(double matrix[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            printf("\t%f", matrix[i][j]);
        printf("\n");
    }
}

/* Expected results for these example:
    x1: 0.9255
    x2: 1.250979
    x3: -1.729033

    The expected results of the determinants should be
        det(matrix): 38.322
        det(temp x1): 35.467 
        det(temp x2): 47.94 
        det(temp x3): -66.26 
*/
int main() {
    omp_set_num_threads(NUM_THR);
    
    // Example matrix with a(NxN) and b(n)
    double M[N][N + 1] = {{2, 3.1, 1, 4},
                    {6.2, 3.1, 4, 2.7},
                    {8, 6.3, 1.9, 12}};

    double b[N];
    double matrix[N][N];

    // Spliting the a and b matrixes 
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N + 1; j++){
            if(j == N) 
                b[i] = M[i][j];
            else
                matrix[i][j] = M[i][j];
        }
    }

    double result;
    double temp_mat[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            temp_mat[i][j] = matrix[i][j];
    }

    // Now calculates de x(n) based on Cramer's Rule (https://en.wikipedia.org/wiki/Cramer%27s_rule)

    #pragma omp parallel for schedule(static) shared(temp_mat)
    for(int i = 0; i < N; i++){
        omp_set_lock(&lock);
        for(int j = 0; j < N; j++)
            temp_mat[j][i] = b[j];

        result = det(temp_mat, N) / det(matrix, N); 
        printf("Result of x%d: %f\n", i + 1, result);
        
        for(int j = 0; j < N; j++)
            temp_mat[j][i] = matrix[j][i];
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    return 0;
}