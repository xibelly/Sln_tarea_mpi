/* Xibelly Eliseth Mosquera Escobar

 10) Escriba un programa que calcule el numero p usando dos aproximaciones diferentes

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


//Varaibles globales
int err, dest, remit;
int error, eclass, len;
char estring[MPI_MAX_ERROR_STRING];
double recvbuf=0.0;
int Number_of_process, task;
MPI_Status status;

//funciones
void pi1(int n)
{
   int i;
   double  suma, producto, pi;

     
   suma       = 0.0;

   producto   = 1.0;

   if(task != 0)
     {
       for(i=0; i<n; i++)
	 {
	   dest = 0;
	   
	   suma     = sqrt(suma+2.0) ;
	   
	   producto =  producto * suma *0.5;	   

	   pi = 2.0 / producto;
	   
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
   
       printf("pi1: %lf\n",recvbuf);
       fflush(stdout);
     }
   
}

void pi2(int n)
 {
   int i;
   double term1, term2, result, suma, pi, pi2, pi_out, out;

   suma = 0.0;

   //////////////////////////////////////modo 1////////////////////////////////////////////

   /*cada proceso calcula la operacion indicada y la envia al proceso raiz -> 0*/
   
   if(task != 0) //identifica el proceso que va a ejecutar la instrucción
        {
     
	  for(i=0; i<n; i++)
	    {
	      dest = 0;
	      
	      term1 = pow(-1,i);
	      term2 = (2.0*i) + 1.0;
	      result = term1 / term2;
	      
	      suma = suma + result;
	      
	      pi = suma * 4.0;
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
   
       printf("MODE1 -> pi2: %lf\n",recvbuf);
       fflush(stdout);
     }

    
    ///////////////////////////////////////////////modo 2///////////////////////////////////////////////

    /*cada proceso calcula un termino de la expansion-> #terminos = #procesos

      task 0 -> termino 0

      task 1 -> termino 1
      .
      .
      .
      
      task N -> termino N

     */

    dest = 0;
    
    term1 = pow(-1,task);
    term2 = (2.0*task) + 1.0;
    result = term1 / term2;
	  
    MPI_Reduce(&result, &out, 1, MPI_DOUBLE, MPI_SUM, dest,MPI_COMM_WORLD); //Se suma cada termino de la expasion


    if(task == 0)	
      {
	pi_out = out * 4.0;
	
	printf("MODE 2 ->pi2 %f\n",pi_out);
	
	fflush(stdout);
      }

 }



///////////////////////////////Programa principal////////////////////
int main(int argc, char **argv){

  int N;
   
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
  
  pi1(N);
  
  pi2(N);
  
  err = MPI_Finalize();
  
}
