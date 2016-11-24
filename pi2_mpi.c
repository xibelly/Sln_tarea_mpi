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


Nota: en este caso cada proceso envia un dato calculado en la operacion asignada
      y el proceso raiz recibe y suma
 */



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<mpi.h>

#define dest 0 //root process

//Varaibles globales
int N;
int err,remit;
int error, eclass, len;
char estring[MPI_MAX_ERROR_STRING];
double recvbuf=0.0;
int Number_of_process, task;
MPI_Status status;
int istar, iend;

//funciones
void pi1()
{
  int i,j;
   double  suma, producto, pi;
   double  suma2, producto2, pi2, result2;
     
   suma       = 0.0;

   producto   = 1.0;

   if(task != 0)
     {
       for(i=istar; i<iend; i++)
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
	 }
              
       MPI_Send(&pi, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD); //se envia cada termino al proceso raiz
     }

   if(task == 0)
     {
       for(remit = 1; remit < Number_of_process; remit ++)
	{
	  error= MPI_Recv(&recvbuf, 1, MPI_DOUBLE, remit, 0,MPI_COMM_WORLD, &status); //se recive cada termino 
  
	  printf("Process 0 receive number %lf to process %d\n", recvbuf, remit);fflush(stdout);


	  MPI_Error_class(error, &eclass);
	  MPI_Error_string(error, estring, &len);
	  printf("Error %d:%s\n",eclass, estring);
 
	  fflush(stdout);

	}
   
       printf("MODE 1 -> pi1: %lf\n",recvbuf);
       fflush(stdout);
     }
   
}

void pi2()
{
  int i,j;
   double term1, term1_0, term2, term2_0, result, result0, suma, suma0, pi, pi0, suma_total;
   double term1_2, term2_2, result2, suma2, pi2;
   
   suma = 0.0;

   if(task != 0) //identifica el proceso que va a ejecutar la instrucción
        {
	  for(i=istar; i<iend; i++)
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
	      
	    }
	  
	  MPI_Send(&pi, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD); //se envia cada termino al proceso raiz
	  
	}

   
    if(task == 0)
     {
       for(remit = 1; remit < Number_of_process; remit ++)
	{
	  error= MPI_Recv(&recvbuf, 1, MPI_DOUBLE, remit, 0,MPI_COMM_WORLD, &status); //se recive cada termino 
  
	  printf("Process 0 receive number %lf to process %d\n", recvbuf, remit);fflush(stdout);

	  MPI_Error_class(error, &eclass);
	  MPI_Error_string(error, estring, &len);
	  printf("Error %d:%s\n",eclass, estring);
 
	  fflush(stdout);


	  term1_0 = pow(-1,task);
	  term2_0 = (2.0*task) + 1.0;
	  result0 = term1_0 / term2_0;
	      
	  suma0 = suma0 + result0;
	  
	  pi0 = suma * 4.0;

	}
       
       suma_total = suma + recvbuf;

       printf("MODE2 -> pi2: %lf\n",suma_total);
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

  istar = 0;
  iend =  (int) ceil(N/Number_of_process);

  pi1();
  
  pi2();
  
  err = MPI_Finalize();
  
}
