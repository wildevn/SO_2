#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <queue.h>
#include <thread.h>

node_t	ready_queue;
tcb_t	*current_running;

int tid_global = 0;

/*
  TODO:  thread_init: initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
 */
int thread_init()
{
  tcb_t *main_tcb;
  node_t *main_node;

  if(is_empty(ready_queue) == 0)
    return EINVAL; // -EINVAL
  queue_init(&ready_queue);
  main_tcb = (tcb_t *) malloc(sizeof(tcb_t));
  main_tcb->thread_id = 0;
  main_tcb->status = 0; // FIRST TIME
  main_tcb->stack = (int *) malloc(STACK_SIZE * sizeof(int));
  main_tcb->sp = main_tcb->stack + 64*STACK_SIZE // unsure
  // Inicializar registradores
  main_node = (node_t *) malloc(sizeof(node_t));
  main_node->tcb = main_tcb;
  main_node->thread_id = main_tcb->thread_id;
  enqueue(ready_queue, main_node);
  current_running = main_tcb;
	return 0;
}

// TODO: creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{
  node_t *node;
  // Inicializing thread_tcb
  thread->tcb = (tcb_t *) malloc(sizeof(tcb_t));
  thread->tcb->thread_id = ++tid_global;
  thread->tcb->status = 0; 
  thread->tcb->stack = (int *) malloc(STACK_SIZE * sizeof(int));
  // inicializa sp && CORRIGIR STACK DEPOIS
  // Inicializing registers
  for (int i = 0; i < 15; i++)
    thread->tcb->regs[i] = NULL;
  thread->tcb->flags = NULL;
  // Preparing to sendo to ready_queue
  node = (node_t *) malloc(sizeof(node_t));
  node->thread_tcb = thread->tcb;
  node->thread_id = thread->thread_id;
  node->prox = NULL;
  enqueue(ready_queue, node);
	return 0;
}

// TODO: yields the CPU to another thread
int thread_yield()
{
  // Saves current_running thread tcb
  scheduler();
  // Load from tcb to registers
	return 0;
}

// TODO: waits for a thread to finish
int thread_join(thread_t *thread, int *retval) 
{
  // revisar esse retval
  thread->tcb->status = 1;
	return 0;
}

// TODO: marks a thread as EXITED and terminates the thread
void thread_exit(int status)
{
  if(status == 3) {
    current_running->status = 3

  }
}

// TODO: selects the next thread to execute
void scheduler()
{
  node_t q;

  if(is_empty(ready_queue->prox) == 0) {
    q = ready_queue->prox;
    while(1) {
    if(q->tcb->status == 1) {
      current_running = q->tcb;
    }  
  }
}

// TODO: you must  make sure this function is called  if a thread does
// not call thread_exit
void exit_handler()
{
}

