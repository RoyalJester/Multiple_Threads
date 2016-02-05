#include "lucy.h"
#include "main.h" 
// Function that consumes the candies, her name is big bad Lucy
void *Consumer_Lucy( void *tmp){
	DATA *consumer = (DATA *) tmp;
	struct timespec sleeptime;
	//milliseconds stored in MS
	int MS = consumer->t;
	
	sleeptime.tv_sec = MS/1000;
	sleeptime.tv_nsec = (MS%1000)*1000000;
	nanosleep(&sleeptime,NULL);
	
	char* name;
	//while loop that keeps eating the candy until they are all gone
	while(*(consumer->c_action) < *(consumer->maxsize)){
		//delay given here
		nanosleep(&sleeptime,NULL);

		sem_wait(consumer->full);		
		sem_wait(consumer->mutex);
		//check to see if max candies produced eaten
		if(*(consumer->q+*(consumer->c_action)) >= *(consumer->maxsize)){
			sem_post(consumer->mutex);
			sem_post(consumer->full);
			sem_post(consumer->empty);
			sem_post(consumer->barrier);
			return NULL;
		}
		//Deciding wether to eat a Frog bite or a Escargot Sucker
		switch(*(consumer->q+*(consumer->c_action))){
		//Case 0 is the Frog
		case 0:
			*(consumer->f) = *(consumer->f) - 1;
			*(consumer->consumed) = *(consumer->consumed) + 1;
			name = "crunchy frog bites";
			break;
		//case 1 is the Escargot
		case 1:
			*(consumer->e) = *(consumer->e) - 1;
			*(consumer->consumed+1) = *(consumer->consumed+1) + 1;			
			name = "escargot suckers";
			break;
		default:
			break;	
		}
						
		*(consumer->candycount) = *(consumer->candycount) - 1;
		*(consumer->c_action) = *(consumer->c_action) + 1;
		printf("belt: %d frogs + %d escargots = %d. produced: %d. Lucy consumed %s.\n",
			 *(consumer->f), *(consumer->e), *(consumer->candycount), *(consumer->total_p), name);
				
		sem_post(consumer->mutex);
		sem_post(consumer->empty);
	}//End while

}//end function