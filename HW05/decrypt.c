#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "functions.h"


int main (int argc, char **argv) {

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

  return 0;
}
