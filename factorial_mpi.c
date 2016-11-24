/* Xibelly Eliseth Mosquera Escobar - Lunes 21 de Nov de 2016

 3) Escriba un programa que calcule el factorial de un numero x.

 */

/*
Analisis y diseño

Para resolver el problema de calcular el seno de un # usando la serie de taylor centrada en x=0
 
            n ! = (n -1)* n                      (1)

Necesitamos:

-recivir el numero al que se le va a calcular el factorail, para ello lo resiviremos por linea de comandos

-calcular cada termino de la ecu (1) y que estos se vayan sumando, tal que el resultado sea la suma de  todas las iteraciones hechas

-retornar en pantalla el valor del seno del numero que se pasa por linea de comandos

Como se va hacer:

 para resolver nuestro problema vamos hacer que cada procesador calcule un conjunto de terminos del factorial, es decir, 

   task 0 -> termino i

   task 1 -> termino k
   .
   .
   .

   task N -> termino j

Donde cada resultado de los diferentes procesos se agrupa en un solo proceso y estos a su vez se multiplican entre asi haciendo uso de un 
redude -> MPI_PROD


NOta: en el caso en que sobran terminos y no hay procesadores, lo que se hace es que el ultimo proceso toma los terminos restantes
      y calcula el correspondiente producto.


*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>


int main(int argc, char **argv)
{

  int i,j;
  double x, fact_x2, fact_x, fact, result;

  int err, dest, remit;
  int error, eclass, len;
  char estring[MPI_MAX_ERROR_STRING];
  double recvbuf=0.0;
  int Number_of_process, task;
  MPI_Status status;
  int min, max, max2;

  //Se inicializa MPI
  
  err = MPI_Init(&argc, &argv); 
  
  MPI_Comm_size(MPI_COMM_WORLD, &Number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &task); 
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  
  if(argc != 2)
    {
      printf("ERROR--> use as:\n");
      printf("%s # \n",argv[0]);
      exit(0);  
    }

  //Carga de parametros
  
  x   = atof(argv[1]);
  min = (task *(int) floor(x/Number_of_process) ) + 1; 
  max = ((task + 1) * (int) floor(x/Number_of_process)) +1 ;

    /* x/Number_of_proces -> establece el # de terminos 
     a calcularse la productoria para cada proceso y 
     min y max el intervalo en que se va calcular
    */
  
  dest = 0;

  //Calculo x! 
  fact_x = 1.0;
  fact_x2 = 1.0;


  for(i=min; i<max; i++)
    {
      
      fact_x  = fact_x * 1.0 * i ;  //producto de los primeros terminos en los primeros procesos

      if(task == (Number_of_process - 1))  //producto de los ultimos terminos en el ultimo proceso caso en que sobran  terminos
	{
	 
	  max = x + 1;
	  for(j=min; j<max; j++)
	    {
	      fact_x2  = fact_x2 * 1.0 * j ;
	    }
	  
	  
	  //MPI_Send(&fact_x2, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD); //se envia el producto al proceso raiz 
	} 
      
    }
  printf("task %d computes %lf -> %d\n",task,fact_x, (int)x);
  MPI_Reduce(&fact_x, &fact, 1, MPI_DOUBLE, MPI_PROD, dest,MPI_COMM_WORLD); //se hace un reduce en estos procesos
       
    
  
  if(x==0)  //caso 0!
    fact = 1.0;
 
  
  if(task == 0)	//calculo del factorial en le proceso raiz
    {
   
      printf("(%lf)! = %lf\n",x,fact);
      
    }

 
  
  
   err = MPI_Finalize();
}

 
  
