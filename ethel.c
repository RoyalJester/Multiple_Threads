#include "ethel.h"
#include "main.h"
//Function that consumes the candy, Her name is Ethel
void *Consumer_Ethel( void *tmp){

	DATA *consumer = (DATA *) tmp;
	struct timespec sleeptime;
	//milliseconds stored here
	int MS = consumer->t;
	
	sleeptime.tv_sec = MS/1000;
	sleeptime.tv_nsec = (MS%1000)*1000000;
	
	char* name;
	//while loop that keeps eating candy until its all gone
	while(*(consumer->c_action) < *(consumer->maxsize)){
		//delay given here
		nanosleep(&sleeptime,NULL);

		sem_wait(consumer->full);		
		sem_wait(consumer->mutex);
		//check to see if max number of candy has been consumed
		if(*(consumer->q+*(consumer->c_action)) >= *(consumer->maxsize)){
			sem_post(consumer->mutex);
			sem_post(consumer->full);
			sem_post(consumer->empty);
			sem_post(consumer->barrier);
			return NULL;
		}
		//deciding what to eat, Frogs or Escargots..they must be french
		switch(*(consumer->q+*(consumer->c_action))){
		//Frog is 0
		case 0:
			*(consumer->f) = *(consumer->f) - 1;
			*(consumer->consumed) = *(consumer->consumed) + 1;
			name = "crunchy frog bites";
			break;
		
		//Escargot is 1
		case 1:
			*(consumer->e) = *(consumer->e) - 1;
			*(consumer->consumed+1) = *(consumer->consumed+1) + 1;			
			name = "escargot suckers";
			break;
		default:
			break;	
		}//end switch
			
		*(consumer->q+*(consumer->c_action));	
		*(consumer->candycount) = *(consumer->candycount) - 1;
		*(consumer->c_action) = *(consumer->c_action) + 1;
		printf("belt: %d frogs + %d escargots = %d. produced: %d. Ethel consumed %s.\n",
			 *(consumer->f), *(consumer->e), *(consumer->candycount), *(consumer->total_p), name);				

		sem_post(consumer->mutex);
		sem_post(consumer->empty);
	}//end while
        pthread_exit(0);

}//end function
