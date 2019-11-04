#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double positive(double p, double delta){
    double sqDelta, x1, x2;
    sqDelta = sqrt(delta);
    x1 = p + sqDelta;
    x2 = p - sqDelta;

    printf("x1=%f\nx2=%f\n", x1, x2);
}

double negative(double a, double b, double c){
    double r1, u1, u2, x1, x2;
    r1 = -b / (2 * a);
    u1 = sqrt( fabs( pow(b/2/a, 2) - (c / a)));
    u2 = -sqrt( fabs( pow(b/2/a, 2) - (c / a)));

    printf("x1=%f + %fi\nx2=%f + %fi\n", r1, u1, r1, u2);
}

int main(int argc, char *argv[]){

    if(argc != 4){
        printf("Usage ./quadratic a b c\nWhere a,b and c are the values\n");
        return 1;
    }
    
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double c = atof(argv[3]);

    double p = -b / (2 * a);
    double q = c / a;
    double delta = pow(p, 2) - q;

    if(delta >= 0)
        positive(p, delta);
    else
        negative(a, b, c);

    return 0;
}

