#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <threadu.h>

#include <util.h>

void *f1(void *);
void *f2(void *);

int main()
{
  int a = 1;
	
  f1(&a);
  return 0;
}

void *f1(void *a)
{
	int i;
	int *one = (int *)a;
  thread_t thd[6];
	int params[6], rv[6];
	printf("End Func1: %lu ------\nEnd Parm: %lu\n", (uint64_t )f1, (uint64_t)a);
	thread_init();
	printf("BEGIN: Time stamp: %ld\n", get_timer());
	for (i = 0; i < 6; i++) {
		params[i] = i;
		thread_create(&thd[i], f2, &params[i]);
    printf("oka\n");
	}

	for (i = 0; i < 6; i++) {
		thread_join(&thd[i], &rv[i]);
		printf("Thread %d finished with exit status %d\n", i, rv[i]);
	}
  while(1);
}

void *f2(void *p)
{
	int i;
	int *one = (int *)p;
	
	for (i = 0; i < 3; i++) {
		printf("f1 will sleep! %d\n", *one);
		sleep(1);
		printf("f1 will yield!\n");
		thread_yield();
	}
	thread_exit(*one);
}
