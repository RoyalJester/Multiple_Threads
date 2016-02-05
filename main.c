#include "main.h"
#include "frog.h"
#include "escargot.h"
#include "lucy.h"
#include "ethel.h"

int main(int argc, char **argv){
	//initialization 
	int total = 0;
	int prod_action = 0;
	int cons_action = 0;
	int candycount = 0;
	int maxsize = 100;
	int frog = 0;
	int escargot = 0;
	int* queue = (int*)malloc(maxsize*sizeof(int));
	if(queue == NULL){
		printf("ERROR. Out of Memory");
		exit(1);	
	
	}		
	sem_t barrier;
	sem_t mutex;
	sem_t empty;
	sem_t full;
	sem_init(&barrier, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, 10);
	sem_init(&full, 0, 0);			
	DATA p_frog;
	DATA p_escargot;
	DATA c_lucy;
	DATA c_ethel;
	//To be used in getopt
	int option;
	int fflag = 0;
	int eflag = 0;
	int Lflag = 0;
	int Eflag = 0;
	int err;
	char* tmpp;
	//Checking for flags entered from command line
	while ((option = getopt(argc, argv, "f:e:L:E:")) != -1){
                switch(option){
                        case 'f' :
                                if(fflag){
                                        printf("only one allowed\n");
                                        exit(1);
                                }else{
                                        fflag++;
                                }
                                tmpp=optarg;	
				p_frog.t = atoi(tmpp);
                                break;
                        case 'e' :
                                if(eflag){
                                        printf("only one allowed\n");
                                        exit(1);
                                }else{
                                        eflag++;
                                }
				tmpp=optarg;
				p_escargot.t = atoi(tmpp);
                                break;
                        case 'L' :
				if(Lflag){
                                        printf("only one allowed\n");
                                        exit(1);
                                }else{
                                        Lflag++;
                                }
				tmpp=optarg;
				c_lucy.t = atoi(tmpp);
                                break;
                        case 'E' :
                                if(Eflag){
                                        printf("only one allowed\n");
                                        exit(1);
                                }else{
                                        Eflag++;
                                }
				tmpp=optarg;
				c_ethel.t = atoi(tmpp);
								
                                break;
                        default :
                                printf("Error\n");
					   } //End switch statement

                }//End while
	
	
	
	
	p_frog.p_action = &prod_action;
	p_frog.candycount = &candycount;
	p_frog.maxsize = &maxsize;
	p_frog.mutex = &mutex;
	p_frog.empty = &empty;
	p_frog.full = &full;
	p_frog.barrier = &barrier;
	p_frog.f = &frog;
	p_frog.e = &escargot;
	p_frog.produced = 0;
	p_frog.total_p = &total;
	
	p_escargot.p_action = &prod_action;
	p_escargot.candycount = &candycount;
	p_escargot.maxsize = &maxsize;
	p_escargot.mutex = &mutex;
	p_escargot.empty = &empty;
	p_escargot.full = &full;
	p_escargot.barrier = &barrier;
	p_escargot.f = &frog;
	p_escargot.e = &escargot;
	p_escargot.produced = 0;
	p_escargot.total_p = &total;

	c_lucy.c_action = &cons_action;
	c_lucy.candycount = &candycount;
	c_lucy.maxsize = &maxsize;
	c_lucy.mutex = &mutex;
	c_lucy.empty = &empty;
	c_lucy.full = &full;
	c_lucy.barrier = &barrier;
	c_lucy.f = &frog;
	c_lucy.e = &escargot;
	c_lucy.consumed = (int*)malloc(2*sizeof(int));
	if(c_lucy.consumed == NULL){
		printf("ERROR. Out of Memory");
		exit(1);		
	}		
	
	*(c_lucy.consumed) = 0;
	*(c_lucy.consumed+1) = 0;
	c_lucy.total_p = &total;
	
	c_ethel.c_action = &cons_action;
	c_ethel.candycount = &candycount;
	c_ethel.maxsize = &maxsize;
	c_ethel.mutex = &mutex;
	c_ethel.empty = &empty;
	c_ethel.full = &full;
	c_ethel.barrier = &barrier;
	c_ethel.f = &frog;
	c_ethel.e = &escargot;	
	c_ethel.consumed = (int*)malloc(2*sizeof(int));
	if(c_ethel.consumed == NULL){
		printf("ERROR. Out of Memory");
		exit(1);		
	}			
	*(c_ethel.consumed) = 0;
	*(c_ethel.consumed+1) = 0;
	c_ethel.total_p = &total;	

	
	p_frog.q = queue;
	p_escargot.q = queue;
	c_lucy.q = queue;
	c_ethel.q = queue;
	
	//Creating the threads
    pthread_t producer_frog;
	pthread_t producer_escargot;
    pthread_t consumer_lucy;
	pthread_t consumer_ethel;
	if((err=pthread_create(&producer_frog,NULL,(void *)Producer_Frog, &p_frog))){
		fprintf(stderr, "Failed to create thread. %s\n", strerror(err));
	}
	if((err=pthread_create(&producer_escargot,NULL,(void *)Producer_Escargot, &p_escargot))){
		fprintf(stderr, "Failed to create thread. %s\n", strerror(err));
	}
	if((err=pthread_create(&consumer_lucy,NULL,(void *)Consumer_Lucy, &c_lucy))){
		fprintf(stderr, "Failed to create thread. %s\n", strerror(err));
	}
	if((err=pthread_create(&consumer_ethel,NULL,(void *)Consumer_Ethel, &c_ethel))){
		fprintf(stderr, "Failed to create thread. %s\n", strerror(err));
	}
	sem_wait(&barrier);
	printf("PRODUCTION REPORT\n");
	printf("---------------------------------------------------------");
	printf("crunchy frog bite producer generated %d candies\n", p_frog.produced);
	printf("escargot sucker producers generated %d candies\n", p_escargot.produced);
	printf("Lucy consumed %d crunchy frog bite suckers + %d escargot suckers = %d\n",
			*(c_lucy.consumed), *(c_lucy.consumed+1), (*(c_lucy.consumed)+*(c_lucy.consumed+1))); 
	printf("Ethel consumed %d crunchy frog bite suckers + %d escargot suckers = %d\n",
			*(c_ethel.consumed), *(c_ethel.consumed+1), (*(c_ethel.consumed)+*(c_ethel.consumed+1))); 
	free(&producer_frog);
	free(&producer_escargot);
	free(c_lucy.consumed);
	free(c_ethel.consumed);
	free(&consumer_lucy);
	free(&consumer_ethel);
	free(queue);
	

}
