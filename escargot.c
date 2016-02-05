#include "escargot.h"
#include "main.h"

//function that produces Everlasting Escargot Suckers
void *Producer_Escargot( void* tmp){
	DATA *producer = (DATA *) tmp;
	struct timespec sleeptime;
	//MS contains milliseconds
	int MS = producer->t;
	
	sleeptime.tv_sec = MS/1000;
	sleeptime.tv_nsec = (MS%1000)*1000000;
	//While loops that keeps producing the Escargots
	while(*(producer->p_action) < *(producer->maxsize)){
		//Delay is given here
		nanosleep(&sleeptime,NULL);
		sem_wait(producer->empty);
        sem_wait(producer->mutex);
		//Checks if max candies have been produced
		if(*(producer->q+*(producer->p_action)) >= *(producer->maxsize)){
			sem_post(producer->mutex);
			sem_post(producer->empty);
			sem_post(producer->full);
			return NULL;
		}
		producer->produced = producer->produced +1;
		*(producer->e) = *(producer->e) + 1;		
		*(producer->q+*(producer->p_action)) = 1;				
		*(producer->candycount) = *(producer->candycount) + 1;
		*(producer->p_action) = *(producer->p_action) + 1;
		*(producer->total_p) = *(producer->total_p) + 1;		
		printf("belt: %d frogs + %d escargots = %d. produced: %d. Added escargot sucker.\n",
			 *(producer->f), *(producer->e), *(producer->candycount), *(producer->total_p));
				
		sem_post(producer->mutex);
		sem_post(producer->full);
		
	}//End while
	
        pthread_exit(0);

}//End Function