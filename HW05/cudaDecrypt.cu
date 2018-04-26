#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "cuda.h"
#include "functions.c"

//compute a*b mod p safely
__device__ unsigned int modprodCuda(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int za = a;
  unsigned int ab = 0;

  while (b > 0) {
    if (b%2 == 1) ab = (ab +  za) % p;
    za = (2 * za) % p;
    b /= 2;
  }
  return ab;
}

//compute a^b mod p safely
__device__ unsigned int modExpCuda(unsigned int a, unsigned int b, unsigned int p) {
  unsigned int z = a;
  unsigned int aExpb = 1;

  while (b > 0) {
    if (b%2 == 1) aExpb = modprodCuda(aExpb, z, p);
    z = modprodCuda(z, z, p);
    b /= 2;
  }
  return aExpb;
}



//device function
__global__ void kernelDecrypt(int n, int p, int g, int h, int x) {

  int threadid = threadIdx.x; //thread number
  int Nblock = blockDim.x;  //number of threads in a block
	int blockid = blockIdx.x; //block number

  int id = threadid + blockid*Nblock;


	if (x==0 || modExpCuda(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExpCuda(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }
}




int main (int argc, char **argv) {

  /* Part 2. Start this program by first copying the contents of the main function from 
     your completed decrypt.c main function. */
  //declare storage for an ElGamal cryptosytem
  unsigned int n, p, g, h, x;
  unsigned int Nints;

  //get the secret key from the user
  printf("Enter the secret key (0 if unknown): "); fflush(stdout);
  char stat = scanf("%u",&x);

  printf("Reading file.\n");

  /* Q3 Complete this function. Read in the public key data from public_key.txt
    and the cyphertexts from messages.txt. */
	int bufferSize = 1024;
  unsigned char *message = (unsigned char *) malloc(bufferSize*sizeof(unsigned char));
	char buf[bufferSize];
  FILE * fp;
  fp = fopen("public_key.txt", "r");
  if(!fp) {
    return 1;
  }

	fgets(buf, bufferSize, fp);
	sscanf(buf, "%d", &n);
	fgets(buf, bufferSize, fp);
	sscanf(buf, "%d", &p);
	fgets(buf, bufferSize, fp);
	sscanf(buf, "%d", &g);
	fgets(buf, bufferSize, fp);
	sscanf(buf, "%d", &h);

	fclose(fp);

	printf("%d, %d, %d, %d\n", n, p, g, h);
	fp = fopen("message.txt", "r");
	fgets(buf, bufferSize, fp);
	sscanf(buf, "%u", &Nints);

	unsigned int charsPerInt = (n-1) / 8;
  unsigned int Nchars = charsPerInt*Nints;

  //storage for message as elements of Z_p
  unsigned int *z = (unsigned int *) malloc(Nints*sizeof(unsigned int)); 
  
  //storage for extra encryption coefficient 
  unsigned int *a = (unsigned int *) malloc(Nints*sizeof(unsigned int));
	
	int i;
	for(i = 0; i < Nints; i++) {
		fgets(buf,bufferSize, fp);
		sscanf(buf, "%u%u", &z[i], &a[i]);
		printf("Z[i]: %u, A[i]: %u\n", z[i], a[i]);
	}

  // find the secret key
  if (x==0 || modExp(g,x,p)!=h) {
    printf("Finding the secret key...\n");
    double startTime = clock();
    for (unsigned int i=0;i<p-1;i++) {
      if (modExp(g,i+1,p)==h) {
        printf("Secret key found! x = %u \n", i+1);
        x=i+1;
      } 
    }
    double endTime = clock();

    double totalTime = (endTime-startTime)/CLOCKS_PER_SEC;
    double work = (double) p;
    double throughput = work/totalTime;

    printf("Searching all keys took %g seconds, throughput was %g values tested per second.\n", totalTime, throughput);
  }

  /* Q3 After finding the secret key, decrypt the message */
  //Decrypt the Zmessage with the ElGamal cyrptographic system
  ElGamalDecrypt(z,a,Nints,p,x);

  convertZToString(z, Nints, message, Nchars);

  printf("Decrypted Message = \"%s\"\n", message);
  printf("\n");

  /* Q4 Make the search for the secret key parallel on the GPU using CUDA. */
	//Device array
  double *d_x;

  //allocate memory on the Device with cudaMalloc
  cudaMalloc(&d_x, sizeof(double));


	int Nthreads = atoi(argv[2]); //get the number of threads per block from command line
  int Nblocks = (N+Nthreads-1)/Nthreads;


  kernelDecrypt <<<Nblocks ,Nthreads >>>(d_x);

	//copy data from the device to the host
  cudaMemcpy(x, d_x, sizeof(double), cudaMemcpyDeviceToHost);


  return 0;
}
