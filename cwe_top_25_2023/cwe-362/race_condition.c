/********Software Analysis - FY2013*************/
/*
The file is extracted from the NIST database https://samate.nist.gov/SARD/test-cases/199300/versions/1.0.0

* File Name: race_condition.c
* Defect Classification
* ---------------------
* Defect Type: Concurrency defects
* Defect Sub-type: Race condition
* Description: Defect Free Code to identify false positives in race condition
*/


/* Race condition
 * Complexity: Thread 1 and thread 2 try to access and change the 
 * same resource at the same time.This leads to erroneous results.
*/


#include"HeaderFile.h"


pthread_mutex_t glb_1_mutex;
pthread_mutex_t glb_2_mutex;
int race_glb_1=5;
int race_glb_2=3;

void* race_condition_001_1()
{
	while(1)
	{
		pthread_mutex_lock(&glb_1_mutex);

		if(race_glb_1==5)
		{
			race_glb_1++; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
		}

		pthread_mutex_unlock(&glb_1_mutex);
		if(race_glb_1>50)
		{
			break;
		}
	}
	return NULL;
}

void* race_condition_001_2()
{
	while(1)
	{
		pthread_mutex_lock(&glb_2_mutex);

		race_glb_1=2+race_glb_1;

		pthread_mutex_unlock(&glb_2_mutex);
		if(race_glb_1>50)
		{
			break;
		}
	}
	return NULL;
}

void race_condition_001()
{
	pthread_t pthread1,pthread2;

	pthread_mutex_init(&glb_1_mutex,NULL);
	pthread_mutex_init(&glb_2_mutex,NULL);

	pthread_create(&pthread1,NULL,race_condition_001_1,NULL);
	pthread_create(&pthread2,NULL,race_condition_001_2,NULL);

	pthread_join(pthread1,NULL);
	pthread_join(pthread2,NULL);

}

/* Race condition
 * Complexity: Thread 1 and thread 2 try to access the 
 * same function at the same time.This leads to erroneous results.
*/


int race_condition_002_gbl=0;
pthread_mutex_t mutex_count;

void* race_condition_002_1()
{
	while(1)
	{
		pthread_mutex_lock(&mutex_count);
	
		if(race_condition_002_gbl<100)
		{
			race_condition_002_gbl++; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
		}

		pthread_mutex_unlock(&mutex_count);
	    break;
	}
	return NULL;
}


void race_condition_002()
{
	while(1)
	{
		pthread_t pthread1,pthread2;

		pthread_create(&pthread1,NULL,race_condition_002_1,NULL);
		pthread_create(&pthread2,NULL,race_condition_002_1,NULL);

		pthread_join(pthread1,NULL);
		pthread_join(pthread2,NULL);
	    break;
	}
}

/* Race condition
 * Complexity: 10 threads try to access the 
 * same variable.This leads to erroneous results if mutexes are not used.
*/

/* Prototype for thread function */
void* mythread(void * data);

/* Initialize a mutex */
  pthread_mutex_t count_mutex =PTHREAD_MUTEX_INITIALIZER;

/* Global variable which will be shared */
int x=0;

void race_condition_003()
{

		int i;
		pthread_t pth[10];

		for(i=0;i<10;i++)
			pthread_create(&pth[i],NULL,mythread,NULL);

		for(i=0;i<10;i++)
			pthread_join(pth[i],NULL);

}

void* mythread(void* data)
{
	pthread_mutex_lock(&count_mutex);

	while(x<100)
	{
		
		x++;  /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
	}

	pthread_mutex_unlock(&count_mutex);
	return NULL;
}

/*
 * Types of defects: Lock But Never Unlock
 * Complexity: Lock But Never Unlock in same function using one thread
 */

pthread_mutex_t race_condition_004_glb_mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined(CHECKER_POLYSPACE)
void race_condition_004_glb_mutex_lock () {}
void race_condition_004_glb_mutex_unlock () {}
#endif /* #if defined(CHECKER_POLYSPACE) */

int race_condition_004_glb_data = 0;

void * race_condition_004_tsk_001 (void *pram)
{
#if !defined(CHECKER_POLYSPACE)
	pthread_mutex_lock(&race_condition_004_glb_mutex);
	race_condition_004_glb_data = (race_condition_004_glb_data % 100) + 1;
	pthread_mutex_unlock(&race_condition_004_glb_mutex);

	unsigned long ip = (unsigned long)pthread_self();
    printf("Task1! Lock Never Unlock, threadID# %lu! gbl1 = %d \n",ip ,race_condition_004_glb_data);
#endif /* defined(CHECKER_POLYSPACE) */
    return NULL;
}

void race_condition_004 ()
{
#if !defined(CHECKER_POLYSPACE)
	pthread_t tid1;
	pthread_mutex_init(&race_condition_004_glb_mutex, NULL);
	pthread_create(&tid1, NULL, race_condition_004_tsk_001, NULL);
	pthread_join(tid1, NULL);
	pthread_mutex_destroy(&race_condition_004_glb_mutex);
#endif /* defined(CHECKER_POLYSPACE) */
}

/* Race condition
 * Complexity: 4 threads try to access the
 * same variable.This leads to erroneous results if mutexes are not used.
*/
pthread_mutex_t race_condition_005_glb_mutex = PTHREAD_MUTEX_INITIALIZER;
int race_condition_005_glb_data = 0;

void * race_condition_005_tsk_001 (void *pram)
{
#if !defined(CHECKER_POLYSPACE)
	pthread_mutex_lock(&race_condition_005_glb_mutex);
	race_condition_005_glb_data = (race_condition_005_glb_data % 100) + 1; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
	pthread_mutex_unlock(&race_condition_005_glb_mutex);
#endif /* defined(CHECKER_POLYSPACE) */
    return NULL;
}

void race_condition_005 ()
{
#if !defined(CHECKER_POLYSPACE)
	pthread_t tid1,tid2,tid3,tid4;
	pthread_mutex_init(&race_condition_005_glb_mutex, NULL);
	pthread_create(&tid1, NULL, race_condition_005_tsk_001, NULL);
	pthread_create(&tid2, NULL, race_condition_005_tsk_001, NULL);
	pthread_create(&tid3, NULL, race_condition_005_tsk_001, NULL);
	pthread_create(&tid4, NULL, race_condition_005_tsk_001, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_join(tid4, NULL);
	pthread_mutex_destroy(&race_condition_005_glb_mutex);
#endif /* defined(CHECKER_POLYSPACE) */
}

#if defined(CHECKER_POLYSPACE)
void race_condition_005_tskentry_001 ()
{
	while (1)
	{
		if (rand())
		{
			race_condition_005_tsk_001(NULL);
		}
	}
}
#endif /* defined(CHECKER_POLYSPACE) */

/*
 * Types of defects: Data Race Condition
 * Complexity: Locking twice and unlocking only once in same function using two thread
 */
pthread_mutex_t race_condition_006_glb_mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined(CHECKER_POLYSPACE)
void race_condition_006_glb_mutex_lock () {}
void race_condition_006_glb_mutex_unlock () {}
#endif /* #if defined(CHECKER_POLYSPACE) */

int race_condition_006_glb_data = 0;

void * race_condition_006_tsk_001 (void * pram)
{
#if !defined(CHECKER_POLYSPACE)
	pthread_mutex_lock(&race_condition_006_glb_mutex);
	race_condition_006_glb_data = (race_condition_006_glb_data % 100) + 1;
	pthread_mutex_unlock(&race_condition_006_glb_mutex);

	pthread_mutex_lock(&race_condition_006_glb_mutex);
	race_condition_006_glb_data = (race_condition_006_glb_data % 100) + 1; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
#if defined PRINT_DEBUG
	unsigned long ip = (unsigned long)pthread_self();
    printf("Task2! Lock Never Unlock, threadID # %lu! gbl2 = %d \n",ip ,race_condition_006_glb_data);
#endif /* defined(PRINT_DEBUG) */
    pthread_mutex_unlock(&race_condition_006_glb_mutex);

#endif /* defined(CHECKER_POLYSPACE) */
	return NULL;
}

void race_condition_006 ()
{
#if !defined(CHECKER_POLYSPACE)
	pthread_t tid1,tid2;
	pthread_mutex_init(&race_condition_006_glb_mutex, NULL);
	pthread_create(&tid1, NULL, race_condition_006_tsk_001, NULL);
	pthread_create(&tid2, NULL, race_condition_006_tsk_001, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

#endif /* defined(CHECKER_POLYSPACE) */
}

#if defined(CHECKER_POLYSPACE)
void race_condition_006_tskentry_001 ()
{
	while (1)
	{
		if (rand())
		{
			race_condition_006_tsk_001(NULL);
		}
	}
}
#endif /* defined(CHECKER_POLYSPACE) */

/*
 * Types of defects: Data Race Condition
 * Complexity: Data Race Condition over multiple functions , 2 threads
 */
pthread_mutex_t race_condition_007_glb_mutex = PTHREAD_MUTEX_INITIALIZER;

#if defined(CHECKER_POLYSPACE)
void race_condition_007_glb_mutex_lock () {}
void race_condition_007_glb_mutex_unlock () {}
#endif /* #if defined(CHECKER_POLYSPACE) */

float race_condition_007_glb_data = 1000.0;

void race_condition_007_func_001 (void *pram)
{
#if ! defined(CHECKER_POLYSPACE)
    pthread_mutex_lock (&race_condition_007_glb_mutex);
    race_condition_007_glb_data += 1.2; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/

    pthread_mutex_unlock (&race_condition_007_glb_mutex);
#endif /* ! defined(CHECKER_POLYSPACE) */
}

void* race_condition_007_tsk_001 (void *pram)
{
#if ! defined(CHECKER_POLYSPACE)

	pthread_mutex_lock(&race_condition_007_glb_mutex);
	race_condition_007_glb_data += 3.5;
	pthread_mutex_unlock(&race_condition_007_glb_mutex);
	race_condition_007_func_001(pram);
#endif /* defined(CHECKER_POLYSPACE) */
	return NULL;
}

void race_condition_007 ()
{
#if ! defined(CHECKER_POLYSPACE)
	pthread_t tid1,tid2;
	intptr_t t1 = 10, t2 = 20;
	pthread_mutex_init(&race_condition_007_glb_mutex, NULL);
	pthread_create(&tid1, NULL, race_condition_007_tsk_001, (void *)t1);
	pthread_create(&tid2, NULL, race_condition_007_tsk_001, (void *)t2);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_mutex_destroy(&race_condition_007_glb_mutex);
#endif /* defined(CHECKER_POLYSPACE) */
}

#if defined(CHECKER_POLYSPACE)
void race_condition_007_tskentry_001 ()
{
	while (1)
	{
		if (rand())
		{
			race_condition_007_tsk_001(NULL);
		}
	}
}
#endif /* defined(CHECKER_POLYSPACE) */

/*
 * Types of defects: Data Race Condition
 * Complexity: Data Race Condition over multiple functions using 2 threads unlocked based on if condition
 */
pthread_mutex_t race_condition_008_glb_mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t race_condition_008_glb_mutex_2 = PTHREAD_MUTEX_INITIALIZER;
float race_condition_008_glb_data = 1000.0;
void *race_condition_008_tsk_001(void *input)
{
#if ! defined(CHECKER_POLYSPACE)
	long ip;

   pthread_mutex_lock( &race_condition_008_glb_mutex_1 );
   ip = (long)input;
   ip = ip *10;
   race_condition_008_glb_data++; /*Tool should not detect this line as error*/ /*No ERROR:Race condition*/
#if defined PRINT_DEBUG
   printf("Task4_1! Lock Never Unlock, thread #%ld!\n",ip);
#endif /* defined(PRINT_DEBUG) */


   pthread_mutex_unlock( &race_condition_008_glb_mutex_1 );
#endif /* defined(CHECKER_POLYSPACE) */

   return NULL;
}

void * race_condition_008_tsk_002(void *input)
{
#if ! defined(CHECKER_POLYSPACE)
	long ip;

   pthread_mutex_lock( &race_condition_008_glb_mutex_2 );
   ip = (long)input;
   ip = ip *20;
   race_condition_008_glb_data--;
#if defined PRINT_DEBUG
   printf("Task4_2! Lock Never Unlock, thread #%ld!\n",ip);
#endif /* defined(PRINT_DEBUG) */
   pthread_mutex_unlock( &race_condition_008_glb_mutex_2 );
#endif /* defined(CHECKER_POLYSPACE) */
   return NULL;
}

void race_condition_008 ()
{
#if ! defined(CHECKER_POLYSPACE)
	pthread_t th1,th2;
        intptr_t t1 = 10;
        intptr_t t2 = 20;
	   pthread_create(&th1, NULL, race_condition_008_tsk_001, (void *)t1);
	   pthread_create(&th2, NULL, race_condition_008_tsk_002, (void *)t2);
	   sleep(1);
#endif /* defined(CHECKER_POLYSPACE) */
}

extern volatile int vflag;
void race_condition_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		race_condition_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		race_condition_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		race_condition_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		race_condition_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		race_condition_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		race_condition_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		race_condition_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		race_condition_008();
	}
}
