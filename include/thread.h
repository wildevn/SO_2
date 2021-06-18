#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <threadu.h>

#define NUMBER_OF_REGISTERS	15
#define STACK_SIZE		2048

typedef enum {
	      FIRST_TIME,
	      READY,
	      BLOCKED,
	      EXITED
} status_t;

typedef struct tcb {
  int thread_id; 
	status_t status;
  uint64_t **stack; 
	uint64_t regs[NUMBER_OF_REGISTERS];
  uint64_t flags;
} tcb_t;

void scheduler_entry();
void exit_handler();

#endif /* THREAD_H */
