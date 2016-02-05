#include "frog.h"
#include "main.h"


// Function that produces crunchy frog bites
void *Producer_Frog( void* tmp){
	DATA *producer = (DATA *) tmp;
	struct timespec sleeptime;
	//milliseconds store in MS
	int MS = producer->t;
	
	sleeptime.tv_sec = MS/1000;
	sleeptime.tv_nsec = (MS%1000)*1000000;
	//While loop that keeps producing until it senses 100 candies have been made
	while(*(producer->p_action) < *(producer->maxsize)){
		//delay occurs here
		nanosleep(&sleeptime,NULL);
		//Check to make sure no more than 3 frog bites exist in the belt
		if(*(producer->f) < 3) {
			
			sem_wait(producer->empty);
			sem_wait(producer->mutex);
			//check to see if max candies produced
			if(*(producer->q+*(producer->p_action)) >= *(producer->maxsize)){
				sem_post(producer->mutex);
				sem_post(producer->empty);
				sem_post(producer->full);
				return NULL;
			}
			
			producer->produced = producer->produced +1;
			*(producer->f) = *(producer->f) + 1;
			*(producer->q+*(producer->p_action)) = 0;
			*(producer->candycount) = *(producer->candycount) + 1;
			*(producer->p_action) = *(producer->p_action) + 1;
			*(producer->total_p) = *(producer->total_p) + 1;
			printf("belt: %d frogs + %d escargots = %d. produced: %d. Added crunchy frog bites.\n",
				 *(producer->f), *(producer->e), *(producer->candycount), *(producer->total_p));
			
			
			sem_post(producer->mutex);
			sem_post(producer->full);
		}//End if	
	}//End while 
	
        pthread_exit(0);

}//End function