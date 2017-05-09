// name:Jinhao Chen
// username:jinhao
// U94977930
// project2
/*ass1*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

// semaphore key
#define SEMKEY ((key_t) 400L)

// bumber of semaphores being created
#define NSEMS 1


/* change the key number */
#define SHMKEY ((key_t) 7890)

typedef unsigned short          ushort;

typedef struct
{
	int value;
} shared_mem;

shared_mem *total;

// GLOBAL

int sem_id; // semaphore id
// semaphore buffers
static struct sembuf OP = { 0, -1, 0 };
static struct sembuf OV = { 0, 1, 0 };
struct sembuf *P = &OP;
struct sembuf *V = &OV;

// semapore union used to generate semaphore
typedef union{
	int val;
	struct semid_ds *buf;
	ushort *array;
} semunion;


// Wait() function for semaphore
int Pop()
{
	int status;
	status = semop(sem_id, P, 1);
	return status;
}

// Signal() function for semaphore
int Vop()
{
	int status;
	status = semop(sem_id, V, 1);
	return status;
}



/*----------------------------------------------------------------------*
* This function increases the value of shared variable "total"
*  by one all the way to 100000
*----------------------------------------------------------------------*/

void process1()
{
	int k = 0;

	while (k < 100000)
	{
		Pop();
		if (total->value < 600000)
		{
			total->value = total->value + 1;
		}
		Vop();
		k++;
	}
	printf("From process1 total = %d\n", total->value);
}


/*----------------------------------------------------------------------*
* This function increases the vlaue of shared memory variable "total"
*  by one all the way to 200000
*----------------------------------------------------------------------*/

void process2()
{
	int k = 0;

	while (k < 200000)
	{
		Pop();
		if (total->value < 600000)
		{
			total->value = total->value + 1;
		}
		Vop();
		k++;
	}

	printf("From process2 total = %d\n", total->value);

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
		Pop();
		if (total->value < 600000)
		{
			total->value = total->value + 1;
		}
		Vop();
		k++;
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
	shmadd = (char *)0;

	//semaphores
	int   semnum = 0;
	int   value, value1;
	semunion semctl_arg;
	semctl_arg.val = 1;

	/* Create semaphores */
	sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
	if (sem_id < 0)
	{
		printf("Error in creating the semaphore./n");
	}
	/* Initialize semaphore */
	value1 = semctl(sem_id, semnum, SETVAL, semctl_arg);
	value = semctl(sem_id, semnum, GETVAL, semctl_arg);
	if (value < 1)
	{
		printf("Eror detected in SETVAL.\n");
	}


	/* Create and connect to a shared memory segmentt*/

	if ((shmid = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}


	if ((total = (shared_mem *)shmat(shmid, shmadd, 0)) == (shared_mem *)-1)
	{
		perror("shmat");
		exit(0);
	}


	total->value = 0;

	if ((pid1 = fork()) == 0)
		process1();

	if ((pid1 != 0) && (pid2 = fork()) == 0)
		process2();
	// To run the process3 function
	if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0)
		process3();

	if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0))
	{
		// Use the "wait" function so that the parent knows precisely
		// when each of the children finishes
		while ((kid = wait(NULL)) > 0);

		if ((shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
		{
			perror("shmctl");
			exit(-1);
		}
		printf("Child with ID: %d has just exited.\n", pid1);
		printf("Child with ID: %d has just exited.\n", pid2);
		printf("Child with ID: %d has just exited.\n", pid3);
		printf("\t\t  End of Program.\n");

		/* De-allocate semaphore */
		semctl_arg.val = 0;
		status = semctl(sem_id, 0, IPC_RMID, semctl_arg);
		if (status < 0)
			printf("Error in removing the semaphore.\n");
	}

}

/***** Note:  loop for parent to wait for child processes to finish and print ID of each child*****/


