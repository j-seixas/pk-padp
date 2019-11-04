#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>     // sleep
 
pthread_mutex_t mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond    = PTHREAD_COND_INITIALIZER; 
#define CARTS 3
int carts_available = CARTS;
#define N 5     /* threads number */
int cartsArray[CARTS];
char condition = 0;

int available_cart(){
    for(int i = 0; i < CARTS ; i++){
        if(cartsArray[i] == 0)
            return i;
    }

    return -1;
}
 
void* thread(void* number) {
    printf("\tThread #%d starts\n", (int)number);
    int shopping_times = 2;
    int id_cart = 0;

    while (shopping_times > 0) {
        
        pthread_mutex_lock(&mutex);
        do {
            id_cart = available_cart();
            if(id_cart >= 0) {
                cartsArray[id_cart] = 1;
                break;
                

            } else {
                printf("\tClient #%d is waiting for cart...\n", (int)number);
                pthread_cond_wait(&cond, &mutex);   
                // printf("\t... Client #%d recieved signal!\n", (int)number);
                
            }

        } while (1);

        pthread_mutex_unlock(&mutex);
        printf("\t... Client #%d has cart! %d\n", (int)number, id_cart);
        //pthread_cond_broadcast(&cond);
        
        int random = rand() % 5;
        sleep(random);
        printf("\t... Client #%d finished shopping after %d time and released shopping cart %d!\n", (int)number, random, id_cart);
        pthread_mutex_lock(&mutex);
        cartsArray[id_cart] = 0;
        pthread_mutex_unlock(&mutex);

        pthread_cond_broadcast(&cond);
        sleep(1);
        shopping_times--;
    }

    printf("-> Client #%d leaved\n", (int)number);

    return NULL;
}
 
int main() {
    pthread_t id[N];
    int i;
    srand(time(NULL));
    puts("Beginning of the program");

    for (i=0; i < N; i++) {
        errno = pthread_create(&id[i], NULL, thread, (void*)(i+1));
        if (errno) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }


    for( i = 0; i < N; i++ )	
		pthread_join( id[i], NULL);


    /* end of main process - we don't care about threads */
    puts("End of program");
    return EXIT_SUCCESS;
}
