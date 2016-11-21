/* Xibelly Eliseth Mosquera Escobar

4) Escriba un programa que evalúe (en una rutina diferente al main) la función

       f(x)=sin(x)*ln(2*x+1)*sinh(x) en el intervalo [0, 10]   1)

El programa debe recibir el tamaño del paso (dx) entre evaluaciones sucesivas de la función.

El resultado se debe escribir en un archivo de tres columnas. La primer columna conteniendo el valor i del incremento en x (x=i*dx), la segunda columna debe contener x, y la tercera conteniendo el valor de f(x). Cada procesador debe acceder el respectivo archivo en el disco duro.
 */

/*
  Analisis y diseño

 Para resolver el problema de calcular el valor de f(x), necesitamos:

-recivir el tamaño del paso, por linea de comandos
 
-crear una funcion que calcule 1)

-imprimir en disco los valor de i, x= i*dx y f(x)

 */



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<mpi.h>


//Varaibles globales
int err, dest, remit;
int error, eclass, len;
char estring[MPI_MAX_ERROR_STRING];
double recvbuf=0.0;
int Number_of_process, task;
MPI_Status status;


//funciones

void fdex(double dx)
 {
   int i,n;
   double x, f_x;
   double term1, term2, term3, result;

   char buff[200];
   FILE *write =NULL;

   n = 10;

   sprintf(buff,"file_%d.dat",task);
   
   write = fopen(buff,"w");
   
   if(write==NULL)
    printf("THE FILE CAN NOT BE OPENED\n");
      
   for(i=0; i<n; i++)
     {
       x = i * dx;
       
       term1 = sin(x);
       term2 = log10(2*x+1);
       term3 = sinh(x);
       result = term1 * term2 * term3;

       fprintf(write,"%d %lf %lf\n",i,x,result);
 
     }
	 
 }



///////////////////////////////Programa principal////////////////////
int main(int argc, char **argv){

  double dx;
  
  //Se inicializa MPI
  
  err = MPI_Init(&argc, &argv); 
  
  MPI_Comm_size(MPI_COMM_WORLD, &Number_of_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &task); 
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  
  if(argc != 2)
    {
      printf("ERROR--> use as:\n");
      printf("%s step_size n",argv[0]);
      exit(0);  
    }

  dx = atof(argv[1]);
  
  fdex(dx);
 
  err = MPI_Finalize();
  
}
