#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <zconf.h>
#include <stdlib.h>
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static int zombieCounter=0;
static int killed=0;
/* Keeps track of number of zombies entered.*/
void zombieEntered(){
    zombieCounter++;
}

void zombieKilled(){
    zombieCounter--;
}
/* Returns true if number of zombies in the room are
greater than or equal to 100.*/
int tooManyZombiesInTheRoom(){
    if(zombieCounter>=100) {
        return true;
    }
    return false;
}

/*Returns true if more than 100 zombies have been killed.*/
int killed100Zotimes(){
    if(killed>=100){
        return true;
    }
    return false;
}

/* Returns true if there is at least one zombies in the room.*/
int zombiesExist(){
    if(zombieCounter>0)
        return true;
    return false;
}
/*Returns the number of zombies killed.*/
int getKilledCount(){
    return killed;
}

/* Returns the number of zombies in the room.*/
int getInTheRoomCount(){
    return zombieCounter;
}

/*doorman thread*/
void *doorMan(void *p){
    pthread_mutex_lock(&m);
    while (!( getInTheRoomCount()>100 ||getKilledCount()>100)){
        usleep(2);
        int a=(rand() % 2) + 1;
        if(a==1){ //50% chance every 2ms,
            zombieEntered();
        }
        else{
            continue;
        }
    }
    pthread_mutex_unlock(&m);
    pthread_exit(NULL);
}

/*slayer thread*/
void *slayer(void *p){
    pthread_mutex_lock(&m);
    while(zombiesExist()==1){
        usleep(2);
        killed++;
        zombieKilled();
    }
    pthread_mutex_unlock(&m);
    pthread_exit(NULL);
}

/*simulator main thread*/
int main(int argc, char **argv){
    pthread_t (*tid),B;
    int i;

    if (argc != 2)
    {
        printf("Need exactly one argument!\n");
        return 1;
    }
    int num_threads = atoi(argv[1]);

    tid = malloc(num_threads * sizeof *tid);

    for(size_t i = 0; i< num_threads; i++) {
        int rc = pthread_create(&tid[i], NULL, &doorMan, NULL);
        if(rc) { perror("create Thread"); }
    }
    i=pthread_create(&B,NULL,slayer,&i);
    if(i){perror("create Thread");}
    for(size_t i = 0;i<num_threads; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_join(B, NULL);

    if(killed100Zotimes()){
        printf("you killed 100 zombie or more \n ");
    }
    else{
        printf("zombies killed you \n");
    }
    free(tid);
    return 0;

    /*
     * ideal durum 270
     * uni@sara:~/CLionProjects/Zombie Slayer$ ./main 270
     * you killed 100 zombie
    */
}
