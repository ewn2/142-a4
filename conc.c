#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

int ChefMake = 0;
int Servings = 0;
int Cust1Scarfs = 0;
int Cust2Scarfs = 0;
int Cust3Scarfs = 0;
int Customer = 0;

pthread_mutex_t Bell = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ChefSem = PTHREAD_COND_INITIALIZER;
pthread_cond_t Ender = PTHREAD_COND_INITIALIZER;
pthread_cond_t Fries = PTHREAD_COND_INITIALIZER;
pthread_cond_t Hamburger = PTHREAD_COND_INITIALIZER;
pthread_cond_t Soda = PTHREAD_COND_INITIALIZER;

void *Chef(){
    Customer = 0;
    while(Servings <= 100) {
        Servings++;
        pthread_mutex_lock(&Bell);
        while(ChefMake != 0) {
            pthread_cond_wait(&ChefSem, &Bell);
        }
        ChefMake = ((rand() % (3)) + 1);
        if(ChefMake == 1 && Servings <= 100) {
            printf("Chef Produces Meal #%d: Fries and Soda and falls asleep\n", Servings);
            pthread_cond_signal(&Hamburger);
            sleep(1);
        }
        if(ChefMake == 2 && Servings <= 100) {
            printf("Chef Produces Meal #%d: Soda and Hamburger and falls asleep\n", Servings);
            pthread_cond_signal(&Fries);
            sleep(1);
        }
        if(ChefMake == 3 && Servings <= 100) {
            printf("Chef Produces Meal #%d: Fries and Hamburger and falls asleep\n", Servings);
            pthread_cond_signal(&Soda);
            sleep(1);
        }
        pthread_mutex_unlock(&Bell);
    }
    pthread_cond_signal(&Ender);
}

void *Finish(){
    pthread_mutex_lock(&Bell);
    pthread_cond_wait(&Ender, &Bell);
    printf("\nKitchen Closed\n\n");
    printf("Customer 1 ate %d times\n", Cust1Scarfs);
    printf("Customer 2 ate %d times\n", Cust2Scarfs);
    printf("Customer 3 ate %d times\n", Cust3Scarfs);
    if(Cust1Scarfs >= 40) {
        printf("Customer 1 is a greedy little thing, ain't he?\n");
    }
    if(Cust2Scarfs >= 40) {
        printf("Customer 2 is a greedy little thing, ain't he?\n");
    }
    if(Cust3Scarfs >= 40) {
        printf("Customer 3 is a greedy little thing, ain't he?\n");
    }
    exit(0);
}

void *Customer1(){
    while(Servings <= 100){
        pthread_mutex_lock(&Bell);
        while(ChefMake != 1) {
            pthread_cond_wait(&Hamburger, &Bell);
        }
        Customer = 1;
        printf("Customer #%d gets Fries and Soda, combines with their own Hamburger\n", Customer);
        pthread_cond_signal(&ChefSem);
        ChefMake = 0;
        pthread_mutex_unlock(&Bell);
        printf("Customer #%d eats full meal and rings bell\n", Customer);
        Cust1Scarfs++;
    }
}

void *Customer2() {
    while(Servings <= 100) {
        pthread_mutex_lock(&Bell);
        while(ChefMake != 2) {
            pthread_cond_wait(&Fries, &Bell);
        }
        Customer = 2;
        printf("Customer #%d gets Soda and Hamburger, combines with their own Fries\n", Customer);
        pthread_cond_signal(&ChefSem);
        ChefMake = 0;
        pthread_mutex_unlock(&Bell);
        printf("Customer #%d eats full meal and rings bell\n", Customer);
        Cust2Scarfs++;
    }
}

void *Customer3(){
    while(Servings <= 100){
        pthread_mutex_lock(&Bell);
        while(ChefMake != 3) {
            pthread_cond_wait(&Soda, &Bell);
        }
        Customer = 3;
        printf("Customer #%d gets Fries and Hamburger, combines with their own Soda\n", Customer);
        pthread_cond_signal(&ChefSem);
        ChefMake = 0;
        pthread_mutex_unlock(&Bell);
        printf("Customer #%d eats full meal and rings bell\n", Customer);
        Cust3Scarfs++;
    }
}

int main() {
    pthread_t ChefThread, EnderThread, Customer1Thread, Customer2Thread, Customer3Thread;
    srand(time(NULL));
    pthread_create(&ChefThread,NULL,Chef,NULL);
    pthread_create(&EnderThread,NULL,Finish,NULL);
    pthread_create(&Customer1Thread,NULL,Customer1,NULL);
    pthread_create(&Customer2Thread,NULL,Customer2,NULL);
    pthread_create(&Customer3Thread,NULL,Customer3,NULL);
    pthread_join(ChefThread, NULL);
    pthread_join(EnderThread, NULL);
    pthread_join(Customer1Thread, NULL);
    pthread_join(Customer2Thread, NULL);
    pthread_join(Customer3Thread, NULL);
}