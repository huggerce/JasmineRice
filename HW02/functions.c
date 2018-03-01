
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"



//compute a*b mod p safely
unsigned int modProd(unsigned int a, unsigned int b, unsigned int p) {
  /* Q1.2: Complete this function */

	unsigned int za = a%p;
	unsigned int ab = 0;

	while(b > 0)
	{
		//check if b is odd
		if (b % 2 == 1)
			ab = (ab + a) % p;
		//otherwise
		a = (a*2) % p;
		b = b / 2;
	}
	return ab % p;
	
}

//compute a^b mod p safely
unsigned int modExp(unsigned int a, unsigned int b, unsigned int p) {
  /* Q1.3: Complete this function */

	unsigned int z = a;
	unsigned int aExpb = 1;
	z = z%p;
	while(b>0)
	{
		//check if b is odd
		if (b % 2 == 1)
		{
			aExpb = modProd(aExpb, z, p);
			//(aExpb * a) % p;
		}
		
		b = b/2;
		z = modProd(z,z,p);
		//(a*a) % p;
		
		//if(b_i == 1) aExpb = modProd(aExpb, z, p);
		//z = modProd(z,z,p);
		
	}
	return aExpb;
}

//returns either 0 or 1 randomly
unsigned int randomBit() {
  return rand()%2;
}

//returns a random integer which is between 2^{n-1} and 2^{n}
unsigned int randXbitInt(unsigned int n) {
  unsigned int r = 1;
  for (unsigned int i=0; i<n-1; i++) {
    r = r*2 + randomBit();
  }
  return r;
}






// make an array of small prime numbers
// find r and d such that n-1 = (2^r) * d where d is odd
// miller rabin
// for all k in small prime list
// x = modExp(a,d,n)
// if x == 1 or x == n-1
	// continue to next k
// for i =1 to r-1
	// x = modProd(x,x,n)
	// if (x==1) return false
	// if (x==n-1) continue to next k
//return false
//return true





//tests for primality and return 1 if N is probably prime and 0 if N is composite
unsigned int isProbablyPrime(unsigned int N) {

  if (N%2==0) return 0; //not interested in even numbers (including 2)

  unsigned int NsmallPrimes = 168;
  unsigned int smallPrimeList[168] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 
                                37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 
                                79, 83, 89, 97, 101, 103, 107, 109, 113, 
                                127, 131, 137, 139, 149, 151, 157, 163, 
                                167, 173, 179, 181, 191, 193, 197, 199, 
                                211, 223, 227, 229, 233, 239, 241, 251, 
                                257, 263, 269, 271, 277, 281, 283, 293, 
                                307, 311, 313, 317, 331, 337, 347, 349, 
                                353, 359, 367, 373, 379, 383, 389, 397, 
                                401, 409, 419, 421, 431, 433, 439, 443, 
                                449, 457, 461, 463, 467, 479, 487, 491, 
                                499, 503, 509, 521, 523, 541, 547, 557, 
                                563, 569, 571, 577, 587, 593, 599, 601, 
                                607, 613, 617, 619, 631, 641, 643, 647, 
                                653, 659, 661, 673, 677, 683, 691, 701, 
                                709, 719, 727, 733, 739, 743, 751, 757, 
                                761, 769, 773, 787, 797, 809, 811, 821, 
                                823, 827, 829, 839, 853, 857, 859, 863, 
                                877, 881, 883, 887, 907, 911, 919, 929, 
                                937, 941, 947, 953, 967, 971, 977, 983, 
                                991, 997};

  //before using a probablistic primality check, check directly using the small primes list
  for (unsigned int n=1;n<NsmallPrimes;n++) {
    if (N==smallPrimeList[n])   return 1; //true
    if (N%smallPrimeList[n]==0) return 0; //false
  }

  //if we're testing a large number switch to Miller-Rabin primality test
  /* Q2.1: Complete this part of the isProbablyPrime function using the Miller-Rabin pseudo-code */
  unsigned int r,d;
  unsigned int current, i, x;
  
  current = N;
  i = 0;
  d = 1;
  unsigned int checker = 0;
  //1 will equal true which means done
  //0 will equal false which means not done
  while(checker == 0){
  	if(current == 2) 
  	{
  		checker = 1;
  		d = current;
  		r = i+1;
  	}
  	if(current % 2 == 1) 
  	{
  		d = current;
  		checker = 1;
  		r = i;
  	}
  	current = current / 2;
  	i = i+1;
  }

	for(unsigned int k=0; k<NsmallPrimes; k++) 
	{
			
			x = modExp(smallPrimeList[k], d, N);
			if( x == 1 || x == N-1)
			{
				continue;
			}
			
			for(int i; i< r-1; i++)
			{
				unsigned int x = modProd(x, x, N);
				if(x==1) return 0;
				if(x==N-1)
				{
					continue;
				}
			}
			
	}
	return 1;
	
	
  {  
  	for (unsigned int n=0;n<NsmallPrimes;n++) {
  		x = modProd(x,x,n);
  		if(x ==1) return 0;
  
  	}
  	return 0;
  }
  return 1; //true
}

//Finds a generator of Z_p using the assumption that p=2*q+1
//its either 2 or q
unsigned int findGenerator(unsigned int p) {
  /* Q3.3: complete this function and use the fact that p=2*q+1 to quickly find a generator */
  unsigned int q = (p-1)/2;
  for (int g=2;g<p;g++) {
  	if(modExp(g,2,p) != 1 && modExp(g,q,p) != 1)
  	{
  		if(modExp(g, p-1, p) == 1) return g;
  	} 
  	
  }
  
  
  
  
  
}
