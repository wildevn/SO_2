#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>
#include <thread.h>

node_t	*ready_queue;
tcb_t	*current_running;
node_t *current_node;

int tid_global = 0;

/*
  thread_init: initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
*/
int thread_init()
{
  tcb_t *main_tcb;
  node_t *main_node;

  // Verifies if the ready_queue is initialized
  if(is_empty(ready_queue) == 0) {
    return -EINVAL;
  }
  // Inicializing ready_queue
  ready_queue = (node_t *) malloc(sizeof(node_t));
  queue_init(ready_queue);
  // Inicializing thread_tcb
  main_tcb = (tcb_t *) malloc(sizeof(tcb_t));
  main_tcb->thread_id = 0;
  main_tcb->status = 1; 
  // Inicializing thread->stack
  main_tcb->stack = (uint64_t **) malloc(STACK_SIZE * sizeof(uint64_t *));
  for (int i = 0; i < STACK_SIZE; i++)
    main_tcb->stack[i] = (uint64_t *) malloc(sizeof(uint64_t *));
  main_tcb->flags = (uint64_t) main_tcb->stack[2048];
  // Inicializing thread->node
  main_node = (node_t *) malloc(sizeof(node_t));
  main_node->thread_tcb = main_tcb;
  main_node->thread_id = main_tcb->thread_id;
  // Setting current thread running
  current_node = main_node;
  current_running = main_tcb;
	return 0;
}

// Creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{
  tcb_t *tcb_aux;
  node_t *node;

  // Inicializing thread_tcb
  tcb_aux = (tcb_t *) malloc(sizeof(tcb_t));
  tcb_aux->thread_id = ++tid_global;
  tcb_aux->status = 1; 
  tcb_aux->stack = (uint64_t **) malloc(STACK_SIZE * sizeof(uint64_t *));
  for (int i = 0; i < STACK_SIZE; i++)
    tcb_aux->stack[i] = (uint64_t *) malloc(sizeof(uint64_t *));
  thread->tcb = tcb_aux; 
  // Inicializing thread->stack
  for (int i = 0; i < 2048; i++) {
    tcb_aux->stack[i] = (uint64_t) 0;
  }
  tcb_aux->stack[2047] = (uint64_t *) *start_routine;
  // Inicializing registers
  for(int i = 0; i < 15; i++) {
    tcb_aux->regs[i] = (uint64_t) 0;
  }
  tcb_aux->regs[9] = (uint64_t) arg;
  tcb_aux->flags = (uint64_t) tcb_aux->stack[2047];
  // Inicializing thread->node
  node = (node_t *) malloc(sizeof(node_t));
  node->thread_tcb = tcb_aux;
  node->thread_id = tcb_aux->thread_id;

  enqueue(ready_queue, node);
  thread_yield();
  
	return 0;
}

// Yields the CPU to another thread
int thread_yield()
{
  enqueue(ready_queue, current_node); // Enqueue the thread running on ready queue
  scheduler_entry(); // Saves the current thread context and loads the next
  current_node = dequeue(ready_queue); // Dequeue the first thread on ready queue
	return 0;
}

// Waits for a thread to finish
int thread_join(thread_t *thread, int *retval) 
{
  tcb_t *tcb_aux;
  tcb_aux = (tcb_t *) thread->tcb;
  while(tcb_aux->status != 3)
    thread_yield();
  return 0;
}

// Marks a thread as EXITED and terminates the thread
void thread_exit(int status)
{
  if(1) {
    current_running->status = 3;
    thread_yield(); // Yields the exitted thread
  }
}

// Selects the next thread to execute
void scheduler()
{
  tcb_t *tcb_aux;
  while(is_empty(ready_queue) == 0) {
    tcb_aux = (tcb_t *) ready_queue->prox->thread_tcb;
    if(tcb_aux->status == 1) { 
      current_running = tcb_aux;
      return;
    }  
    // If the thread verified is blocked or exit
    enqueue(ready_queue, dequeue(ready_queue));
  }
}

// TODO: you must make sure this function is called  if a thread does 
// not call thread_exit
void exit_handler()
{
}
