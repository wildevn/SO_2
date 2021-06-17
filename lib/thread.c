#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <queue.h>
#include <thread.h>

node_t	*ready_queue;
tcb_t	*current_running;
node_t *current_node;

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

  if(is_empty(ready_queue) == 0) {
    return -EINVAL;
  }
  ready_queue = (node_t *) malloc(sizeof(node_t));
  queue_init(ready_queue);
  main_tcb = (tcb_t *) malloc(sizeof(tcb_t));
  main_tcb->thread_id = 0;
  main_tcb->status = 0; // FIRST TIME
  main_tcb->stack = (uint64_t *) malloc(STACK_SIZE);
  //main_tcb->sp = main_tcb->stack + 8 * STACK_SIZE;
  main_node = (node_t *) malloc(sizeof(node_t));
  main_node->thread_tcb = main_tcb;
  main_node->thread_id = main_tcb->thread_id;
  current_node = main_node;
  current_running = main_tcb;
	return 0;
}

// TODO: creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg)
{
  tcb_t *tcb_aux;
  node_t *node;

  // Inicializing thread_tcb
  tcb_aux = (tcb_t *) malloc(sizeof(tcb_t));
  tcb_aux->thread_id = ++tid_global;
  tcb_aux->status = 0; 
  tcb_aux->stack = (uint64_t *) malloc(STACK_SIZE);
  //tcb_aux->sp = tcb_aux->stack + 8 * STACK_SIZE;
  //*tcb_aux->sp = (void *)*start_routine(arg);
  thread->tcb = tcb_aux; 
  for (int i = 0; i < 256; i++) {
    tcb_aux->stack[i] = (uint64_t) 0;
  }
    tcb_aux->stack[255] = (uint64_t) *start_routine;
    tcb_aux->stack[254] = (uint64_t) arg;
  for(int i = 0; i < 15; i++) {
    tcb_aux->regs[i] = (uint64_t) 0;
  }
  tcb_aux->regs[4] = tcb_aux->stack[255];
  tcb_aux->regs[3] = tcb_aux->stack[254];
  tcb_aux->flags = (uint64_t) 0;
  start_routine(arg);
  printf("Tcb alocado\n");
  // Preparing to send to ready_queue
  node = (node_t *) malloc(sizeof(node_t));
  node->thread_tcb = tcb_aux;
  node->thread_id = tcb_aux->thread_id;
  printf("Node alocado\n");
  enqueue(ready_queue, node);
  printf("Node na fila\n");
  thread_yield();
  printf("Troca de função");
  
  printf("Executado");
  
	return 0;
}

// TODO: yields the CPU to another thread
int thread_yield()
{
  printf("preparando a troca\n");
  enqueue(ready_queue, current_node); 
  scheduler_entry(); // vai fazer a troca de salvar o contexto e carregar o 
  printf("Finalizada a troca");
	return 0;
}

// TODO: waits for a thread to finish
int thread_join(thread_t *thread, int *retval) 
{
  tcb_t *tcb_aux;
  tcb_aux = (tcb_t *) thread->tcb;
  // revisar esse retval
  while(tcb_aux->status != 3)
    thread_yield();
  /*if(retval != endereçodoretorno)
	  return 0;*/
  return 0;
}

// TODO: marks a thread as EXITED and terminates the thread
void thread_exit(int status) // revisar o status
{
  if(1) {
    current_running->status = 3;
  }
}

// TODO: selects the next thread to execute
void scheduler()
{
  printf("Printando os registradores da thread %d:\n", current_running->thread_id);
  for(int i = 0; i < 15; i++)
    printf("reg[%d] = %lu\n", i, current_running->regs[i]);
  printf("Reg Flag = %lu\n", current_running->flags);
  tcb_t *tcb_aux;
  while(is_empty(ready_queue) == 0) {
    printf("Nao vazia scheduler\n");
    tcb_aux = (tcb_t *) ready_queue->prox->thread_tcb;
    if(tcb_aux->status <= 1) { // Verificar o que fazer quando não tiver 
      printf("Thread disponivel\n");
      current_running = tcb_aux; // thread disponível
      current_node = dequeue(ready_queue); 
      printf("Printando os registradores da thread %d:\n", current_running->thread_id);
      for(int i = 0; i < 15; i++)
        printf("reg[%d] = %lu\n", i, current_running->regs[i]);
      printf("Reg Flag = %lu\n", current_running->flags);
      return;
    }  
    enqueue(ready_queue, dequeue(ready_queue)); // Caso esteja bloqueada ou tenha 
    //saído
    printf("Thread passada indisponivel\n");
  }
}

// TODO: you must make sure this function is called  if a thread does 
// not call thread_exit
void exit_handler()
{
}
