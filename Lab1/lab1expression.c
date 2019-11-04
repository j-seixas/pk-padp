#include <stdio.h>
#include <math.h>

double expression_v(double x, double y, double i, double j){
    double v = (log(pow(x, 2)) + 2 * pow(x, 3) + pow(y, -2)) / ((sin(x) - 10)* j) + (2 * i) / (j + 1);
    return v;
}

double expression_w(double x, double y, double i, double j){
    double w = x * cos(x + 1) / 2 * pow(sin(5 * i * i), 2);
    return w;
}


int main(){
    
    printf("v=%f\n", expression_v(1.2,5.6,4,2));
    printf("w=%f\n", expression_w(1.2,5.6,4,2));

    return 0;
}


