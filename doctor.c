#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/shm.h>
#include<string.h>
#include<semaphore.h>
sem_t pt_w,dc_a,tr_d,c;
void *shared_memory;
void consultdoc();
void treatpat();
void notetreat();

void *shared_memory;
    char buff[100];
    int shmid;

void treatpat(){
    
	shmid=shmget((key_t)2345, 1000,0666);
    shared_memory=shmat(shmid,NULL,0);
    printf("symptom is :: %s\n",shared_memory);
    



    printf("enter treatment::\n");
    read(0,buff,100); 
    strcpy(shared_memory,buff); 


}

void notetreat(){
    shmid=shmget((key_t)2345, 1000,0666);
    shared_memory=shmat(shmid,NULL,0); 
    printf("treatment is : %s\n",shared_memory);
    strcpy(shared_memory,buff);
}



void consultdoc(){
    shmid=shmget((key_t)2345, 1000,0666|IPC_CREAT); 
    shared_memory=shmat(shmid,NULL,0);
    printf("enter symptom::\n");
    read(0,buff,100); 
    strcpy(shared_memory,buff); 
}
void* patient(){
while(1){
sem_wait(&dc_a);
sem_post(&pt_w);
sem_wait(&c);

consultdoc();

sem_post(&c);
sem_wait(&tr_d);
sem_wait(&c);

notetreat();

sem_post(&c);
printf("\n==============================\n");
}
}


void* doctor(){
while(1){

sleep(1);
sem_wait(&pt_w);
sem_wait(&c);

treatpat();

sem_post(&c);
sem_post(&tr_d);
sem_post(&dc_a);

}
}


int main(){
sem_init(&pt_w,0,1);
sem_init(&dc_a,0,1);
sem_init(&tr_d,0,0);
sem_init(&c,0,1);
pthread_t doc,pat;
pthread_create(&doc,NULL,doctor,NULL);
pthread_create(&pat,NULL,patient,NULL);
pthread_join(doc,NULL);
pthread_join(pat,NULL);


}
