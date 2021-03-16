/**
  Tonya Shulkey
  CECS 326 sec 08
  Project 1
  Due Date (2/21/21)
*/


#include <pthread.h>	//used for pthreads
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	//used for usleep
#include <ctype.h>	//used for isdigit


//Define for Sync portion
#define PTHREAD_SYNC

//The data shared by the threads
int SharedVariable = 0;

//Barrier variable
pthread_barrier_t barr;

//Shared mutex
pthread_mutex_t mutex;


//This is the thread function
void *SimpleThread(void *n){
	int num, val;
	
	//Convert the parameter void to int
	int which = *(int *)(n);
	
	for(num = 0; num < 20; num++){
	#ifdef PTHREAD_SYNC
		//Lock mutex
		pthread_mutex_lock(&mutex);
		
	#endif
		if(random() > RAND_MAX/2)
			//wait time
			usleep(500);
	
		val = SharedVariable;
		
		//print the thread id and the value it sees
		printf("*** thread %d sees value %d\n", which, val);
		
		SharedVariable = val + 1;
		
		#ifdef PTHREAD_SYNC		
			//Unlock the mutex
			pthread_mutex_unlock(&mutex);
			
		#endif	
	}
	//print the final value that is shared
	
	//Create barrier to wait till all threads have finished and are looking at the same final value.
	#ifdef PTHREAD_SYNC		
		pthread_barrier_wait(&barr);
	#endif	
	
	val = SharedVariable;
	
	
	printf("Thread %d sees final value %d\n", which, val);
	
}


int main(int argc, char *argv[]){
	
	//Check argument
	if(argc < 2){
		fprintf(stderr, "usage: a.out <integer value>\n");
		return -1;
	}
	
	//This program needs arguments in the command line
	if(atoi(argv[1]) < 0){
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}
		


	//Check if the command line is an integer otherwise end
	int num = atoi(argv[1]);

	int len = strlen(argv[1]);
	for(int i = 0; i < len; i++){
		if(!isdigit(argv[1][i])){
			printf("Invalid entry. %s is not a number. Only enter numbers", argv[1]);
			return 0;
		}
	}
	
	
	
	//Create an Array to hold the pthread IDs and one for holding the numbers
	pthread_t tidArr[num];
	int tNum[num];
	
	//Barrier Initialization
	pthread_barrier_init(&barr, NULL, num);
	
	//Mutex Initialization
	pthread_mutex_init(&mutex, NULL);

	//Create number of threads
	for(int i = 0; i < num; i++){
		tNum[i] = i;
			
		pthread_create(&tidArr[i], NULL, SimpleThread, (void *)&tNum[i]);
		
	}		

	//pthread_exit(NULL);
	
	//wait on threads to finish
	for(int i = 0; i < num; i++){
		pthread_join(tidArr[i], NULL);
	}
	
	//Clean up Mutex
	pthread_mutex_destroy(&mutex);


	return 0;
	
	
}
