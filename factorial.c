/*
  Xibelly Eliseth Mosquera Escobar
  
  Este programa consiste en diseñar la funcion factorial de un numero n cualquiera.
  

*/

double factorial(double n)
{
  double i, p;

  p = 1.0;
  for(i=1.0; i<=n; i ++)
    {
      p = p*i;
    }
  
  return p;
}
