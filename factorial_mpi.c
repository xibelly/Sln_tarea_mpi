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

 para resolver nuestro problema vamos hacer que cada procesador calcule un termino de la suma, es decir, #procesos = #de iteraciones
   asi:

   task 0 -> 0!

   task 1 -> 1!
   .
   .
   .

   task N -> N!

Hay dos modos:

Modo 1 -> con un reduce

Modo 2 -> con cada proceso

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

//Sub-rutinas
#include"factorial.h"


int main(int argc, char **argv)
{

  double x, fact1_x, fact_x, fact, suma=0.0;

  int err, dest, remit;
  int error, eclass, len;
  char estring[MPI_MAX_ERROR_STRING];
  double recvbuf=0.0;
  int Number_of_process, task;
  MPI_Status status;


  //Se inicializa MPI
  
  err = MPI_Init(&argc, &argv); 
  
  MPI_Comm_size(MPI_COMM_WORLD, &Number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &task); 
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  
  if(argc != 2)
    {
      printf("ERROR--> use as:\n");
      printf("%s angulo \n",argv[0]);
      exit(0);  
    }

  //Carga de parametros
  
  x   = atof(argv[1]);
 

  //Calculo x! modo 1

  fact1_x = factorial(x);

  printf("MODE 1 ->  (%lf)! = %f\n",x, fact1_x);
  fflush(stdout);

   //Calculo Modo 2

  if(task != 0) //identifica el proceso que va a ejecutar la instrucción
    {
      dest = 0;
      
      fact_x  = factorial(x);    
      
      MPI_Send(&fact_x, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD); //se envia cada termino al proceso raiz
    }
  
  if(task == 0)	
    {
      
      for(remit = 1; remit < Number_of_process; remit ++)
	{
	  error= MPI_Recv(&recvbuf, 1, MPI_DOUBLE, remit, 0,MPI_COMM_WORLD, &status); //se recive cada termino 
	  	  
	  printf("Process 0 receive number (%lf)! = %lf to process %d\n", x, recvbuf, remit);
	 
	  
	  
	  MPI_Error_class(error, &eclass);
	  MPI_Error_string(error, estring, &len);
	  printf("Error %d:%s\n",eclass, estring);
	  
	  

	  fact = (recvbuf * x) / x;
	  
	}
      
      
      printf("MODE 2 -> (%lf)! = %f\n",x, fact);
      fflush(stdout);
    }
  
   err = MPI_Finalize();
}

 
  
