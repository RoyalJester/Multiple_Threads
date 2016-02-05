A03: 		main.o frog.o escargot.o lucy.o ethel.o
			gcc main.o frog.o escargot.o lucy.o ethel.o -o A03 -lpthread -lrt
	
main.o:		main.c main.h frog.h escargot.h lucy.h ethel.h
			gcc -c main.c

frog.o: 	frog.c frog.h
			gcc -c frog.c
	
escargot.o:	escargot.c escargot.h
			gcc -c escargot.c
		
lucy.o:		lucy.c lucy.h
			gcc -c lucy.c
		
ethel.o:	ethel.c ethel.h
			gcc -c ethel.c
			
clean:		
			rm -rf *o A03