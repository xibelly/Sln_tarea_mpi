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
   int i;
   double  suma, producto, pi, result;
    
   suma       = 0.0;

   producto   = 1.0;
   
   for(i=istar; i<iend; i++)
     {
       
       suma     = sqrt(suma+2.0) ;
       
       producto =  producto * suma *0.5;	   
       
       pi = 2.0 / producto;

       //printf("%lf\n",pi);
       
       MPI_Reduce(&pi, &result,1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //se envia cada termino al proceso raiz
       
     }

   if(task == 0)
     {
       printf("pi1: %lf\n", result/Number_of_process);
       fflush(stdout);
     }
   
}

void pi2()
 {
   int i;
   double term1, term2, result, suma, pi, pi_out;

   suma = 0.0;
  
   for(i=istar; i<iend; i++)
     {      
       term1 = pow(-1,i);
       term2 = (2.0*i) + 1.0;
       result = term1 / term2;
       
       suma = suma + result;
       
       pi = suma * 4.0;

       MPI_Reduce(&pi, &pi_out,1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //se envia cada termino al proceso raiz
     }
   
    if(task == 0)
     {
       
       printf("pi2: %lf\n",pi_out);
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
  
  
  istar = task * (int) ceil(N/Number_of_process);
  iend = (task +1) * (int) ceil(N/Number_of_process);
  
  pi1();
  
  pi2();
  
  err = MPI_Finalize();
  
}
