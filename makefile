CC = mpicc

CFLAGS = -c -I/home/$(USER)/local/include/ -I/usr/include/

CFLAGSDEBUG = -g -Wall -c -I/home/$(USER)/local/include/ -I/usr/include/

LFLAGS = -lm -L/home/$(USER)/local/lib -Wl,-R /home/$(USER)/local/lib 


factorial:
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o

seno_mpi:
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o
	$(CC) factorial.o $@.o $(LFLAGS) -lm -o  $@.x	#factorial.o	
coseno_mpi:
	echo Estoy compilando $@.c
	$(CC) $(CFLAGS) $@.c -o $@.o
	$(CC) factorial.o $@.o $(LFLAGS) -lm -o  $@.x
debug: 
	echo Estoy compilando para debugear $@.c
	$(CC) $(CFLAGSDEBUG) $@.c -o $@.o
	$(CC) $@.o $(LFLAGS)  -o $@.x


clean:
	rm -rf *.~
	rm -rf *.out
	rm -rf *#
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm *.x
