/* Xibelly Eliseth Mosquera Escobar - Lunes 21 de Nov de 2016

 2) Escribir un programa que utilice la serie de taylor de la funcion cos(alrededor de x=0) para calcular el cos de un angulo. El programa debe recibir como entrada el valor del angulo (x) en grados y en el numero de terminos (N) de la serie a usarse en la expansion, y retornar en pantalla el valor de sen(x). Debe de correr con un # indeterminado de procesadores.

 */

/*
Analisis y diseño

Para resolver el problema de calcular el cos de un # usando la serie de taylor centrada en x=0
 
  cos(x) = sum_n=0^inf ( (-1)^n x^{2n} )/ (2n)!                                 (1)

Necesitamos:

-recivir el # en grados y pasarlo a radianes, para ello lo resiviremos por linea de comandos

-crear una funcion que calcule el factorial de un numero

-que al calcular cada termino de la ecu (1) estos se vayan sumando, tal que el resultado sea la suma de  todas las iteraciones hechas

-retornar en pantalla el valor del cos del numero que se pasa por linea de comandos

Como se va hacer:

 para resolver nuestro problema vamos hacer que cada procesador calcule un conjunto de terminos de la suma, es decir, 

   task 0 -> termino i

   task 1 -> termino k
   .
   .
   .

   task N -> termino j

Donde cada resultado de los diferentes procesos se agrupa en un solo proceso y estos a su vez se suman entre asi haciendo uso de un 
redude -> MPI_SUM. cada termino lo  enviamos al proceso raiz, este recibe cada termino y luego suma.

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

//Sub-rutinas
#include"factorial.h"


int main(int argc, char **argv)
{
  int i,N;
  
  double angulo, numerador, term, denominador, suma=0.0, cos_x, radianes;

  int err, dest, remit;
  int error, eclass, len;
  char estring[MPI_MAX_ERROR_STRING];
  double recvbuf=0.0;
  int Number_of_process, task;
  MPI_Status status;

  int istar, iend;

  
  //Se inicializa MPI
  
  err = MPI_Init(&argc, &argv); 
  
  MPI_Comm_size(MPI_COMM_WORLD, &Number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &task); 
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  
  if(argc != 3)
    {
      printf("ERROR--> use as:\n");
      printf("%s #terms angle \n",argv[0]);
      exit(0);  
    }

  //Carga de parametros

  N = atoi(argv[1]);
  angulo   = atof(argv[2]);
  radianes = angulo*M_PI/180.0;
  istar = task * (int) ceil(N/Number_of_process);
  iend = (task +1) * (int) ceil(N/Number_of_process);

  if(N < 0)
    printf("ERROR: NEGATIVE ORDER\n");

   
  dest = 0;//proceso raiz
  
  //Calculo cos(x)

  for(i=istar; i<iend; i++)
    {
      term         = (2*i);
	  
      numerador    = pow(-1,i) * pow(radianes,term);
	  
      denominador  = factorial(term);    
	  
      cos_x       += numerador / denominador; 

      MPI_Reduce(&cos_x, &suma, 1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //se calcula el factorial total
      
    }

  if(task == 0)
    {
       printf("sen( %lf grados) = %f \n",angulo,suma);
       fflush(stdout);
    }

      
   err = MPI_Finalize();
}

 
  
