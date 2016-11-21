/* Xibelly Eliseth Mosquera Escobar - Lunes 21 de Nov de 2016

 1) Escribir un programa que utilice la serie de taylor de la funcion seno(alrededor de x=0) para calcular el seno de un angulo. El programa debe recibir como entrada el valor del angulo (x) en grados y en el numero de terminos (N) de la serie a usarse en la expansion, y retornar en pantalla el valor de sen(x). Debe de correr con un # indeterminado de procesadores.

 */

/*
Analisis y diseño

Para resolver el problema de calcular el seno de un # usando la serie de taylor centrada en x=0
 
  sen(x) = sum_n=0^inf ( (-1)^n x^{2n+1} )/ (2n+1)!                                 (1)

Necesitamos:

-recivir el # en grados y pasarlo a radianes, para ello lo resiviremos por linea de comandos

-crear una funcion que calcule el factorial de un numero

-que al calcular cada termino de la ecu (1) estos se vayan sumando, tal que el resultado sea la suma de  todas las iteraciones hechas

-retornar en pantalla el valor del seno del numero que se pasa por linea de comandos

Como se va hacer:

 para resolver nuestro problema vamos hacer que cada procesador calcule un termino de la suma, es decir, #procesos = #de iteraciones
   asi:

   task 0 -> termino 0

   task 1 -> termino 1
   .
   .
   .

   task N -> termino N

Tenemos dos modos de hacer dicho calculo -> 

Modo1) calculamos cada termino y hacemos un reduce al proceso raiz

Modo 2) calculamos cada termino envioamos al proceso raiz, este recibe cada termino y luego suma.

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

//Sub-rutinas
#include"factorial.h"


int main(int argc, char **argv)
{

  double angulo, numerador, numerador0, term, term0, denominador, denominador0, suma=0.0, seno_x,seno_x0, radianes, suma2=0.0;

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
  
  angulo   = atof(argv[1]);
  radianes = angulo*M_PI/180.0;


  //Calculo sen(x) Modo 1 
  
  dest = 0;      
          
  term         = (2*task) + 1;
  
  numerador    = pow(-1,task) * pow(radianes,term);
  
  denominador  = factorial(term);    //Se llama a la funcion factorial
  
  seno_x       = numerador / denominador; 
  
      
  MPI_Reduce(&seno_x, &suma2, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD); //Se calcula el reduce

  
      
   if(task == 0)	
    {
      printf("MODE 1\n");
      printf("sen( %lf grados) =  %f\n",angulo,suma2);
      
      fflush(stdout);
    }


   //Calculo sen(x) Modo 2

    if(task != 0) //identifica el proceso que va a ejecutar la instrucción
        {
     
	  dest = 0;      
          
	  term         = (2*task) + 1;
	  
	  numerador    = pow(-1,task) * pow(radianes,term);
	  
	  denominador  = factorial(term);    
	  
	  seno_x       = numerador / denominador; 
	  
	  MPI_Send(&seno_x, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD); //se envia cada termino al proceso raiz
	}

    if(task == 0)	
    {
      printf("MODE 2\n");
      
      for(remit = 1; remit < Number_of_process; remit ++)
	{
	  error= MPI_Recv(&recvbuf, 1, MPI_DOUBLE, remit, 0,MPI_COMM_WORLD, &status); //se recive cada termino 

	  
	  printf("Process 0 receive number %lf to process %d\n", recvbuf, remit);


	  MPI_Error_class(error, &eclass);
	  MPI_Error_string(error, estring, &len);
	  printf("Error %d:%s\n",eclass, estring);
 
	  fflush(stdout);
	  
	  term0         = (2*task) + 1; //se calcula el termino 0 de la serie, respectivo del proceso 0
	  
	  numerador0    = pow(-1,task) * pow(radianes,term0);
	  
	  denominador0  = factorial(term0);    
	  
	  seno_x0       = numerador0 / denominador0;
	  
	  suma   = suma+recvbuf;
	}

       suma = suma + seno_x0 ;

       
       printf("sen( %lf grados) = %f \n",angulo,suma);
       fflush(stdout);

    }

    
   err = MPI_Finalize();
}

 
  
