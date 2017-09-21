/* ----------------------------------------------------------------------------
Name:		processes.c

Required:	processes.h	

Developer:	Mat Siwoski

Created On: 	2017-01-17

Description:
	Runs a multiprocesses version of the sieve of Eratosthenes, one of a number of prime number sieves, 
	is a simple, ancient algorithm for finding all prime numbers up to any given limit. 
	It does so by iteratively marking as composite (i.e., not prime) the multiples of 
	each prime, starting with the multiples of 2. The program runs with user defined processes.
Revisions:
	(none)

---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <math.h>

#include "processes.h"

/* ----------------------------------------------------------------------------
FUNCTION

Name:		main

Prototype:	int main(int argc, char *argv[]) 

Developer:	Mat Siwoski

Created On: 	2017-01-17

Parameters:
	argc - Number of arguments
	argv - Arguments
	
Return Values:
	

Description:
	Runs the multiproccessor version of sieve of Eratosthenes and outputs the
	results into a text file. 

Revisions:
	(none)

---------------------------------------------------------------------------- */
int main(int argc, char *argv[]) 
{
	pid_t *processes;
	pid_t tmp_process; 
	int i, n, status;
	FILE *fp1;

	if (argc != 2)
	{   /* check for valid number of command-line arguments */ 
      		fprintf(stderr, "Usage: %s processes\n", argv[0]);
      		return 1; 
   	}     

   	n = atoi(argv[1]); 	
	
	fp1 = fopen("OverallTimeProcesses.txt", "w");
	if (fp1 == NULL)
	{
		return 1;
	}

	struct timeval start,end;

	processes = (pid_t*)malloc(n * sizeof(pid_t));

	gettimeofday(&start, NULL);

	for(i = 0; i< n; i++)
	{
		tmp_process = fork();
		if(tmp_process == 0){
			recordIntoFile();
			return 0;
		}
		else 
		{
			processes[i] = tmp_process;
		}
	}
	//wait for processes to complete
	for(i = 0; i < n; i++) {
		
		waitpid(processes[i], &status, 0);
	}
	gettimeofday(&end, NULL);
	fprintf(fp1, "Time to run: %d processes of the Sieve in microseconds: %ld microseconds\n", n, ((end.tv_sec - start.tv_sec)*1000000L + end.tv_usec) - start.tv_usec); 
	free(processes);
	
	fclose(fp1);

	return 0;
}

/* ----------------------------------------------------------------------------
FUNCTION

Name:		Sieve

Prototype:	void sieve(FILE *fp, int *a, int n)

Developer:	Mat Siwoski/https://rosettacode.org/wiki/Sieve_of_Eratosthenes

Created On: 2017-01-17

Parameters:
	fp - 	File pointer to print info for the sieve 
	array - Hold the numbers
	n -	What number to find all the prime numbers for

Return Values:
	

Description:
	In mathematics, the sieve of Eratosthenes, one of a number of prime number sieves, 
	is a simple, ancient algorithm for finding all prime numbers up to any given limit. 
	It does so by iteratively marking as composite (i.e., not prime) the multiples of 
	each prime, starting with the multiples of 2

Revisions:
	(none)

---------------------------------------------------------------------------- */
void sieve(FILE *fp, int *a, int n)
{
    int i=0, j=0;
 
    for(i=2; i<=n; i++) {
        a[i] = 1;
    }
 
    for(i=2; i<=n; i++) {
        fprintf(fp, "\ni:%d", i);
        if(a[i] == 1) {
            for(j=i; (i*j)<=n; j++) {
                fprintf(fp, "\nj:%d", j);
                fprintf(fp, "\nBefore a[%d*%d]: %d", i, j, a[i*j]);
                a[(i*j)] = 0;
                fprintf(fp, "\nAfter a[%d*%d]: %d", i, j, a[i*j]);
            }
        }
    }
 
    fprintf(fp, "\nPrimes numbers from 1 to %d are : ", n);
    for(i=2; i<=n; i++) {
        if(a[i] == 1)
            fprintf(fp, "%d, ", i);
    }
    fprintf(fp, "\n\n");
}

/* ----------------------------------------------------------------------------
FUNCTION

Name:		RecordIntoFile

Prototype:	void recordIntoFile

Developer:	Mat Siwoski

Created On: 2017-01-17

Parameters:

Return Values:

Description:
	Purpose is to record the time it takes to do each individual sieve and
	to record the individual sieve results into external files for data.

Revisions:
	(none)

---------------------------------------------------------------------------- */
void recordIntoFile()
{	
 	int *array, n=10000;
	array =(int *)malloc(sizeof(int)*n);
	
	FILE *fp2,*fp3;
	fp2 = fopen("TimeProcesses.txt", "w");
	if (fp2 == NULL)
	{
		return;
	}

	fp3 = fopen("SieveProcesses.txt", "w");
	if (fp3 == NULL)
	{
		return;
	}


	struct timeval beginning,ending;

	gettimeofday(&beginning, NULL);
	
	//math problem
	sieve(fp3, array, n);	
	
	gettimeofday(&ending, NULL);
	fprintf(fp2, "Time in microseconds: %ld microseconds\n", ((ending.tv_sec - beginning.tv_sec)*1000000L + ending.tv_usec) - beginning.tv_usec); 
	
	fclose(fp3);
	fclose(fp2);
}
