/* Xibelly Eliseth Mosquera Escobar

5) Escriba un programa que calcule el numero p usando dos aproximaciones diferentes

pi1 ­> 2/π = (√2) / 2 . (√(2+ √2) )/2 . √(2+ (√(2 + √2)))/2 ...

pi2 ­> π/4 =1 −1/3  + 1/5  − 1/7  + 1/9 − 1/11 + ...
 
Calcule p en cada aproximación implementándola en una función distinta en cada caso (en el mismo
programa). El programa debe recibir el numero de términos en las expansiones a usarse para el calculo
de p y debe imprimir en pantalla los dos valores de p obtenidos.
 */

/*
  Analisis y diseño

 Para resolver el problema de calcular el valor de pi a partir de las dos aproximacioned, necesitamos:

-recivir el numero de terminos en la expansion, por linea de comandos
 
-implementar cada aproximacion en una funcion distinta

-imprimir en pantalla los valores de pi en cada caso

Nota: para ello cada proceso calcula un numero de terminos de la expasion y luego se 

Como se va hacer:

 para resolver nuestro problema vamos hacer que cada procesador calcule un conjunto de terminos, es decir: 

   task 0 -> termino i

   task 1 -> termino k
   .
   .
   .

   task N -> termino j

Donde cada resultado de los diferentes procesos se agrupa en un solo proceso --el root- y estos a su vez se suman entre asi haciendo uso de un 
redude -> MPI_SUM


NOta: en el caso en que sobran terminos y no hay procesadores, lo que se hace es que el ultimo proceso toma los terminos restantes
      y calcula el correspondiente producto.



 */



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<mpi.h>

#define dest 0 //root process

//Varaibles globales
int N;
int err;
int error, eclass, len;
char estring[MPI_MAX_ERROR_STRING];
int Number_of_process, task;
MPI_Status status;
int istar, iend;

//funciones
void pi1()
{
  int i,j;
   double  suma, producto, pi, result;
   double  suma2, producto2, pi2, result2;
    
   suma       = 0.0;

   producto   = 1.0;
   
   for(i=istar; i<iend; i++)//calculo de los primeros en los primeros tasks
     {
       
       suma     = sqrt(suma+2.0) ;
       
       producto =  producto * suma *0.5;	   
       
       pi = 2.0 / producto;

       if(task == (Number_of_process - 1))//calculo de los ultimos terminos en el ultimo task
	 {
	   iend = N + 1;
	   
	   for(j=istar; j<iend; j++)
	     {
	        
	       suma2     = sqrt(suma2+2.0) ;
	       
	       producto2 =  producto2 * suma2 *0.5;	   
	       
	       pi2 = 2.0 / producto2;
	     }

	 }
       
       MPI_Reduce(&pi, &result,1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //se envia cada termino al proceso raiz
       
     }

   if(task == 0)
     {
       printf("MODE 1 -> pi1: %lf\n", result/Number_of_process);
       fflush(stdout);
     }
   
}

void pi2()
 {
   int i,j;
   double term1, term2, result, suma, pi, pi_out;
   double term1_2, term2_2, result2, suma2, pi2;

   suma = 0.0, suma2 = 0.0;
  
   for(i=istar; i<iend; i++)//calculo de los primeros en los primeros tasks
     {      
       term1 = pow(-1,i);
       term2 = (2.0*i) + 1.0;
       result = term1 / term2;
       
       suma = suma + result;
       
       pi = suma * 4.0;

       if(task == (Number_of_process - 1))//calculo de los ultimos terminos en el ultimo task
	 {
	   iend = N + 1;
	  
	  for(j=istar; j<iend; j++)
	    {
	      term1_2 = pow(-1,i);
	      term2_2 = (2.0*i) + 1.0;
	      result2 = term1 / term2;
       
	      suma2 = suma2 + result2;
       
	      pi2 = suma2 * 4.0;

	    }

	 }
       
       MPI_Reduce(&pi, &pi_out,1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //se envia cada termino al proceso raiz
     }
   
    if(task == 0)
     {
       
       printf("MODE 2 -> pi2: %lf\n",pi_out);
       fflush(stdout);
     }
 }



///////////////////////////////Programa principal////////////////////
int main(int argc, char **argv){

    
  //Se inicializa MPI
  
  err = MPI_Init(&argc, &argv); 
  
  MPI_Comm_size(MPI_COMM_WORLD, &Number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &task); 
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  
  if(argc != 2)
    {
      printf("ERROR--> use as:\n");
      printf("%s number_of_terms n",argv[0]);
      exit(0);  
    }

  N = atoi(argv[1]);

  if(N < 0)
    printf("ERROR: NEGATIVE ORDER\n");

  if(N < Number_of_process)
     printf("ERROR: A process needs at least one data\n");
   
    
  istar = task * (int) ceil(N/Number_of_process);
  iend = (task +1) * (int) ceil(N/Number_of_process);
  
  pi1();
  
  pi2();
  
  err = MPI_Finalize();
  
}
