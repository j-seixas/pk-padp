#include <stdlib.h>
#include <stdio.h>
#include "tmeas.h"


int main(){
    double a = 1.0, t;

    tstart();
    for(int i = 0; i < 30000; i++) 
        a = a * 2.1 + 1.2;

    t = tstop();
    printf("%lf", t);

    tstart();
    for(int i = 0; i < 30; i++)
        printf("%d\n", i);

    t = tstop();
    printf("%lf", t);

    return 0;
}

