// name:Jinhao Chen
// username:jinhao
// U94977930
// project1
/*ass1*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

/* change the key number */
#define SHMKEY ((key_t) 7890)


typedef struct
{
  int value;
} shared_mem;

shared_mem *total;




/*----------------------------------------------------------------------*
 * This function increases the value of shared variable "total"
 *  by one all the way to 100000
 *----------------------------------------------------------------------*/

void process1 ()
{
  int k = 0;
 
  while (k < 100000)
    {
      k++;
      total->value = total->value + 1;
    }
  printf ("From process1 total = %d\n", total->value);
}


/*----------------------------------------------------------------------*
 * This function increases the vlaue of shared memory variable "total"
 *  by one all the way to 200000
 *----------------------------------------------------------------------*/

void process2 ()
{
  int k = 0;

  while (k < 200000)
    {
      k++;
      
     total->value = total->value + 1;
    }
  
  printf ("From process2 total = %d\n", total->value);
  
}

/*----------------------------------------------------------------------*
* This function increases the vlaue of shared memory variable "total"
*  by one all the way to 300000
*----------------------------------------------------------------------*/

void process3()
{
	int k = 0;

	while (k < 300000)
	{
		k++;

		total->value = total->value + 1;
	}

	printf("From process3 total = %d\n", total->value);

}

/*----------------------------------------------------------------------*
 * MAIN()
 *----------------------------------------------------------------------*/

main()
{
  int   shmid;
  int   pid1;
  int   pid2;
  int   ID;
  int   pid3;
  pid_t kid;
  int   status;
 

  char *shmadd;
  shmadd = (char *) 0;

/* Create and connect to a shared memory segmentt*/

  if ((shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
    {
      perror ("shmget");
      exit (1);
    }

 
 if ((total = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) -1)
    {
      perror ("shmat");
      exit (0);
    }
  
  
  total->value = 0;

  if ((pid1 = fork()) == 0)
    process1();

  if ((pid1 != 0) && (pid2 = fork()) == 0)
    process2();
  // To run the process3 function
  if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0)
	  process3();

  if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) )
    {
		// Use the "wait" function so that the parent knows precisely
		// when each of the children finishes
		while ((kid = wait(NULL)) > 0);

      if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
	{
	  perror ("shmctl");
	  exit (-1);
	}
	  printf("Child with ID: %d has just exited.\n", pid1);
	  printf("Child with ID: %d has just exited.\n", pid2);
	  printf("Child with ID: %d has just exited.\n", pid3);
      printf ("\t\t  End of Program.\n");
    }
  
} 

/***** Note:  loop for parent to wait for child processes to finish and print ID of each child*****/


