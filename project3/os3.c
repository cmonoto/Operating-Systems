/*
JinhaoChen
Assignment 3*/
#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
//SET BUFFER SIZE
#define BUFFERSIZE 15

int count =0;  //Character counter
int i = 0; //index of producer
int j = 0; //index of consumer
char buffer[BUFFERSIZE];    //Buffer


//SEMAPHORS
sem_t  buffer_full;
sem_t  buffer_empty;
sem_t  mutex;


//PRODUCER FUNCTION
void *producer(){
	//Prepare for file Input
	char newChar;
	FILE* fp;
	fp= fopen("mytest.dat", "r");
	while(1){
		sem_wait(&buffer_empty);
		sem_wait(&mutex);
		//Loop continuously until the end of the file
		if(fscanf(fp,"%c",&newChar) != EOF){
			buffer[i] = newChar;			
			i = (i + 1)%BUFFERSIZE;
		}
		else{
	    //Places an asterisk in the buffer when the end of file is reached
	                buffer[i] = '*';  
             //release the buffer
			sem_post(&mutex);
			sem_post(&buffer_full);			
			break;
		}
		sem_post(&mutex);
		sem_post(&buffer_full);
	}
	fclose(fp);
}


//CONSUMER FUNCTION
void *consumer(){ 
	while(1){
		sleep(1);		            //delay for consumer
		sem_wait(&buffer_full);
		sem_wait(&mutex);		
		fflush(stdout);             //flushes out 
		//Loops continuously until an asterisk is found in the buffer
		if(buffer[j] !='*'){   			
			printf("%c", buffer[j]);
			j = (j + 1)%BUFFERSIZE;
			if(buffer[j] != '\n') 
	                   count = count + 1;  
                         //New lines not considered as a character		
		}
		else{
			sem_post(&mutex);
			sem_post(&buffer_empty);			
			break;
		}
		sem_post(&mutex);
		sem_post(&buffer_empty);
	 }
}

main()
{

    pthread_t tid1[1];     /* process id for thread 1 */
    pthread_t tid2[1];     /* process id for thread 2 */
    pthread_attr_t attr[1];     /* attribute pointer array */
	
	//Initialize sempahors
	sem_init(&buffer_full, 0, 0);  	
	sem_init(&buffer_empty, 0, BUFFERSIZE);
	sem_init(&mutex, 0, 1);

    
	fflush(stdout);
    /* Required to schedule thread independently.*/
    pthread_attr_init(&attr[0]);
    pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  
	/* end to schedule thread independently */

    /* Create the threads */
    pthread_create(&tid1[0], &attr[0], producer, NULL);
    pthread_create(&tid2[0], &attr[0], consumer, NULL);
	
//	printf("\t\t	File content:\n");
//	printf("------------------------------------------------\n");

    /* Wait for the threads to finish */
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
    
    //Destroy sempahors
	sem_destroy(&buffer_full);  	
	sem_destroy(&buffer_empty);
	sem_destroy(&mutex);
	printf("\n------------------------------------------------\n");
	printf("From parent counter  =  %d\n", count);
	printf("------------------------------------------------\n");
	printf("\t\t	End of simulation\n");

	exit(0);		
}
