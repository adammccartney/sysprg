/* main-race.c 
 * introduces the use of multiple exclusions (mutexes)
 * these are a good idea when dealing with concurrent threads who need to
 * share data with one another
 * tips: any thread that wants access to global data needs to be locked before
 * and unlocked after it accesses the address of the data. This should be done
 * for each thread at each position that it accesses global data. 
 * */

#include <stdio.h>

#include "common_threads.h"

int balance = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    int s;
    s = pthread_mutex_lock(&lock);
    if (s != 0)
        perror("Error: threading catastrophy!");
    balance++; // unprotected access 
    s = pthread_mutex_unlock(&lock);
    if (s != 0)
        perror("Error: unlocking catastrophy!");
    return NULL;
}

int main(int argc, char *argv[]) {
    int s;
    pthread_t p;
    pthread_create(&p, NULL, worker, NULL);
    s = pthread_mutex_lock(&lock);
    if (s != 0)
        perror("Error: threading catastrophy!");
    balance++; // unprotected access
    s = pthread_mutex_unlock(&lock);
    if (s != 0) 
        perror("Error: unlock.\n");
    pthread_join(p, NULL);
    return 0;
}
